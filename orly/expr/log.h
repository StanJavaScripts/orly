/* <orly/expr/log.h>

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

#include <base/class_traits.h>
#include <orly/expr/unary.h>

namespace Orly {

  namespace Expr {

    class TLog
        : public TUnary {
      NO_COPY(TLog);
      public:

      typedef std::shared_ptr<TLog> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &expr, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TLog(const TExpr::TPtr &expr, const TPosRange &pos_range);

    };  // TLog

    class TLog2
        : public TUnary {
      NO_COPY(TLog2);
      public:

      typedef std::shared_ptr<TLog2> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &expr, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TLog2(const TExpr::TPtr &expr, const TPosRange &pos_range);

    };  // TLog2

    class TLog10
        : public TUnary {
      NO_COPY(TLog10);
      public:

      typedef std::shared_ptr<TLog10> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &expr, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TLog10(const TExpr::TPtr &expr, const TPosRange &pos_range);

    };  // TLog10

  }  // Expr

}  // Orly
