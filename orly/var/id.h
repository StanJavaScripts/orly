/* <orly/var/id.h>

   A Orly id, which is Base::TUuid.

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

#include <base/as_str.h>
#include <orly/rt/runtime_error.h>
#include <orly/var/impl.h>

namespace Orly {

  namespace Var {

    class TId
        : public TVar::TImpl {
      public:

      /* TODO */
      virtual Var::TVar &Index(const TVar &);

      /* TODO */
      virtual TId &Add(const TVar &);

      /* TODO */
      virtual TId &And(const TVar &);

      /* TODO */
      virtual TId &Div(const TVar &);

      /* TODO */
      virtual TId &Exp(const TVar &);

      /* TODO */
      virtual TId &Intersection(const TVar &);

      /* TODO */
      virtual TId &Mod(const TVar &);

      /* TODO */
      virtual TId &Mult(const TVar &);

      /* TODO */
      virtual TId &Or(const TVar &);

      /* TODO */
      virtual TId &Sub(const TVar &);

      /* TODO */
      virtual TId &SymmetricDiff(const TVar &);

      /* TODO */
      virtual TId &Union(const TVar &);

      /* TODO */
      virtual TId &Xor(const TVar &);

      /* TODO */
      Base::TUuid GetVal() const {
        assert(this);
        return Val;
      }

      /* TODO */
      virtual size_t GetHash() const;

      /* TODO */
      virtual Type::TType GetType() const;

      /* TODO */
      virtual void Touch();

      /* TODO */
      virtual void Write(std::ostream &) const;

      /* TODO */
      static TVar New(const Base::TUuid &that);

      private:

      /* TODO */
      TId(const Base::TUuid &that);

      /* TODO */
      virtual ~TId();

      /* TODO */
      virtual void Accept(const TVisitor &visitor) const;

      /* TODO */
      virtual TVar Copy() const;

      /* TODO */
      Base::TUuid Val;

    };  // TId

    /* TODO */
    template <>
    struct TVar::TDt<Base::TUuid> {

      /* TODO */
      Base::TUuid static As(const TVar &that) {
        TId *ptr = dynamic_cast<TId *>(that.Impl.get());
        if (ptr) {
          return ptr->GetVal();
        }
        throw Rt::TSystemError(
            HERE, Base::AsStr("Trying to dynamic cast Var to id. \"", that.GetType(), "\" is not an id.").c_str());
      }

    };  // TDt<Base::TUuid>

  }  // Var

}  // Orly
