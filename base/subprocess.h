/* <base/subprocess.h>

   Run a subprocess and communicate with it.

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
#include <memory>
#include <string>
#include <vector>

#include <base/pump.h>

namespace Base {

  /* Run a subprocess and communicate with it. */
  class TSubprocess final {
    public:

    static int WaitAll();
    // Returns the pid of the process which exited / work us up.

    /* Destroying this object won't stop the child process, we'll just stop talking to it. */
    ~TSubprocess() = default;

    /* The id of the child process. */
    int GetChildId() const {
      assert(this);
      return ChildId;
    }

    /* An fd from which to read the child's stderr output. */
    const TFd &GetStdErrFromChild() const {
      assert(this);
      return StdErrFromChild;
    }

    /* An fd from which to write the child's stdin input. */
    const TFd &GetStdInToChild() const {
      assert(this);
      return StdInToChild;
    }

    /* An fd from which to read the child's stdout output. */
    const TFd &GetStdOutFromChild() const {
      assert(this);
      return StdOutFromChild;
    }

    /* An fd from which to read the child's stderr output. */
    TFd TakeStdErrFromChild() const {
      assert(this);
      return std::move(StdErrFromChild);
    }

    /* An fd from which to write the child's stdin input. */
    TFd TakeStdInToChild() const {
      assert(this);
      return std::move(StdInToChild);
    }

    /* An fd from which to read the child's stdout output. */
    TFd TakeStdOutFromChild() {
      assert(this);
      return std::move(StdOutFromChild);
    }

    /* Wait for the child to complete and return its exit code. */
    int Wait() const;

    /* Execute the given command in a shell.  This will replace the calling process
       entirely with the shell process, so don't expect this function to return. */
    [[noreturn]] static void ExecStr(const char *cmd);

    /* Execute the given command in a shell.  This will replace the calling process
       entirely with the target command, so don't expect this function to return. */
    [[noreturn]] static void Exec(const std::vector<std::string> &cmd);

    /* Fork a new child process.  If we are the parent, return a pointer to a newly
       allocated TSubprocess instance.  If we are the child, return null. */
    static std::unique_ptr<TSubprocess> New(TPump &pump) {
      // http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-active.html#2070
      auto result = std::unique_ptr<TSubprocess>(new TSubprocess(pump));
      if (!result->ChildId) {
        result.reset();
      }
      return result;
    }

    /* Fork a new child process.  If we are the parent, return a pointer to a newly
       allocated TSubprocess instance.  If we are the child, shell out to execute the
       command and don't return at all. */
    static std::unique_ptr<TSubprocess> NewStr(TPump &pump, const char *cmd) {
      auto subprocess = New(pump);
      if (!subprocess) {
        ExecStr(cmd);
      }
      return subprocess;
    }

    /* Fork a new child process.  If we are the parent, return a pointer to a newly
       allocated TSubprocess instance.  If we are the child, shell out to execute the
       command and don't return at all. */
    static std::unique_ptr<TSubprocess> New(TPump &pump, const std::vector<std::string> &cmd) {
      auto subprocess = New(pump);
      if (!subprocess) {
        Exec(cmd);
      }
      return subprocess;
    }

    private:

    /* Construct the pipes and fork. */
    TSubprocess(TPump &pump);

    /* See accessors. */
    TFd StdInToChild, StdOutFromChild, StdErrFromChild;

    /* See accessor. */
    int ChildId;

  };  // TSubprocess


  /* TODO: Move this to a better place */
  void EchoOutput(Base::TFd &&fd);

}  // Base
