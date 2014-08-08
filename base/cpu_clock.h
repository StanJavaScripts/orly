/* <base/cpu_clock.h>

   Meets the C++ requirements of a TrivialClock (http://en.cppreference.com/w/cpp/concept/TrivialClock)

   Wraps CLOCK_THREAD_CPUTIME_ID, which is linux-specific

   NOTE: We use STL naming conventions here because we want it to look like the STL provided ones.

   TODO: This is something we'd like in the standard library (Or at least a system lib of some sort)

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

#include <chrono>

namespace Base {

  struct cpu_clock {
    using duration = std::chrono::nanoseconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<cpu_clock, duration>;

    static constexpr bool is_steady = false;
    static time_point now() noexcept;
  };
}