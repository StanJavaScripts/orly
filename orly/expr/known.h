/* <orly/expr/known.h>

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
#include <orly/expr/binary.h>
#include <orly/expr/unary.h>

namespace Orly {

  namespace Expr {

    /* is known test */
    class TIsKnown
        : public TUnary {
      NO_COPY(TIsKnown);
      public:

      typedef std::shared_ptr<TIsKnown> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &expr, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TIsKnown(const TExpr::TPtr &expr, const TPosRange &pos_range);

    };  // TIsKnown

    /* is unknown test */
    class TIsUnknown
        : public TUnary {
      NO_COPY(TIsUnknown);
      public:

      typedef std::shared_ptr<TIsUnknown> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &expr, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TIsUnknown(const TExpr::TPtr &expr, const TPosRange &pos_range);

    };  // TIsUnknown

    /* is known expr test */
    class TIsKnownExpr
        : public TBinary {
      NO_COPY(TIsKnownExpr);
      public:

      typedef std::shared_ptr<TIsKnownExpr> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TIsKnownExpr(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range);

    };  // TIsKnownExpr

    /* Prefix 'known' to unwrap an optional */
    class TKnown
        : public TUnary {
      NO_COPY(TKnown);
      public:

      typedef std::shared_ptr<TKnown> TPtr;

      static TExpr::TPtr New(const TExpr::TPtr &expr, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      virtual Type::TType GetTypeImpl() const override;

      private:

      TKnown(const TExpr::TPtr &expr, const TPosRange &pos_range);

    };  // TKnown

  }  // Expr

}  // Orly
