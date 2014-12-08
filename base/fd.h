/* <base/fd.h>

   An RAII container for an OS file descriptor.

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


#include <unistd.h>
#include <sys/socket.h>

#include <algorithm>
#include <cassert>
#include <string>

#include <base/class_traits.h>
#include <util/error.h>

namespace Base {

  /* An RAII container for an OS file descriptor.

     This is a value type.  If you copy an instance of this class, it will use dup() to copy the file descriptor it contains (if any).

     This class implicitly casts to int, so you can use an instance of it in any call where you would normally pass a naked file descriptor.

     You can construct an instance of this class to capture the result of a function, such as the OS function socket(), which returns a newly created
     file descriptor.

     For example:

        TFd sock(HERE, socket(IF_INET, SOCK_STREAM, IPPROTO_TCP));

     If socket() fails (and so returns a negative value), the TFd constructor will throw an instance of std::system_error.

     You may also pass a naked file descriptor in the stdio range (0-2) to this constructor.  In this case, the newly constructed object will hold the
     file desciptor, but it will not attempt to close it. */
  class TFd {
    public:

    /* Default-construct as an illegal value (-1). */
    TFd()
        : OsHandle(-1) {}

    /* Move-construct, leaving the donor in the default-constructed state. */
    TFd(TFd &&that) {
      assert(&that);
      OsHandle = that.OsHandle;
      that.OsHandle = -1;
    }

    /* Copy-construct, duplicating the file descriptor with the OS call dup(), if necessary. */
    TFd(const TFd &that) {
      assert(&that);
      OsHandle = that.IsSystemFd() ? that.OsHandle : Util::IfLt0(dup(that.OsHandle));
    }

    /* Construct from a naked file descriptor, which the new instance will own.  Use this constructor to capture the result of an OS function, such as
       socket(), which returns a newly created file descriptor.  If the result is not a legal file descriptor, this function will throw the
       appropriate error.  */
    TFd(int os_handle) {
      OsHandle = Util::IfLt0(os_handle);
    }

    /* Close the file descriptor we own, if any.  If the descriptor is in the stdio range (0-2), then don't close it. */
    ~TFd() {
      assert(this);
      if (!IsSystemFd() && OsHandle != -1) {
        close(OsHandle);
      }
    }

    /* Swaperator. */
    TFd &operator=(TFd &&that) {
      assert(this);
      assert(&that);
      std::swap(OsHandle, that.OsHandle);
      return *this;
    }

    /* Assignment.  This will duplicate the file descriptor, if any, using the OS function dup(). */
    TFd &operator=(const TFd &that) {
      assert(this);
      return *this = TFd(that);
    }

    /* Assign from a naked file descriptor, which we will now own.  Use this constructor to capture the result of an OS function, such as socket(),
       which returns a newly created file descriptor.  If the result is not a legal file descriptor, this function will throw the appropriate
       error.  */
    TFd &operator=(int os_handle) {
      assert(this);
      return *this = TFd(os_handle);
    }

    /* Returns the naked file descriptor, which may be -1. */
    operator int() const {
      assert(this);
      return OsHandle;
    }

    /* True iff. this handle is open. */
    bool IsOpen() const {
      assert(this);
      return OsHandle >= 0;
    }

    /* True iff. the file descriptor can be read from without blocking.
       Waits for at most the given number of milliseconds for the descriptor to become readable.
       A negative timeout will wait forever. */
    bool IsReadable(int timeout = 0) const;

    /* True iff this is a system fd (stdin, stdout, stderr) */
    bool IsSystemFd() const {
      assert(this);
      return OsHandle < 3 && OsHandle >= 0;
    }

    /* Returns the naked file desciptor, which may be -1, and returns to the default-constructed state.  This is how to get the naked file desciptor
       away from the object without the object attempting to close it. */
    int Release() {
      assert(this);
      int result = OsHandle;
      OsHandle = -1;
      return result;
    }

    /* Return to the default-constructed state. */
    TFd &Reset() {
      assert(this);
      return *this = TFd();
    }

    /* Construct the read- and write-only ends of a pipe. */
    static void Pipe(TFd &readable, TFd &writeable) {
      assert(&readable);
      assert(&writeable);
      int fds[2];
      Util::IfLt0(pipe(fds) < 0);
      readable = TFd(fds[0], NoThrow);
      writeable = TFd(fds[1], NoThrow);
    }

    /* Construct both ends of a socket. */
    static void SocketPair(TFd &lhs, TFd &rhs, int domain, int type, int proto = 0) {
      assert(&lhs);
      assert(&rhs);
      int fds[2];
      Util::IfLt0(socketpair(domain, type, proto, fds));
      lhs = TFd(fds[0], NoThrow);
      rhs = TFd(fds[1], NoThrow);
    }

    private:

    /* Use to disambiguate construction for Pipe() and SocketPair(). */
    enum TNoThrow { NoThrow };

    /* Constuctor used by Pipe() and SocketPair(). */
    TFd(int os_handle, TNoThrow)
        : OsHandle(os_handle) {}

    /* The naked file descriptor we wrap.  This can be -1. */
    int OsHandle;

  };  // TFd

  /* Read everything on the fd.
     Puts it into a giant buffer in a string.
     NOTE: Don't do this with untrusted sources, for obvious reasons. */
  std::string ReadAll(TFd &&fd);

  /* Wrappers of stdin (0), stdout (1), and stderr (2). */
  extern const TFd In, Out, Err;

}  // Base
