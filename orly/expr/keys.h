/* <orly/expr/keys.h>

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

#include <memory>
#include <vector>

#include <base/class_traits.h>
#include <orly/shared_enum.h>
#include <orly/expr/addr.h>
#include <orly/expr/n_ary.h>

namespace Orly {

  namespace Expr {

    class TKeys
        : public TNAry<TAddr::TMemberVec> {
      NO_COPY(TKeys);
      public:

      typedef std::shared_ptr<TKeys> TPtr;

      static TPtr New(const TAddr::TMemberVec &members, const Type::TType &value_type, const TPosRange &pos_range);

      virtual void Accept(const TVisitor &visitor) const;

      /* Alias for GetContainer from TNAry */
      inline const TAddr::TMemberVec &GetMembers() const {
        assert(this);
        return GetContainer();
      }

      /* Returns the type of Addr used in the key expression. */
      virtual Type::TType GetAddrType() const;

      /* A sequence of GetAddrType */
      virtual Type::TType GetTypeImpl() const override;

      /* Get's the type of the value stored in keys expression */
      const Type::TType &GetValueType() const {
        return ValueType;
      }

      private:
      Type::TType ValueType;

      TKeys(const TAddr::TMemberVec &members, const Type::TType &value_type, const TPosRange &pos_range);

    };  // TKeys

  }  // Expr

}  // Orly
