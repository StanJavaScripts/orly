/* <jhm/jobs/link.h>

   Job which calculates / generates a C dependency file

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

    class TLink final : public TJob {
      public:

      static TJobProducer GetProducer();

      virtual const char *GetName() final;
      virtual const TSet<TFile*> GetNeeds() final;
      virtual TSet<TFile*> GetAntiNeeds() final;
      virtual std::vector<std::string> GetCmd() final;
      virtual Util::TTimestamp GetCmdTimestamp() const final;
      virtual bool IsComplete() final;

      private:
      TLink(TEnv &env, TFile *input);

      TEnv &Env;
      std::unordered_map<TFile*, TFile*> NeededDepToObj;
      TSet<TFile*> AntiNeeds;
      TSet<TFile*> ObjToCheck;
      TSet<TFile*> ObjFiles;
    };

  }
}