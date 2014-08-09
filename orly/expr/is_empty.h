/* <orly/expr/is_empty.h>

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

    class TIsEmpty
        : public TUnary {
      NO_COPY(TIsEmpty);
      public:

      typedef std::shared_ptr<TIsEmpty> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &expr, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TIsEmpty(const TExpr::TPtr &expr, const TPosRange &pos_range);

    };  // TIsEmpty

  }  // Expr

}  // Orly
