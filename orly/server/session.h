/* <orly/server/session.h>

   An user's session.

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

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <vector>

#include <base/class_traits.h>
#include <base/event_semaphore.h>
#include <base/opt.h>
#include <base/sigma_calc.h>
#include <base/thrower.h>
#include <base/uuid.h>
#include <orly/durable/kit.h>
#include <orly/indy/fiber/fiber.h>
#include <orly/method_request.h>
#include <orly/method_result.h>
#include <orly/notification/notification.h>
#include <orly/package/manager.h>
#include <orly/server/pov.h>

namespace Orly {

  namespace Server {

    /* An open session. */
    class TSession final
        : public Durable::TObj {
      NO_COPY(TSession);
      public:

      /* Convenience. */
      using TNotification = Notification::TNotification;

      /* TODO */
      class TServer
          : public TPov::TServer {
        public:

        /* TODO */
        virtual ~TServer() {}

        /* TODO */
        virtual const std::shared_ptr<Durable::TManager> &GetDurableManager() const = 0;

        /* TODO */
        virtual const Package::TManager &GetPackageManager() const = 0;

        /* TODO */
        virtual Base::TScheduler *GetScheduler() const = 0;

        /* TODO */
        static Base::TSigmaCalc TryReadTimeCalc;
        static Base::TSigmaCalc TryReadCPUTimeCalc;
        static Base::TSigmaCalc TryWriteTimeCalc;
        static Base::TSigmaCalc TryWriteCPUTimeCalc;
        static Base::TSigmaCalc TryWalkerCountCalc;
        static Base::TSigmaCalc TryWalkerConsTimerCalc;
        static Base::TSigmaCalc TryCallCPUTimerCalc;
        static Base::TSigmaCalc TryReadCallTimerCalc;
        static Base::TSigmaCalc TryWriteCallTimerCalc;
        static Base::TSigmaCalc TryFetchCountCalc;
        static Base::TSigmaCalc TryHashHitCountCalc;

        static Base::TSigmaCalc TryWriteSyncHitCalc;
        static Base::TSigmaCalc TryWriteSyncTimeCalc;
        static Base::TSigmaCalc TryReadSyncHitCalc;
        static Base::TSigmaCalc TryReadSyncTimeCalc;

        static std::mutex TryTimeLock;

        protected:

        /* TODO */
        TServer(size_t num_runners) : RunnerCons(num_runners), SlowAssignmentCounter(0UL), FastAssignmentCounter(0UL) {}

        /* TODO */
        void InitalizeFramePoolManager(size_t num_frames, size_t frame_stack_size, Indy::Fiber::TRunner *runner) {
          FramePoolManager = std::unique_ptr<Base::TThreadLocalGlobalPoolManager<Indy::Fiber::TFrame, size_t, Indy::Fiber::TRunner *>>(
            new Base::TThreadLocalGlobalPoolManager<Indy::Fiber::TFrame, size_t, Indy::Fiber::TRunner *>(num_frames, frame_stack_size, runner));
        }

        /* TODO */
        Indy::Fiber::TRunner::TRunnerCons RunnerCons;
        std::vector<std::unique_ptr<Indy::Fiber::TRunner>> SlowRunnerVec;
        std::vector<std::unique_ptr<std::thread>> SlowRunnerThreadVec;
        std::atomic<size_t> SlowAssignmentCounter;
        std::unique_ptr<Base::TThreadLocalGlobalPoolManager<Indy::Fiber::TFrame, size_t, Indy::Fiber::TRunner *>> FramePoolManager;
        std::vector<std::unique_ptr<Indy::Fiber::TRunner>> FastRunnerVec;
        std::atomic<size_t> FastAssignmentCounter;
        std::vector<std::unique_ptr<std::thread>> FastRunnerThreadVec;

        friend class TSession;

      };  // TServer

      /* Temporary.  Once all the RPC entry points are implmented, remove this error. */
      DEFINE_ERROR(TStubbed, std::logic_error, "the RPC entry point is not yet implemented");

      /* See <orly/protocol.h>. */
      TMethodResult DoInPast(
          TServer *server, const Base::TUuid &pov_id, const std::vector<std::string> &fq_name, const TClosure &closure,
          const Base::TUuid &tracking_id);

      /* Call back for each pending notification, in order of increasing sequence number. */
      bool ForEachNotification(const std::function<bool (uint32_t, const TNotification *)> &cb) const;

      /* TODO */
      const TNotification *GetFirstNotification(uint32_t &seq_number);

      /* TODO */
      virtual const char *GetKind() const noexcept override {
        assert(this);
        return "Session";
      }

      /* The number of pending notifications. */
      size_t GetNotificationCount() const {
        assert(this);
        std::lock_guard<std::mutex> lock(NotificationMutex);
        return NotificationBySeqNumber.size();
      }

      /* TODO */
      const Base::TEventSemaphore &GetNotificationSem() const {
        assert(this);
        return NotificationSem;
      }

      /* The id of the user who owns this session.  If the session is anonymous, this is unknown. */
      const Base::TOpt<Base::TUuid> &GetUserId() const {
        assert(this);
        return UserId;
      }

      /* See <orly/protocol.h>. */
      Base::TUuid NewFastPrivatePov(TServer *server, const Base::TOpt<Base::TUuid> &parent_pov_id, const std::chrono::seconds &time_to_live);

      /* See <orly/protocol.h>. */
      Base::TUuid NewFastSharedPov(TServer *server, const Base::TOpt<Base::TUuid> &parent_pov_id, const std::chrono::seconds &time_to_live);

      /* See <orly/protocol.h>. */
      Base::TUuid NewSafePrivatePov(TServer *server, const Base::TOpt<Base::TUuid> &parent_pov_id, const std::chrono::seconds &time_to_live);

      /* See <orly/protocol.h>. */
      Base::TUuid NewSafeSharedPov(TServer *server, const Base::TOpt<Base::TUuid> &parent_pov_id, const std::chrono::seconds &time_to_live);

      /* See <orly/protocol.h>. */
      void PausePov(TServer *server, const Base::TUuid &pov_id);

      /* Insert the given notification into the pending set and return the sequence number that is assigned to it.
         If this function fails, it will delete the notification before throwing. */
      uint32_t InsertNotification(Notification::TNotification *notification);

      /* Remove the notification with the given sequence number.
         If notification doesn't exist (never existed or has already been discarded), do nothing. */
      void RemoveNotification(uint32_t seq_number);

      /* See <orly/protocol.h>. */
      void SetTimeToLive(TServer *server, const Base::TUuid &durable_id, const std::chrono::seconds &ttl);

      /* See <orly/protocol.h>. */
      void SetUserId(TServer *server, const Base::TUuid &user_id);

      /* See <orly/protocol.h>. */
      TMethodResult Try(TServer *server, const Base::TUuid &pov_id, const std::vector<std::string> &fq_name, const TClosure &closure);

      /* See <orly/protocol.h>. */
      bool RunTestSuite(TServer *server, const std::vector<std::string> &package_name, uint64_t package_version, bool verbose);

      /* Return the notification with the given sequence number.  If there is no such notification, return null. */
      TNotification *TryGetNotification(uint32_t seq_number) const;

      /* See <orly/protocol.h>. */
      TMethodResult TryTracked(TServer *server, const Base::TUuid &pov_id, const std::vector<std::string> &fq_name, const TClosure &closure);

      /* See <orly/protocol.h>. */
      void UnpausePov(TServer *server, const Base::TUuid &pov_id);

      /* The id of the global point of view. */
      static const Base::TUuid GlobalPovId;

      //TODO: This should be private...
      /* Add the given pov to the collection of povs we'll keep open. */
      void AddPov(const Durable::TPtr<TPov> &pov);

      private:

      /* TODO */
      TSession(Durable::TManager *manager, const Base::TUuid &id, const Durable::TTtl &ttl);

      /* TODO */
      TSession(Durable::TManager *manager, const Base::TUuid &id, Io::TBinaryInputStream &strm);

      /* Calls Cleanup(). */
      virtual ~TSession();

      /* Create a private POV that is a child of the POV represented by
         'parent_pov_id'.  Execute 'func' in that POV and push any size effects
         to the parent. */
      void RunInPrivateChildPov(TServer *server,
          const std::vector<std::string> &package_name,
          const std::function<void(Package::TContext &ctx)> &func,
          const Base::TUuid &parent_pov_id);

      bool RunTestBlock(const Base::TUuid &parent_pov_id,
          const Package::TTestBlock &test_block, bool verbose);

      /* Stream out. */
      virtual void Write(Io::TBinaryOutputStream &strm) const override;

      /* TODO */
      void Cleanup();

      /* TODO */
      Base::TUuid NewPov(
          TServer *server, const Base::TOpt<Base::TUuid> &parent_pov_id, TPov::TAudience audience, TPov::TPolicy policy,
          const std::chrono::seconds &time_to_live);

      /* See base class. */
      virtual bool ForEachDependentPtr(const std::function<bool (Durable::TAnyPtr &)> &cb) noexcept override;

      /* See accessor. */
      Base::TOpt<Base::TUuid> UserId;

      /* The sequence number to assign to the next notification. */
      uint32_t NextSeqNumber;

      /* The queue of pending notifications. */
      std::map<uint32_t, TNotification *> NotificationBySeqNumber;
      mutable std::mutex NotificationMutex;
      mutable Base::TEventSemaphore NotificationSem;

      /* Povs to keep alive while we're alive. */
      std::vector<Durable::TPtr<TPov>> Povs;
      std::mutex PovMutex;

      /* For access to constructors/destructor. */
      friend class Durable::TManager;

    };  // TSession

  }  // Server

}  // Orly
