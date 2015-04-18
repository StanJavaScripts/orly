/* <jhm/jobs/compile_c_family.h>

   Job which compiles a C family file (.c, .cc) to a .o file

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

#include <jhm/job.h>

namespace Jhm {

namespace Job {

class TCompileCFamily final : public TJob {
  public:
  static std::vector<std::string> GetStandardArgs(TFile *input, bool is_cpp, const TEnv &env);

  static TJobProducer GetCProducer();
  static TJobProducer GetCppProducer();

  virtual const char *GetName() final;
  virtual const TSet<TFile *> GetNeeds() final;
  virtual std::vector<std::string> GetCmd() final;
  virtual Util::TTimestamp GetCmdTimestamp() const final;
  virtual bool IsComplete() final;

  private:
  TCompileCFamily(TEnv &env, TFile *input, bool is_cpp);

  TEnv &Env;
  // NOTE: We could make IsCpp be constant / CompileCFamily be templated on it
  // But that results in more ug than the tiny perf benefit is worth.
  bool IsCpp;
  TFile *Need;
};

}  // Job

}  // Jhm
