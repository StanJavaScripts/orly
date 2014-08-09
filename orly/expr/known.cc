/* <orly/expr/known.cc>

   Implements <orly/expr/known.h>

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

#include <orly/expr/known.h>

#include <orly/error.h>
#include <orly/expr/visitor.h>
#include <orly/pos_range.h>
#include <orly/type/comp_visitor.h>
#include <orly/type/impl.h>
#include <orly/type/unwrap.h>
#include <orly/type/unwrap_visitor.h>

using namespace Orly;
using namespace Orly::Expr;

class TIsKnownUnknownTypeVisitor
    : public Type::TUnwrapVisitor {
  NO_COPY(TIsKnownUnknownTypeVisitor);
  public:
  TIsKnownUnknownTypeVisitor(Type::TType &type, const TPosRange &pos_range)
      : Type::TUnwrapVisitor(type, pos_range) {}
  virtual void operator()(const Type::TAddr     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TBool     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TDict     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TId       *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TInt      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TList     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TObj      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TOpt      *that) const {
    class TVisitor
        : public Type::TUnwrapVisitor {
      NO_COPY(TVisitor);
      public:
      TVisitor(Type::TType &type, const TPosRange &pos_range)
          : Type::TUnwrapVisitor(type, pos_range) {}
      virtual void operator()(const Type::TAddr     *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TBool     *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TDict     *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TId       *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TInt      *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TList     *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TObj      *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TOpt      *) const {
        throw TExprError(HERE, PosRange, "Cannot have nested optionals");
      }
      virtual void operator()(const Type::TReal     *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TSet      *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TSeq      *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TStr      *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TTimeDiff *) const { Type = Type::TBool::Get(); }
      virtual void operator()(const Type::TTimePnt  *) const { Type = Type::TBool::Get(); }
    };  // TVisitor
    that->GetElem().Accept(TVisitor(Type, PosRange));
  }
  virtual void operator()(const Type::TReal     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TSet      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TStr      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TTimeDiff *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TTimePnt  *) const { throw TExprError(HERE, PosRange); }
};  // TIsKnownUnknownTypeVisitor

TExpr::TPtr TIsKnown::New(const TExpr::TPtr &expr, const TPosRange &pos_range) {
  return TIsKnown::TPtr(new TIsKnown(expr, pos_range));
}

TIsKnown::TIsKnown(const TExpr::TPtr &expr, const TPosRange &pos_range)
    : TUnary(expr, pos_range) {}

void TIsKnown::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TIsKnown::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  GetExpr()->GetType().Accept(TIsKnownUnknownTypeVisitor(type, GetPosRange()));
  return type;
}

TExpr::TPtr TIsUnknown::New(const TExpr::TPtr &expr, const TPosRange &pos_range) {
  return TIsUnknown::TPtr(new TIsUnknown(expr, pos_range));
}

TIsUnknown::TIsUnknown(const TExpr::TPtr &expr, const TPosRange &pos_range)
    : TUnary(expr, pos_range) {}

void TIsUnknown::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TIsUnknown::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  GetExpr()->GetType().Accept(TIsKnownUnknownTypeVisitor(type, GetPosRange()));
  return type;
}

TExpr::TPtr TIsKnownExpr::New(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range) {
  return TIsKnownExpr::TPtr(new TIsKnownExpr(lhs, rhs, pos_range));
}

TIsKnownExpr::TIsKnownExpr(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range)
    : TBinary(lhs, rhs, pos_range) {}

void TIsKnownExpr::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TIsKnownExpr::GetTypeImpl() const {
  class TIsKnownExprTypeVisitor
      : public Type::TUnwrapVisitor {
    NO_COPY(TIsKnownExprTypeVisitor);
    public:
    TIsKnownExprTypeVisitor(Type::TType &type, const Type::TType &expr_type, const TPosRange &pos_range)
        : Type::TUnwrapVisitor(type, pos_range), ExprType(expr_type) {}
    virtual void operator()(const Type::TAddr     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TBool     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TDict     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TId       *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TInt      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TList     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TObj      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TOpt      *that) const {
      Type::TType::Accept(that->GetElem(), ExprType, Type::TEqCompVisitor(Type, PosRange));
    }
    virtual void operator()(const Type::TReal     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TSet      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TStr      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TTimeDiff *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TTimePnt  *) const { throw TExprError(HERE, PosRange); }
    private:
    const Type::TType &ExprType;
  };  // TIsKnownExprTypeVisitor
  assert(this);
  Type::TType type;
  GetLhs()->GetType().Accept(TIsKnownExprTypeVisitor(type, GetRhs()->GetType(), GetPosRange()));
  return type;
}

TExpr::TPtr TKnown::New(const TExpr::TPtr &expr, const TPosRange &pos_range) {
  return TKnown::TPtr(new TKnown(expr, pos_range));
}

TKnown::TKnown(const TExpr::TPtr &expr, const TPosRange &pos_range)
    : TUnary(expr, pos_range) {}

void TKnown::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TKnown::GetTypeImpl() const {
  class TKnownTypeVisitor
      : public Type::TUnwrapVisitor {
    NO_COPY(TKnownTypeVisitor);
    public:
    TKnownTypeVisitor(Type::TType &type, const TPosRange &pos_range)
        : Type::TUnwrapVisitor(type, pos_range) {}
    virtual void operator()(const Type::TAddr     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TBool     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TDict     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TId       *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TInt      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TList     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TObj      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TOpt      *that) const {
      Type = that->GetElem();
    }
    virtual void operator()(const Type::TReal     *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TSet      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TStr      *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TTimeDiff *) const { throw TExprError(HERE, PosRange); }
    virtual void operator()(const Type::TTimePnt  *) const { throw TExprError(HERE, PosRange); }
  };  // TKnownTypeVisitor
  assert(this);
  Type::TType type;
  GetExpr()->GetType().Accept(TKnownTypeVisitor(type, GetPosRange()));
  return type;
}
