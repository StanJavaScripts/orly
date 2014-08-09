/* <orly/expr/slice.h>

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

#include <array>
#include <memory>

#include <base/assert_true.h>
#include <base/class_traits.h>
#include <orly/expr/n_ary.h>
#include <orly/expr/visitor.h>

namespace Orly {

  namespace Expr {

    class TSlice
        : public TNAry<std::array<TExpr::TPtr, 3U>> {
      NO_COPY(TSlice);
      public:

      typedef std::shared_ptr<TSlice> TPtr;

      typedef std::array<TExpr::TPtr, 3U> TExprArray;

      enum class TEndState { Closed, Open };

      static TPtr New(
          const TExpr::TPtr &container,
          const TExpr::TPtr &opt_lhs,
          bool colon,
          const TExpr::TPtr &opt_rhs,
          const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      const TExprArray &GetExprs() const {
        assert(this);
        return TNAry::GetContainer();
      }

      /* Return the pointer to the container */
      const TExpr::TPtr &GetContainer() const;

      /* Return the pointer to an optional lhs. This means the pointer can be null */
      const TExpr::TPtr &GetOptLhs() const;

      /* Return the pointer to an optional rhs. This means the pointer can be null */
      const TExpr::TPtr &GetOptRhs() const;

      /* Returns the state of the end. This is either open or closed
         NOTE. if end is null, this gets set to true */
      bool HasColon() const;

      private:

      TSlice(
          const TExpr::TPtr &container,
          const TExpr::TPtr &opt_lhs,
          bool colon,
          const TExpr::TPtr &opt_rhs,
          const TPosRange &pos_range);

      bool Colon;

    };  // TSlice

  }  // Expr

}  // Orly
