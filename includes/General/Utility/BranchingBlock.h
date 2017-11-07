#ifndef IS__GENERAL__UTILITY__BRANCHING_BLOCK_H
#define IS__GENERAL__UTILITY__BRANCHING_BLOCK_H

#include <map>
#include <functional>
#include <memory>
#include <iostream>

namespace IS {
  namespace General {
    namespace Utility {
      template <class KEY = std::string> class BranchingBlock {
      public:
        using BranchMap = std::map<KEY, std::pair<std::function<void(void)>, BranchingBlock<KEY>>>;
        BranchingBlock () : mp_branchs(std::make_shared<BranchMap>()), get_root_branch([this](){return *this;}) {}
      public:
        template <class TASK> void addBranch (KEY key, TASK task, BranchingBlock<KEY> branch = BranchingBlock<KEY>()) {
          branch.get_root_branch = get_root_branch;
          if (branch.mp_branchs != mp_branchs) {
            auto self = *this;
            branch.get_parent_branch = [self](){
              return self;
            };
          } else {
            branch.get_parent_branch = get_parent_branch;
          }
          (*mp_branchs)[key] = std::make_pair(task, branch);
        }

        BranchingBlock<KEY> operator() (KEY key) {
          auto it = mp_branchs->find(key);
          if (it != mp_branchs->end()) {
            it->second.first();
            return it->second.second;
          }

          auto branch = BranchingBlock<KEY>();
          auto self = *this;
          branch.get_root_branch = get_root_branch;
          branch.get_parent_branch = [self](){
            return self;
          };

          return branch;
        }

        bool hasBranch (KEY key) {
          auto it = mp_branchs->find(key);
          if (it != mp_branchs->end()) {
            return true;
          }
          return false;
        }

        BranchingBlock<KEY> back () {
          return get_parent_branch();
        }

        BranchingBlock<KEY> root () {
          return get_root_branch();
        }

      private:
        std::function<BranchingBlock<KEY>(void)> get_parent_branch;
        std::function<BranchingBlock<KEY>(void)> get_root_branch;
        std::shared_ptr<BranchMap> mp_branchs;
      };
    }
  }
}

#endif
