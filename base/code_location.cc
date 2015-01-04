/* <base/code_location.cc>

   Implements <base/code_location.h>.

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

#include <base/code_location.h>

#include <cstring>

using namespace std;
using namespace Base;

// NOTE: Calculating this once and caching it is perf critical.
static size_t GetSrcRootLen() {
  static size_t len = strlen(TCodeLocation::SrcRoot);
  return len;
}

const char *TCodeLocation::GetFile() const {
  assert(this);
  return File + ((strncmp(File, SrcRoot, GetSrcRootLen()) == 0) ? (GetSrcRootLen()) : 0);
}

void TCodeLocation::Write(ostream &strm) const {
  assert(this);
  assert(&strm);
  strm << '[' << GetFile() << ", " << LineNumber << ']';
}

const char *TCodeLocation::SrcRoot = SRC_ROOT;
