/* <orly/var/str.cc>

   Implements <orly/var/str.h>.

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

#include <orly/var/str.h>

#include <iomanip>

#include <orly/type/str.h>

using namespace Orly;
using namespace Var;

size_t TStr::GetHash() const {
  assert(this);
  return std::hash<std::string>()(Val);
}

Type::TType TStr::GetType() const {
  assert(this);
  return Type::TStr::Get();
}

void TStr::Write(std::ostream &strm) const {
  assert(this);
  strm << "std::string(" << std::quoted(Val) << ')';
}

void TStr::Accept(const TVisitor &visitor) const {
  assert(this);
  assert(&visitor);
  visitor(this);
}

void TStr::Touch() {
  assert(this);
}

Var::TVar &TStr::Index(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Index not supported on str.");
}

TStr &TStr::Add(const TVar &rhs) {
  assert(this);
  Val += Var::TVar::TDt<std::string>::As(rhs);
  return *this;
}

TStr &TStr::And(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "And not supported on str.");
}

TStr &TStr::Div(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Div not supported on str.");
}

TStr &TStr::Exp(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Exp not supported on str.");
}

TStr &TStr::Intersection(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Intersection not supported on str.");
}

TStr &TStr::Mod(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Mod not supported on str.");
}

TStr &TStr::Mult(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Mul not supported on str.");
}

TStr &TStr::Or(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Or not supported on str.");
}

TStr &TStr::Sub(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Sub not supported on str.");
}

TStr &TStr::SymmetricDiff(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "SymmetricDiff Xor not supported on str.");
}

TStr &TStr::Union(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Union Or not supported on str.");
}

TStr &TStr::Xor(const TVar &) {
  assert(this);
  throw Rt::TSystemError(HERE, "Xor not supported on str.");
}

TStr::TStr(const std::string &that) : Val(that) {}

TStr::~TStr() {}

TVar TStr::Copy() const {
  assert(this);
  return TStr::New(Val);
}

TVar TStr::New(const std::string &that) {
  return (new TStr(that))->AsVar();
}

TVar::TVar(const std::string &that) {
  *this = TStr::New(that);
}
