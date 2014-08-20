/* <orly/balancer/failover_test_balancer.h>

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

#include <base/event_semaphore.h>
#include <orly/balancer/balancer.h>

namespace Orly {

  namespace Balancer {

    /* TODO */
    class TFailoverTestBalancer
        : public TBalancer {
      NO_COPY(TFailoverTestBalancer);
      public:

      /* TODO */
      TFailoverTestBalancer(Base::TScheduler *scheduler,
                            const TBalancer::TCmd &cmd,
                            std::chrono::milliseconds interval);

      /* TODO */
      virtual ~TFailoverTestBalancer();

      /* TODO */
      virtual const Socket::TAddress &ChooseHost();

      /* TODO */
      void RegisterHost(const Socket::TAddress &addr);

      private:

      /* TODO */
      void CheckHosts();

      /* TODO */
      bool CheckHost(const Socket::TAddress &addr) const;

      /* TODO */
      virtual void OnError(const std::exception &ex);

      /* TODO */
      std::unordered_set<Socket::TAddress> HostSet;
      Base::TOpt<Socket::TAddress> MasterHost;
      std::mutex HostMutex;
      std::condition_variable HostCond;

      /* TODO */
      std::chrono::milliseconds Interval;

      /* TODO */
      bool Running;

      /* TODO */
      Base::TEventSemaphore ErrorSem;

    };  // TFailoverTestBalancer

  }  // Balancer

}  // Orly