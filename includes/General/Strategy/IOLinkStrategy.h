#ifndef INVEK_SYSTEMS__STRATEGY__IO_LINK_STRATEGY_H
#define INVEK_SYSTEMS__STRATEGY__IO_LINK_STRATEGY_H

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
      > class IOLinkStrategy :
        virtual public PointerCreatorStrategy<ITEM>,
        virtual public std::enable_shared_from_this<ITEM> {
      public:
        virtual ~IOLinkStrategy () {
          removeAllInputLinks();
          removeAllOutputLinks();
        }
      public:
        template <class ...LinkArgs> void addInputLink (std::shared_ptr<ITEM> &item, LinkArgs &&...args) {
          if (hasInputLink(item)) {return;}
          auto link = std::make_shared<LINK>(std::forward<LinkArgs>(args)...);
          mp_input_links.push_back(std::make_pair(std::shared_ptr<ITEM>(item), link));
          item->mp_output_links.push_back(std::make_pair(this->shared_from_this(), link));
        }
        template <class ...LinkArgs> void addOutputLink (std::shared_ptr<ITEM> &item, LinkArgs &&...args) {
          if (hasOutputLink(item)) {
            return;
          }
          auto link = std::make_shared<LINK>(std::forward<LinkArgs>(args)...);
          mp_output_links.push_back(std::make_pair(std::shared_ptr<ITEM>(item), link));
          item->mp_input_links.push_back(std::make_pair(this->shared_from_this(), link));
        }
        void removeInputLink (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_input_links.begin(); it != mp_input_links.end(); it++) {
            if (it->first == item) {
              mp_input_links.erase(it);
              for (auto jt = item->mp_output_links.begin(); jt != item->mp_output_links.end(); jt++) {
                if (jt->first.get() == this) {
                  item->mp_output_links.erase(jt);
                  return;
                }
              }
              return;
            }
          }
        }
        void removeOutputLink (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_output_links.begin(); it != mp_output_links.end(); it++) {
            if (it->first == item) {
              mp_output_links.erase(it);
              for (auto jt = item->mp_input_links.begin(); jt != item->mp_input_links.end(); jt++) {
                if (jt->first.get() == this) {
                  item->mp_input_links.erase(jt);
                  return;
                }
              }
              return;
            }
          }
        }
        void removeAllInputLinks () {
          while (mp_input_links.size()) {
            removeInputLink(mp_input_links.back().first);
          }
        }
        void removeAllOutputLinks () {
          while (mp_output_links.size()) {
            removeOutputLink(mp_output_links.back().first);
          }
        }
        bool hasInputLink (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_input_links.begin(); it != mp_input_links.end(); it++) {
            if (it->first == item) {
              return true;
            }
          }
          return false;
        }
        bool hasOutputLink (std::shared_ptr<ITEM> &item) {
          for (auto it = mp_output_links.begin(); it != mp_output_links.end(); it++) {
            if (it->first == item) {
              return true;
            }
          }
          return false;
        }
        const auto& inputLinks () {
          return mp_input_links;
        }
        const auto& outputLinks () {
          return mp_output_links;
        }
        auto getInputLinkByItem (std::shared_ptr<ITEM> item) {
          for (auto it = mp_input_links.begin(); it != mp_input_links.end(); it++) {
            if (it->first == item) {
              return it->second;
            }
          }
          throw std::runtime_error("Input link by current item is not existed.");
        }
        auto getItemByInputLink (std::shared_ptr<LINK> link) {
          for (auto it = mp_input_links.begin(); it != mp_input_links.end(); it++) {
            if (it->second == link) {
              return it->first;
            }
          }
          throw std::runtime_error("Item by current input link is not existed.");
        }
        auto getOutputLinkByItem (std::shared_ptr<ITEM> item) {
          for (auto it = mp_output_links.begin(); it != mp_output_links.end(); it++) {
            if (it->first == item) {
              return it->second;
            }
          }
          throw std::runtime_error("Output link by current item is not existed.");
        }
        auto getItemByOutputLink (std::shared_ptr<LINK> link) {
          for (auto it = mp_output_links.begin(); it != mp_output_links.end(); it++) {
            if (it->second == link) {
              return it->first;
            }
          }
          throw std::runtime_error("Item by current output link is not existed.");
        }

      private:
        std::list<std::pair<std::shared_ptr<ITEM>, std::shared_ptr<LINK>>> mp_input_links;
        std::list<std::pair<std::shared_ptr<ITEM>, std::shared_ptr<LINK>>> mp_output_links;
      };
    }
  }
}

#endif
