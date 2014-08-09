/* <orly/expr/expr.cc>

   Implements <orly/expr/expr.h>

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

#include <orly/expr/expr.h>

#include <orly/type/util.h>

using namespace Orly;
using namespace Orly::Expr;

TExpr::TExpr(const TPosRange &pos_range)
    : ExprParent(nullptr), PosRange(pos_range) {}

const TExprParent *TExpr::GetExprParent() const {
  assert(this);
  assert(ExprParent);
  return ExprParent;
}

Type::TType TExpr::GetType() const {
  if (CachedType) {
    return *CachedType;
  }

  auto type = GetTypeImpl();
  if (!type.Is<Type::TAny>()) {
    assert (!CachedType || *CachedType == type);
    CachedType = type;
  }
  return type;
}

void TExpr::SetExprParent(const TExprParent *expr_parent) {
  assert(this);
  assert(expr_parent);
  assert(!ExprParent);
  ExprParent = expr_parent;
}

const TPosRange &TExpr::GetPosRange() const {
  assert(this);
  return PosRange;
}

void TExpr::UnsetExprParent(const TExprParent *expr_parent) {
  assert(this);
  assert(expr_parent);
  assert(ExprParent == expr_parent);
  ExprParent = nullptr;
}
