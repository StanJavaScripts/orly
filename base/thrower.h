/* <base/thrower.h>

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

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <base/class_traits.h>
#include <base/code_location.h>
#include <base/demangle.h>
#include <base/exception.h>

/* Use this macro to throw an error, like this: THROW_ERROR(TSomethingBad) << "more info" <<
 * Base::EndOfPart << "yet more info"; */
#define THROW_ERROR(error_t) (::Base::TThrower<error_t>(HERE_STRING))

namespace Base {

template <typename TError>
std::is_same<decltype(TError::GetDesc()), const char *> HasGetDescImpl(void *);

template <typename>
std::false_type HasGetDescImpl(...);

template <typename TError>
static constexpr bool HasGetDesc() {
  return decltype(HasGetDescImpl<TError>(nullptr))::value;
}

/* Helper template so that we can prefix methods with a GetDesc() function if the error has one. Not
 * if it doesn't. */
template <typename TError>
std::enable_if_t<HasGetDesc<TError>(), const char *> GetErrorDescHelper() {
  return TError::GetDesc();
}

template <typename TError>
std::enable_if_t<!HasGetDesc<TError>(), const char *> GetErrorDescHelper() {
  return nullptr;
}

/* Construct a temporary instance of this object, passing in the current code location.
   While the object exists, you may stream additional error information onto it.
   When the object goes out of scope, it will throw the error. */
template <typename TError>
class TThrower final {
  public:
  MOVE_ONLY(TThrower);

  /* Begin the error message with the code location and the description provided by TError (if any).
   */
  TThrower(const char *code_location) : AtEndOfPart(true) {
    Strm << code_location;
    const char *desc = GetErrorDescHelper<TError>();
    if(desc) {
      Strm << "; " << desc;
    } else if(!HasGetDesc<TError>()) {
      // If the GetDesc() doesn't exist, provide the typename for the user (It's probably an STL
      // exception).
      Strm << "; " << Demangle<TError>();
    }
  }

  /* Boom goes the dynamite. */
  [[noreturn]] ~TThrower() noexcept(false) {
    assert(this);
    throw TError(Strm.str().c_str());
  }

  /* Append the value to the error message we are building.
     If we're currently positioned at the end of a message part, insert a delimiter before the
     value. */
  template <typename TVal>
  void Write(const TVal &val) {
    assert(this);
    if(AtEndOfPart) {
      Strm << "; ";
      AtEndOfPart = false;
    }
    Strm << val;
  }

  template<typename TVal>
  TThrower<TError> &operator<<(const TVal &val) {
    Write(val);
    return *this;
  }

  private:
  bool AtEndOfPart;

  /* Collects the error message as we build it. */
  std::ostringstream Strm;

};  // TThrower<TError>

}  // Base
