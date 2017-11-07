#ifndef INVEK_SYSTEMS__STRATEGY__ASSOCIATION_STRATEGY_H
#define INVEK_SYSTEMS__STRATEGY__ASSOCIATION_STRATEGY_H

#include "PointerCreatorStrategy.h"

#include <list>
#include <utility>
#include <stdexcept>

namespace IS {
  namespace General {
    namespace Strategy {
      template <
        class ITEM,
        class ASSOC
      > class AssociationStrategy :
        virtual public PointerCreatorStrategy<ITEM>,
        virtual public std::enable_shared_from_this<ITEM> {
      public:
        virtual ~AssociationStrategy () {
          removeAllAssociations();
        }
      public:
        template <class ...AssociationArgs> void addAssociation (std::shared_ptr<ITEM> &item, AssociationArgs &&...args) {
          if (hasAssociation(item)) {return;}
          mp_associations.push_back(std::make_pair(std::shared_ptr<ITEM>(item), std::make_shared<ASSOC>(std::forward<AssociationArgs>(args)...)));
        }
        void removeAssociation (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_associations.begin(); it != mp_associations.end(); it++) {
            if (it->first == item) {
              mp_associations.erase(it);
              return;
            }
          }
        }
        void removeAllAssociations () {
          mp_associations.clear();
        }
        bool hasAssociation (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_associations.begin(); it != mp_associations.end(); it++) {
            if (it->first == item) {
              return true;
            }
          }
          return false;
        }
        const auto& associations () {
          return mp_associations;
        }
        auto getAssociationByItem (std::shared_ptr<ITEM> item) {
          for (auto it = mp_associations.begin(); it != mp_associations.end(); it++) {
            if (it->first == item) {
              return it->second;
            }
          }
          throw std::runtime_error("Association by current item is not existed.");
        }
        auto getItemByAssociation (std::shared_ptr<ASSOC> assoc) {
          for (auto it = mp_associations.begin(); it != mp_associations.end(); it++) {
            if (it->second == assoc) {
              return it->first;
            }
          }
          throw std::runtime_error("Item by current association is not existed.");
        }

      protected:
        AssociationStrategy () {}
      private:
        std::list<std::pair<std::shared_ptr<ITEM>, std::shared_ptr<ASSOC>>> mp_associations;
      };
    }
  }
}

#endif
