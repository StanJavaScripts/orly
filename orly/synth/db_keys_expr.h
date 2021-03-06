/* <orly/synth/db_keys_expr.h>

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

#include <functional>
#include <vector>

#include <base/class_traits.h>
#include <orly/orly.package.cst.h>
#include <orly/synth/expr.h>
#include <orly/synth/new_expr.h>
#include <orly/synth/scope_and_def.h>
#include <orly/synth/type.h>

namespace Orly {

  namespace Synth {

    /* TODO */
    class TDbKeysExpr
        : public TExpr {
      NO_COPY(TDbKeysExpr);
      public:

      /* TODO */
      TDbKeysExpr(const TExprFactory *expr_factory, const Package::Syntax::TDbKeysExpr *db_keys_expr);

      /* TODO */
      virtual ~TDbKeysExpr();

      /* TODO */
      virtual Expr::TExpr::TPtr Build() const;

      /* TODO */
      virtual void ForEachInnerScope(const std::function<void (TScope *)> &cb);

      /* TODO */
      virtual void ForEachRef(const std::function<void (TAnyRef &)> &cb);

      /* TODO */
      TType* GetValueType() {
        return ValueType;
      }

      private:

      /* TODO */
      class TMember {
        NO_COPY(TMember);
        public:

        /* TODO */
        virtual ~TMember();

        /* TODO */
        virtual Expr::TExpr::TPtr Build() const = 0;

        /* TODO */
        virtual void ForEachInnerScope(const std::function<void (TScope *)> &cb) = 0;

        /* TODO */
        virtual void ForEachRef(const std::function<void (TAnyRef &)> &cb) = 0;

        /* TODO */
        TAddrDir GetAddrDir() const {
          assert(this);
          return AddrDir;
        }

        protected:

        /* TODO */
        TMember(TAddrDir addr_dir);

        private:

        /* TODO */
        TAddrDir AddrDir;

      };  // TMember

      /* TODO */
      typedef std::vector<TMember *> TMemberVec;

      /* TODO */
      class TFixedMember
          : public TMember {
        NO_COPY(TFixedMember);
        public:

        /* TODO */
        TFixedMember(TAddrDir addr_dir, TExpr *expr);

        /* TODO */
        virtual ~TFixedMember();

        /* TODO */
        virtual Expr::TExpr::TPtr Build() const;

        /* TODO */
        virtual void ForEachInnerScope(const std::function<void (TScope *)> &cb);

        /* TODO */
        virtual void ForEachRef(const std::function<void (TAnyRef &)> &cb);

        private:

        /* TODO */
        TExpr *Expr;

      };  // TFixedMember

      /* TODO */
      class TFreeMember
          : public TMember {
        NO_COPY(TFreeMember);
        public:

        /* TODO */
        TFreeMember(TAddrDir addr_dir, TType *type, const TPosRange &pos_range);

        /* TODO */
        virtual ~TFreeMember();

        /* TODO */
        virtual Expr::TExpr::TPtr Build() const;

        /* TODO */
        virtual void ForEachInnerScope(const std::function<void (TScope *)> &cb);

        /* TODO */
        virtual void ForEachRef(const std::function<void (TAnyRef &)> &cb);

        private:

        /* TODO */
        TType *Type;

        /* TODO */
        const TPosRange PosRange;

      };  // TFreeMember

      /* TODO */
      void Cleanup();

      /* TODO */
      const Package::Syntax::TDbKeysExpr *DbKeysExpr;

      /* TODO */
      TType *ValueType;

      /* TODO */
      TMemberVec Members;

    };  // TDbKeysExpr

  }  // Synth

}  // Orly
