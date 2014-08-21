/* <orly/indy/fiber/fiber.cc>

   Implements <orly/indy/fiber/fiber.h>.

   Copyright 2010-2014 OrlyAtomics, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#include <orly/indy/fiber/fiber.h>

#include <xmmintrin.h>

#include <thread>

using namespace std::literals;
using namespace Orly::Indy::Fiber;

__thread TRunner *TRunner::LocalRunner = nullptr;
__thread TFrame *TFrame::LocalFrame = nullptr;
__thread Base::TThreadLocalGlobalPoolManager<TFrame, size_t, TRunner *>::TThreadLocalPool *TFrame::LocalFramePool = nullptr;
FiberLocal::TFiberLocal *FiberLocal::TFiberLocal::Root = nullptr;

/********************************************************/
/******************* EXTERN FIBER ***********************/
/********************************************************/

void Orly::Indy::ExternFiber::SchedTaskLocally(const std::function<void ()> &func) {
  class TRunFunc : public TRunnable {
    NO_COPY(TRunFunc);
    public:
    TRunFunc(const std::function<void ()> &func)
        : FramePool(Indy::Fiber::TFrame::LocalFramePool),
          Func(func) {
      assert(FramePool);
      auto *frame = FramePool->Alloc();
      assert(frame);
      try {
        frame->Latch(TRunner::LocalRunner, this, static_cast<Indy::Fiber::TRunnable::TFunc>(&TRunFunc::Run));
      } catch (...) {
        Indy::Fiber::TFrame::LocalFramePool->Free(frame);
        throw;
      }
    }
    ~TRunFunc() {
      assert(this);
      FreeMyFrame(FramePool);
    }
    void Run() {
      Func();
      delete this;
    }
    private:
    Base::TThreadLocalGlobalPoolManager<Indy::Fiber::TFrame, size_t, Indy::Fiber::TRunner *>::TThreadLocalPool *FramePool;
    const std::function<void ()> Func;
  };
  new TRunFunc(func);
}

Orly::Indy::ExternFiber::TSync::TSync(size_t waiting_for) {
  new (GetImpl()) Fiber::TSync(waiting_for);
}

Orly::Indy::ExternFiber::TSync::~TSync() {
  assert(this);
  GetImpl()->~TSync();
}

void Orly::Indy::ExternFiber::TSync::Sync(bool come_back_right_away) {
  assert(this);
  GetImpl()->Sync(come_back_right_away);
}

void Orly::Indy::ExternFiber::TSync::Complete() {
  assert(this);
  GetImpl()->Complete();
}

void Orly::Indy::ExternFiber::TSync::WaitForMore(size_t num) {
  assert(this);
  GetImpl()->WaitForMore(num);
}

