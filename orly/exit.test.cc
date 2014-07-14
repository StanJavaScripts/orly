/* <orly/exit.test.cc>

   Unit test for whether or not TService quits on destruction, which it should.

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

#include <orly/spa/service.h>

#include <orly/spa/honcho.h>

#include <test/kit.h>

using namespace std;
using namespace Orly;
using namespace Orly::Spa;

FIXTURE(Startup) {
  THoncho honcho;
  TService service;
  /* Parse command-line arguments */
  service.SetPackageDir("/tmp/");
  /* Set checkpoint path and package directory */
  Base::TUuid session;
  service.CreateSession(Base::TOpt<Base::TUuid>(Base::TUuid()), 1000, session);
  Base::TUuid private_pov;
  service.CreatePrivatePov(session, {}, 1000, false, private_pov);
}