/* <util/error.h>

   Error utilities.

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

#include <cstddef>
#include <system_error>

#include <base/code_location.h>

namespace Util {

/* Throw the given code as an error in the system category. */
template <typename TCode>
[[noreturn]] inline void ThrowSystemError(TCode code) {
  throw std::system_error(code, std::system_category());
}

/* If the given value is < 0, throw a system error based on errno.
   Use this function to test the results of system I/O calls. */
template <typename TRet>
TRet IfLt0(TRet &&ret) {
  if(ret < 0) {
    ThrowSystemError(errno);
  }
  return ret;
}

/* If the given value != 0, throw a system error based on the return value.
   Use this function to test the results of pthread calls. */
template <typename TRet>
TRet IfNe0(TRet &&ret) {
  if(ret != 0) {
    ThrowSystemError(ret);
  }
  return ret;
}

/* If the given value != 0, throw a system error based on the neagtion of the return value.
   Use this function to test the results of weird-assed library calls. */
template <typename TRet>
TRet IfWeird(TRet &&ret) {
  if(ret != 0) {
    ThrowSystemError(-ret);
  }
  return ret;
}

/* This is a thread safe wrapper that hides ugly platform-specific issues
   associated with strerror_r().  It will either copy an error message
   corresponding to 'errno_value' into the caller-supplied 'buf' and return a
   pointer to that buffer, or return a pointer to some statically allocated
   string constant.  The valid lifetime of the memory pointed to by the
   return value must be assumed to not exceed the lifetime of 'buf'. */
const char *Strerror(int errno_value, char *buf, size_t buf_size);

/* Aborts the program, sending an error message to stderr. */
[[noreturn]] void Abort(const Base::TCodeLocation &code_location);

}  // Util