/********************************************************/
/***************** END EXTERN FIBER *********************/
/********************************************************/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
void TRunner::Run() {
  assert(this);
  LocalRunner = this;
  TFrame *rt_queue = nullptr; /* we use this to push come back right away jobs to the front... */
  TFrame *next_frame = nullptr; /* we use this to loop through a queue... */
  try {
    const size_t laps_before_short_sleep = 100UL;
    const size_t laps_before_long_sleep = 100UL;
    size_t laps_without_work = 0UL;
    for (; likely(KeepRunning.load());) {
      assert(!ReadyToRunQueue);
      /* check for inbound frames */ {
        if (InboundFrameQueue) {
          assert(rt_queue == nullptr);
          TFrame *cur_tail = __sync_lock_test_and_set(&InboundFrameQueue, nullptr);
          for (TFrame *frame = cur_tail; frame; frame = next_frame) {
            assert(frame->InboundQueueNextFrame != frame);
            next_frame = frame->InboundQueueNextFrame;
            if (!frame->ComeBackRightAway) {
              frame->InboundQueueNextFrame = ReadyToRunQueue;
              ReadyToRunQueue = frame;
              //frame->QueueMembership.Insert(&MyFrameQueue, InvCon::Rev);
            } else {
              frame->InboundQueueNextFrame = rt_queue;
              rt_queue = frame;
            }
            //printf("TRunner [%p] push frame [%p]\n", this, frame);
          }
          /* here we have to choose between putting the most recent or least recent "high priority" (come_back_soon) fiber first. We currently
             implement the more recent one (as opposed to fair one) because it's most likely to still have data in the cache... */
          for (TFrame *frame = rt_queue; frame; frame = next_frame) {
            //frame->QueueMembership.Insert(&MyFrameQueue, InvCon::Rev);
            next_frame = frame->InboundQueueNextFrame;
            frame->InboundQueueNextFrame = ReadyToRunQueue;
            ReadyToRunQueue = frame;
          }
          rt_queue = nullptr;
        } else {
          for (size_t i = 0; i < TotalNumRunners; ++i) {
            TRunner *cur_runner = RunnerArray[i];
            if (cur_runner) {
              TFrame *&cur_inbound_queue = cur_runner->QueueArray[RunnerId].Ptr;
              if (cur_inbound_queue) {
                assert(rt_queue == nullptr);
                TFrame *cur_tail = __sync_lock_test_and_set(&cur_inbound_queue, nullptr);
                for (TFrame *frame = cur_tail; frame; frame = next_frame) {
                  assert(frame->InboundQueueNextFrame != frame);
                  next_frame = frame->InboundQueueNextFrame;
                  if (!frame->ComeBackRightAway) {
                    frame->InboundQueueNextFrame = ReadyToRunQueue;
                    ReadyToRunQueue = frame;
                    //frame->QueueMembership.Insert(&MyFrameQueue, InvCon::Rev);
                    //_mm_prefetch(frame->MyFiber.jmp, _MM_HINT_T1);
                  } else {
                    frame->InboundQueueNextFrame = rt_queue;
                    rt_queue = frame;
                  }
                  //printf("TRunner [%p] push frame [%p]\n", this, frame);
                }
                /* here we have to choose between putting the most recent or least recent "high priority" (come_back_soon) fiber first. We currently
                   implement the more recent one (as opposed to fair one) because it's most likely to still have data in the cache... */
                for (TFrame *frame = rt_queue; frame; frame = next_frame) {
                  next_frame = frame->InboundQueueNextFrame;
                  frame->InboundQueueNextFrame = ReadyToRunQueue;
                  ReadyToRunQueue = frame;
                  //frame->QueueMembership.Insert(&MyFrameQueue, InvCon::Rev);
                  #ifdef FAST_SWITCH
                  _mm_prefetch(frame->MyFiber.jmp, _MM_HINT_T1);
                  #endif
                }
                rt_queue = nullptr;
              }
            }
          }
        }
      }
      if (ReadyToRunQueue) {
        laps_without_work = 0UL;
      } else {
        ++laps_without_work;
        if (laps_without_work >= laps_before_long_sleep) {
          std::this_thread::sleep_for(10000ns);
        } else if (laps_without_work >= laps_before_short_sleep) {
          std::this_thread::sleep_for(100000ns);
        }
      }
      for (;;) {
        for (TFrame *frame = ReadyToRunQueue; ReadyToRunQueue; frame = ReadyToRunQueue) {
          ReadyToRunQueue = frame->InboundQueueNextFrame;
          _mm_prefetch(reinterpret_cast<uint8_t *>(ReadyToRunQueue) + offsetof(TFrame, MyFiber), _MM_HINT_T0);
          fiber_t *sched_fib = &frame->GetFiber();
          TFrame::LocalFrame = frame;
          FreeFrame = nullptr;
          FreeFramePool = nullptr;
          //printf("[%p]\tSwitch to Frame\n", this);
          switch_to_fiber(*sched_fib, MainFiber);
          //printf("[%p]\tDone Frame\n", this);
          if (FreeFrame) {
            assert(FreeFrame == frame);
            assert(FreeFramePool);
            FreeFramePool->Free(FreeFrame);
            FreeFrame = nullptr;
            FreeFramePool = nullptr;
          }
          if (ForeignRunnerToMoveFrameTo) {
            assert(FrameToMoveToForeignRunner);
            ScheduleFrameSlow(ForeignRunnerToMoveFrameTo, FrameToMoveToForeignRunner);
            ForeignRunnerToMoveFrameTo = nullptr;
            FrameToMoveToForeignRunner = nullptr;
          }
          TFrame::LocalFrame = nullptr;
        }
        if (NewReadyToRunQueue) {
          assert(!ReadyToRunQueue);
          std::swap(ReadyToRunQueue, NewReadyToRunQueue);
          assert(ReadyToRunQueue);
          assert(!NewReadyToRunQueue);
        } else {
          break;
        }
      }
    }
  } catch (const std::exception &ex) {
    syslog(LOG_INFO, "TRunner [%p] caught exception [%s]", this, ex.what());
  } catch (...) {
    LocalRunner = nullptr;
    throw;
  }
  LocalRunner = nullptr;
}

#pragma GCC diagnostic pop