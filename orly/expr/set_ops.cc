/* <orly/expr/set_ops.cc>

   Implements <orly/expr/set_ops.h>.

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

#include <orly/expr/set_ops.h>

#include <orly/error.h>
#include <orly/expr/visitor.h>
#include <orly/pos_range.h>
#include <orly/type/set_ops_visitor.h>

using namespace Orly;
using namespace Orly::Expr;

class TSetOpsTypeVisitor
    : public Type::TSetOpsVisitor {
  public:

  TSetOpsTypeVisitor(Type::TType &type, const TPosRange &pos_range)
      : Type::TSetOpsVisitor(type, pos_range) {}

};  // TSetOpsTypeVisitor

TExpr::TPtr TIntersection::New(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range) {
  return TIntersection::TPtr(new TIntersection(lhs, rhs, pos_range));
}

TIntersection::TIntersection(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range)
    : TBinary(lhs, rhs, pos_range) {}

void TIntersection::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TIntersection::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  Type::TType::Accept(GetLhs()->GetType(), GetRhs()->GetType(), TSetOpsTypeVisitor(type, GetPosRange()));
  return type;
}

TExpr::TPtr TSymmetricDiff::New(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range) {
  return TSymmetricDiff::TPtr(new TSymmetricDiff(lhs, rhs, pos_range));
}

TSymmetricDiff::TSymmetricDiff(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range)
    : TBinary(lhs, rhs, pos_range) {}

void TSymmetricDiff::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TSymmetricDiff::GetTypeImpl() const {
  assert(this);
  Type::TType type;
  Type::TType::Accept(GetLhs()->GetType(), GetRhs()->GetType(), TSetOpsTypeVisitor(type, GetPosRange()));
  return type;
}

TExpr::TPtr TUnion::New(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range) {
  return TUnion::TPtr(new TUnion(lhs, rhs, pos_range));
}

TUnion::TUnion(const TExpr::TPtr &lhs, const TExpr::TPtr &rhs, const TPosRange &pos_range)
    : TBinary(lhs, rhs, pos_range) {}

void TUnion::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TUnion::GetTypeImpl () const {
  assert(this);
  Type::TType type;
  Type::TType::Accept(GetLhs()->GetType(), GetRhs()->GetType(), TSetOpsTypeVisitor(type, GetPosRange()));
  return type;
}
