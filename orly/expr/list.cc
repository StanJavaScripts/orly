/* <orly/expr/list.cc>

   Implements <orly/expr/list.h>

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

#include <orly/expr/list.h>

#include <orly/error.h>
#include <orly/expr/visitor.h>
#include <orly/pos_range.h>
#include <orly/synth/context.h>
#include <orly/type/list.h>
#include <orly/type/seq.h>
#include <orly/type/unwrap.h>
#include <orly/type/util.h>

using namespace Orly;
using namespace Orly::Expr;

TList::TPtr TList::New(const Type::TType &type, const TPosRange &pos_range) {
  return TList::TPtr(new TList(type, pos_range));
}

TList::TPtr TList::New(const TExprVec &exprs, const TPosRange &pos_range) {
  return TList::TPtr(new TList(exprs, pos_range));
}

/* Empty contructor */
TList::TList(const Type::TType &type, const TPosRange &pos_range)
    : TCtor(pos_range), Type(type) {}

/* Non-empty constructor */
TList::TList(const TExprVec &exprs, const TPosRange &pos_range)
    : TCtor(exprs, pos_range) {
  assert(!GetExprs().empty());
}

void TList::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TList::GetTypeImpl() const {
  assert(this);
  if (!Type) {
    Type::TType elem = Type::Unwrap(GetExprs().front()->GetType());
    bool is_sequence = false;
    for (auto expr : GetExprs()) {
      if (Type::Unwrap(expr->GetType()) != elem) {
        Synth::GetContext().AddError(expr->GetPosRange(), "A list constructor's elements must be of equal type.");
      }
      is_sequence |= expr->GetType().Is<Type::TSeq>();
    }
    Type = Type::TList::Get(elem);
    if (is_sequence) {
      Type = Type::TSeq::Get(Type);
    }
    assert(Type);
  }
  return Type;
}
