/* <orly/expr/reduce.cc>

   Implements <orly/expr/reduce.h>.

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

#include <orly/expr/reduce.h>

#include <orly/error.h>
#include <orly/expr/visitor.h>
#include <orly/pos_range.h>
#include <orly/type/seq.h>
#include <orly/type/unwrap.h>
#include <orly/type/util.h>

using namespace Orly;
using namespace Orly::Expr;

TReduce::TPtr TReduce::New(
    const TExpr::TPtr &lhs,
    const TPosRange &pos_range) {
  return TReduce::TPtr(new TReduce(lhs, pos_range));
}

TReduce::TReduce(
    const TExpr::TPtr &lhs,
    const TPosRange &pos_range)
      : TThatableBinary(lhs, pos_range) {}

void TReduce::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TReduce::GetTypeImpl() const {
  assert(this);
  if (!GetLhs()->GetType().Is<Type::TSeq>()) {
    throw TExprError(HERE, GetPosRange(), "The lhs of reduce expression must result in a sequence");
  }
  if (!Start) {
    throw TExprError(HERE, GetPosRange(), "Missing a 'start' expression in reduce");
  }
  assert(Start);
  assert(GetRhs());
  Type::TType type = Type::Unwrap(GetRhs()->GetType());
  if (Type::Unwrap(Start->GetType()) != Type::Unwrap(GetRhs()->GetType())) {
    throw TExprError(HERE, GetPosRange(),
        "The rhs of reduce expression must result in the same type as its starting expression");
  }
  return type;
}
