/* <orly/expr/string.cc>

   Implements <orly/expr/string.h>

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

#include <orly/expr/string.h>

#include <orly/error.h>
#include <orly/expr/visitor.h>
#include <orly/pos_range.h>
#include <orly/type/impl.h>
#include <orly/type/unwrap.h>
#include <orly/type/unwrap_visitor.h>

using namespace Orly;
using namespace Orly::Expr;

class TStringTypeVisitor
    : public Type::TUnwrapVisitor {
  NO_COPY(TStringTypeVisitor);
  public:
  TStringTypeVisitor(Type::TType &type, const TPosRange &pos_range)
    : Type::TUnwrapVisitor(type, pos_range) {}

  virtual void operator()(const Type::TAddr     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TBool     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TDict     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TId       *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TInt      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TList     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TObj      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TReal     *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TSet      *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TStr      *) const { Type = Type::TStr::Get(); }
  virtual void operator()(const Type::TTimeDiff *) const { throw TExprError(HERE, PosRange); }
  virtual void operator()(const Type::TTimePnt  *) const { throw TExprError(HERE, PosRange); }
};  // TStringTypeVisitor

TExpr::TPtr TToLower::New(const TExpr::TPtr &expr, const TPosRange &pos_range) {
  return TToLower::TPtr(new TToLower(expr, pos_range));
}

TToLower::TToLower(const TExpr::TPtr &expr, const TPosRange &pos_range)
    : TUnary(expr, pos_range) {}

void TToLower::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TToLower::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  GetExpr()->GetType().Accept(TStringTypeVisitor(type, GetPosRange()));
  return type;
}

TExpr::TPtr TToUpper::New(const TExpr::TPtr &expr, const TPosRange &pos_range) {
  return TToUpper::TPtr(new TToUpper(expr, pos_range));
}

TToUpper::TToUpper(const TExpr::TPtr &expr, const TPosRange &pos_range)
    : TUnary(expr, pos_range) {}

void TToUpper::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TToUpper::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  GetExpr()->GetType().Accept(TStringTypeVisitor(type, GetPosRange()));
  return type;
}