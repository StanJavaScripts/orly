/* <orly/indy/context.h>

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

#include <unordered_set>

#include <base/chrono.h>
#include <base/class_traits.h>
#include <base/no_throw.h>
#include <base/uuid.h>
#include <orly/atom/suprena.h>
#include <orly/context_base.h>
#include <orly/indy/fiber/fiber.h>
#include <orly/indy/manager.h>
#include <orly/indy/sequence_number.h>
#include <orly/key_generator.h>
#include <orly/package/rt.h>
#include <orly/sabot/all.h>
#include <orly/var/sabot_to_var.h>

namespace Orly {

  namespace Indy {

    /* TODO */
    class TContext
        : public TContextBase {
      NO_COPY(TContext);
      public:

      /* Forward Declarations. */
      class TKeyCursor;

      /* TODO */
      typedef InvCon::UnorderedList::TCollection<TContext, TKeyCursor> TKeyCursorCollection;

      private:

      /* TODO */
      typedef std::vector<std::pair<Indy::L0::TManager::TPtr<TRepo>, std::unique_ptr<TRepo::TView>>> TRepoTree;

      /* TODO */
      class TPresentWalker {
        NO_COPY(TPresentWalker);
        public:

        /* TODO */
        using TItem = Orly::Indy::TPresentWalker::TItem;

        /* TODO */
        TPresentWalker(TContext *context, const TRepoTree &repo_tree, const TIndexKey &key);

        /* TODO */
        TPresentWalker(TContext *context, const TRepoTree &repo_tree, const TIndexKey &from, const TIndexKey &to);

        /* TODO */
        ~TPresentWalker();

        /* TODO */
        inline operator bool() const;

        /* TODO */
        inline const TItem &operator*() const;

        /* TODO */
        inline TPresentWalker &operator++();

        private:

        /* TODO */
        void Refresh();

        /* TODO */
        std::vector<std::shared_ptr<Indy::TPresentWalker>> WalkerVec;

        /* TODO */
        Util::TMinHeap<Indy::TPresentWalker::TItem, size_t> MinHeap;

        /* TODO */
        bool Valid;

        /* TODO */
        mutable TItem Item;

      };  // TPresentWalker

      public:

      /* TODO */
      class TKeyCursor
          : public Orly::TKeyCursor {
        NO_COPY(TKeyCursor);
        public:

        /* TODO */
        typedef InvCon::UnorderedList::TMembership<TKeyCursor, TContext> TContextMembership;

        /* TODO */
        TKeyCursor(TContext *context, const Indy::TIndexKey &pattern);

        /* TODO */
        TKeyCursor(TContext *context, const Indy::TIndexKey &from, const Indy::TIndexKey &to);

        /* TODO */
        virtual ~TKeyCursor();

        /* TODO */
        virtual operator bool() const override;

        /* TODO */
        virtual const Indy::TKey &operator*() const override;

        /* TODO */
        virtual const Indy::TKey *operator->() const override;

        /* TODO */
        virtual TKeyCursor &operator++() override;

        /* TODO */
        const TContext::TPresentWalker::TItem &GetVal() const;

        private:

        /* TODO */
        void Refresh() const;

        /* TODO */
        Indy::TIndexKey Key;

        /* TODO */
        Indy::TIndexKey To;

        /* TODO */
        mutable bool Valid;

        /* TODO */
        mutable bool Cached;

        /* TODO */
        mutable TPresentWalker Csr;

        /* TODO */
        mutable Indy::TKey Item;

        /* TODO */
        mutable TContextMembership::TImpl ContextMembership;

      };  // TKeyCursor

      /* TODO */
      TContext(const Indy::L0::TManager::TPtr<TRepo> &private_repo, Atom::TCore::TExtensibleArena *arena);

      /* TODO */
      virtual ~TContext();

      /* TODO */
      virtual Indy::TKey operator[](const Indy::TIndexKey &key) override;

      /* TODO */
      virtual bool Exists(const Indy::TIndexKey &key) override;

      /* TODO */
      inline size_t GetWalkerCount() const {
        assert(this);
        return WalkerCount;
      }

      /* TODO */
      const Base::TTimer &GetPresentWalkConsTimer() const {
        assert(this);
        return PresentWalkConsTimer;
      }

      /* TODO */
      TKeyCursorCollection::TImpl KeyCursorCollection;

      private:

      /* TODO */
      TRepoTree RepoTree;

      /* TODO */
      size_t WalkerCount;

      /* TODO */
      Base::TTimer PresentWalkConsTimer;

      /* TODO */
      friend class TIndyContext;

    };  // TContext

    /* TODO */
    class TIndyContext
          : public Orly::Package::TContext {
      NO_COPY(TIndyContext);
      public:

      /* TODO */
      TIndyContext(
          const Rt::TOpt<Base::TUuid> &user_id,
          const Base::TUuid &session_id,
          Indy::TContext &context,
          Atom::TCore::TExtensibleArena *arena,
          Base::TScheduler *scheduler,
          Rt::TOpt<Base::Chrono::TTimePnt> now,
          Rt::TOpt<uint32_t> seed)
          : Orly::Package::TContext(user_id, session_id, arena, scheduler, now, seed), DataContext(context) {}

      /* TODO */
      virtual Orly::TContextBase &GetFlux() override{
        return DataContext;
      }

      /* TODO */
      virtual TKeyCursor *NewKeyCursor(TContextBase *context, const Indy::TIndexKey &pattern) const override {
        auto data_context = dynamic_cast<Indy::TContext *>(context);

        /*
        void *state_alloc = alloca(Sabot::State::GetMaxStateSize());
        std::cout << "NewKeyCursor(";
        pattern.GetState(state_alloc)->Accept(Sabot::TStateDumper(std::cout));
        std::cout << ")" << std::endl;
        */
        return new Indy::TContext::TKeyCursor(data_context, pattern);
      }

      /* TODO */
      virtual TKeyCursor *NewKeyCursor(TContextBase *context, const Indy::TIndexKey &from, const Indy::TIndexKey &to) const override {
        auto data_context = dynamic_cast<Indy::TContext *>(context);

        /*
        void *state_alloc = alloca(Sabot::State::GetMaxStateSize());
        std::cout << "NewKeyCursor(";
        pattern.GetState(state_alloc)->Accept(Sabot::TStateDumper(std::cout));
        std::cout << ")" << std::endl;
        */
        return new Indy::TContext::TKeyCursor(data_context, from, to);
      }

      private:

      /* TODO */
      Indy::TContext &DataContext;

    };  // TIndyContext

    /********************
      ***** inline ******
      ******************/

    inline TContext::TPresentWalker::operator bool() const {
      assert(this);
      return Valid;
    }

    inline const TContext::TPresentWalker::TItem &TContext::TPresentWalker::operator*() const {
      assert(this);
      assert(Valid);
      return Item;
    }

    inline TContext::TPresentWalker &TContext::TPresentWalker::operator++() {
      assert(this);
      assert(Valid);
      Valid = static_cast<bool>(MinHeap);
      Refresh();
      return *this;
    }

  }  // Indy

}  // Orly
