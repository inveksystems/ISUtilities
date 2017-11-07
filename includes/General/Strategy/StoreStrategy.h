#ifndef INVEK_SYSTEMS__STRATEGY__STORE_STRATEGY_H
#define INVEK_SYSTEMS__STRATEGY__STORE_STRATEGY_H

#include "PointerCreatorStrategy.h"

#include <list>
#include <utility>
#include <stdexcept>

namespace IS {
  namespace General {
    namespace Strategy {
      template <class ITEM_SELF, class ITEM = ITEM_SELF> class StoreStrategy :
        virtual public PointerCreatorStrategy<ITEM> {
      public:
        virtual ~StoreStrategy () {
          removeAllStoreItems();
        }
      public:
        void addStoreItem (std::shared_ptr<ITEM> item) {
          if (hasStoreItem(item)) {return;}
          mp_store.push_back(item);
        }
        void removeStoreItem (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_store.begin(); it != mp_store.end(); it++) {
            if (item == *it) {
              mp_store.erase(it);
              return;
            }
          }
        }
        void removeAllStoreItems () {
          mp_store.clear();
        }
        bool hasStoreItem (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_store.begin(); it != mp_store.end(); it++) {
            if (item == *it) {
              return true;
            }
          }
          return false;
        }
        const auto& store () {
          return mp_store;
        }

      private:
        std::list<std::shared_ptr<ITEM>> mp_store;
      };
    }
  }
}

#endif
