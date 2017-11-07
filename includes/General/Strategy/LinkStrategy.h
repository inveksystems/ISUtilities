#ifndef INVEK_SYSTEMS__STRATEGY__LINK_STRATEGY_H
#define INVEK_SYSTEMS__STRATEGY__LINK_STRATEGY_H

#include "PointerCreatorStrategy.h"

#include <list>
#include <utility>
#include <stdexcept>

namespace IS {
  namespace General {
    namespace Strategy {
      template <
        class ITEM,
        class LINK
      > class LinkStrategy :
        virtual public PointerCreatorStrategy<ITEM>,
        virtual public std::enable_shared_from_this<ITEM> {
      public:
        virtual ~LinkStrategy () {
          removeAllLinks();
        }
      public:
        template <class ...LinkArgs> void addLink (std::shared_ptr<ITEM> &item, LinkArgs &&...args) {
          if (hasLink(item)) {return;}
          auto link = std::make_shared<LINK>(std::forward<LinkArgs>(args)...);
          mp_links.push_back(std::make_pair(std::shared_ptr<ITEM>(item), link));
          item->mp_links.push_back(std::make_pair(this->shared_from_this(), link));
        }
        void removeLink (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_links.begin(); it != mp_links.end(); it++) {
            if (it->first == item) {
              mp_links.erase(it);
              for (auto jt = item->mp_links.begin(); jt != item->mp_links.end(); jt++) {
                if (jt->first.get() == this) {
                  item->mp_links.erase(jt);
                  return;
                }
              }
              return;
            }
          }
        }
        void removeAllLinks () {
          while (mp_links.size()) {
            removeLink(mp_links.back().first);
          }
        }
        bool hasLink (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_links.begin(); it != mp_links.end(); it++) {
            if (it->first == item) {
              return true;
            }
          }
          return false;
        }
        const auto& links () {
          return mp_links;
        }
        auto getLinkByItem (std::shared_ptr<ITEM> item) {
          for (auto it = mp_links.begin(); it != mp_links.end(); it++) {
            if (it->first == item) {
              return it->second;
            }
          }
          throw std::runtime_error("Link by current item is not existed.");
        }
        auto getItemByLink (std::shared_ptr<LINK> link) {
          for (auto it = mp_links.begin(); it != mp_links.end(); it++) {
            if (it->second == link) {
              return it->first;
            }
          }
          throw std::runtime_error("Item by current link is not existed.");
        }

      private:
        std::list<std::pair<std::shared_ptr<ITEM>, std::shared_ptr<LINK>>> mp_links;
      };
    }
  }
}

#endif
