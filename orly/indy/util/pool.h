/* <orly/indy/util/pool.h>

   TODO

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

#include <atomic>
#include <cassert>
#include <cstddef>
#include <mutex>
#include <new>
#include <thread>
#include <syslog.h>

#include <base/class_traits.h>

namespace Orly {

  namespace Indy {

    namespace Util {

      /* TODO */
      class TPool {
        NO_COPY(TPool);
        public:

        /* TODO */
        TPool(size_t block_size, const char *name, size_t block_count = 0UL);

        /* TODO */
        ~TPool();

        /* TODO */
        void Init(size_t block_count);

        /* TODO */
        inline const char *GetName() const;

        /* TODO */
        inline size_t GetNumBlocksUsed() const;

        /* TODO */
        inline size_t GetMaxBlocks() const;

        /* TODO */
        void *Alloc(size_t size) {
          assert(this);
          void *ptr = TryAlloc(size);
          if (!ptr) {
            size_t retry = 2000UL;
            while (!ptr && retry) {
              --retry;
              std::this_thread::sleep_for(std::chrono::milliseconds(1));
              ptr = TryAlloc(size);
            }
            if (ptr) {
              return ptr;
            }
            syslog(LOG_EMERG, "TPool::Alloc() [%s] bad_alloc after %ld retries", Name, 2000UL);
            throw std::bad_alloc();
          }
          return ptr;
        }

        /* TODO */
        void Free(void *ptr);

        /* TODO */
        void *TryAlloc(size_t size);

        private:

        /* TODO */
        class TBlock {
          NO_COPY(TBlock);
          public:

          /* TODO */
          TBlock *NextBlock;

        };  // TBlock

        /* TODO */
        const size_t BlockSize;

        /* TODO */
        void *Blob;

        /* TODO */
        TBlock *FirstBlock;

        /* TODO */
        std::mutex Mutex;

        /* TODO */
        const char *Name;

        /* TODO */
        std::atomic<size_t> NumBlocksUsed;

        /* TODO */
        size_t MaxBlocks;

      };  // TPool

      inline const char *TPool::GetName() const {
        assert(this);
        return Name;
      }

      inline size_t TPool::GetNumBlocksUsed() const {
        assert(this);
        return NumBlocksUsed.load();
      }

      inline size_t TPool::GetMaxBlocks() const {
        assert(this);
        return MaxBlocks;
      }

    }  // Util

  }  // Indy

}  // Orly
