#ifndef INVEK_SYSTEMS__STRATEGY__RELATIONSHIP_STRATEGY_H
#define INVEK_SYSTEMS__STRATEGY__RELATIONSHIP_STRATEGY_H

#include "PointerCreatorStrategy.h"

#include <list>
#include <utility>
#include <stdexcept>

namespace IS {
  namespace General {
    namespace Strategy {
      template <class ITEM_SELF, class ITEM = ITEM_SELF> class RelationshipStrategy :
        virtual public PointerCreatorStrategy<ITEM>,
        virtual public std::enable_shared_from_this<ITEM> {
      public:
        virtual ~RelationshipStrategy () {
          removeAllRelationships();
        }
      public:
        void addRelationship (std::shared_ptr<ITEM> item) {
          if (hasRelationship(item)) {return;}
          mp_relationships.push_back(item);
          item->mp_relationships.push_back(this->shared_from_this());
        }
        void removeRelationship (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_relationships.begin(); it != mp_relationships.end(); it++) {
            if (item == *it) {
              mp_relationships.erase(it);
              for (auto jt = item->mp_relationships.begin(); jt != item->mp_relationships.end(); jt++) {
                if (jt->get() == this) {
                  item->mp_relationships.erase(jt);
                  return;
                }
              }
              return;
            }
          }
        }
        void removeAllRelationships () {
          while (mp_relationships.size()) {
            removeRelationship(mp_relationships.back());
          }
        }
        bool hasRelationship (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_relationships.begin(); it != mp_relationships.end(); it++) {
            if (item == *it) {
              return true;
            }
          }
          return false;
        }
        const auto& relationships () {
          return mp_relationships;
        }

      private:
        std::list<std::shared_ptr<ITEM>> mp_relationships;
      };
    }
  }
}

#endif
