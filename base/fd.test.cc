/* <base/fd.test.cc>

   Unit test for <base/fd.h>.

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

#include <base/fd.h>

#include <unistd.h>

#include <cstring>

#include <base/zero.h>
#include <util/io.h>

#include <test/kit.h>

using namespace std;
using namespace Base;
using namespace Util;

static const char *ExpectedData = "hello";
static const size_t ExpectedSize = strlen(ExpectedData);

static const size_t MaxActualSize = 1024;
static char ActualData[MaxActualSize];

static void Transact(const TFd &readable, const TFd &writeable) {
  WriteExactly(writeable, ExpectedData, ExpectedSize);
  Zero(ActualData);
  size_t actual_size = ReadAtMost(readable, ActualData, MaxActualSize);
  if (EXPECT_EQ(actual_size, ExpectedSize)) {
    EXPECT_FALSE(strcmp(ActualData, ExpectedData));
  }
}

static void CheckClose(const TFd &readable, TFd &writeable) {
  writeable.Reset();
  size_t actual_size = ReadAtMost(readable, ActualData, MaxActualSize);
  EXPECT_FALSE(actual_size);
}

FIXTURE(Pipe) {
  TFd readable, writeable;
  TFd::Pipe(readable, writeable);
  Transact(readable, writeable);
  CheckClose(readable, writeable);
}

// Handling of stdin, stdout, stderr
FIXTURE(SystemFd) {
  {
    TFd fd(STDIN_FILENO);
    // We consider it a system fd
    EXPECT_TRUE(fd.IsSystemFd());

    //We don't dup() system fds.
    TFd fd2(fd);
    EXPECT_EQ(int(fd2), STDIN_FILENO);
  }

  // Checks that we didn't close the fd
  EXPECT_TRUE(IsValidFd(STDIN_FILENO));

  // Repeat the checks for STDOUT, STDERR
  {
    TFd fd(STDOUT_FILENO);
    EXPECT_TRUE(fd.IsSystemFd());
    TFd fd2(fd);
    EXPECT_EQ(int(fd2), STDOUT_FILENO);
  }
  EXPECT_TRUE(IsValidFd(STDOUT_FILENO));

  {
    TFd fd(STDERR_FILENO);
    EXPECT_TRUE(fd.IsSystemFd());
    TFd fd2(fd);
    EXPECT_EQ(int(fd2), STDERR_FILENO);
  }
  EXPECT_TRUE(IsValidFd(STDERR_FILENO));

}