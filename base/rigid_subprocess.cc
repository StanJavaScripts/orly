/* <base/rigid_subprocess.cc>

   Implements <base/rigid_subprocess.h>.

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

#include <base/rigid_subprocess.h>

#include <unistd.h>
#include <sys/wait.h>

#include <base/subprocess.h>
#include <util/error.h>
#include <util/io.h>

using namespace std;
using namespace Base;
using namespace Util;

TRigidSubprocess::TRigidSubprocess(const char *cmd) : TRigidSubprocess(vector<string>{"/bin/sh", "/bin/sh", "-c", cmd}) {}

TRigidSubprocess::TRigidSubprocess(vector<string> cmd) {
  /* Save copies of the current in/out/err. */
  TFd saved_in(dup(0)), saved_out(dup(1)), saved_err(dup(2));
  /* Make new in/out/err pipes for the child to inherit, then fork. */
  TFd child_id, child_out, child_err;
  TFd::Pipe(child_id, StdInToChild);
  TFd::Pipe(StdOutFromChild, child_out);
  TFd::Pipe(StdErrFromChild, child_err);
  /* Make sure the child only inherits the fds it's meant to inherit. */
  for (auto *fd : {&saved_in, &saved_out, &saved_err, &StdInToChild, &StdErrFromChild, &StdOutFromChild}) {
    SetCloseOnExec(*fd);
  }
  /* Fork the dangerous way. */
  ChildId = IfLt0(fork());
  if (ChildId) {
    /* We're the parent.  The child has already launched
       (because fork() doesn't return until the child calls exec())
       so we can put the saved in/our/err fds back into place. */
    IfLt0(dup2(saved_in, 0));
    IfLt0(dup2(saved_out, 1));
    IfLt0(dup2(saved_err, 2));
  } else {
    /* We're the child.  Dupe the pipes into their conventional positions. */
    IfLt0(dup2(child_id, 0));
    IfLt0(dup2(child_out, 1));
    IfLt0(dup2(child_err, 2));
    /* Execute the command. */
    TSubprocess::Exec(cmd);
  }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

int TRigidSubprocess::Wait() const {
  assert(this);
  int status;
  IfLt0(waitpid(ChildId, &status, 0));
  return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

#pragma GCC diagnostic pop
