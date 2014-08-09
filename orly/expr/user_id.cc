/* <orly/expr/user_id.cc>

   Implements <orly/expr/user_id.h>

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

#include <orly/expr/user_id.h>

#include <orly/expr/visitor.h>
#include <orly/pos_range.h>
#include <orly/type/id.h>
#include <orly/type/opt.h>

using namespace Orly;
using namespace Orly::Expr;

TUserId::TPtr TUserId::New(const TPosRange &pos_range) {
  return TUserId::TPtr(new TUserId(pos_range));
}

TUserId::TUserId(const TPosRange &pos_range)
    : TLeaf(pos_range) {}

void TUserId::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

Type::TType TUserId::GetTypeImpl() const {
  assert(this);
  return Type::TOpt::Get(Type::TId::Get());
}
