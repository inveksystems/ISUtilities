#ifndef IS__GENERAL__UTILITY__TASK_PRIORITY_BLOCK_H
#define IS__GENERAL__UTILITY__TASK_PRIORITY_BLOCK_H

#include <map>
#include <functional>

namespace IS {
  namespace General {
    namespace Utility {
      template <class PRIORITY = unsigned> class TaskPriorityBlock {
      public:
        template <class TASK, class ...ARGS> void addTask (PRIORITY priority, TASK task, ARGS &&...args) {
          mp_tasks.insert(std::make_pair(priority, [task, &args...](){
            task(std::forward<ARGS>(args)...);
          }));
        }

        void clear () {
          mp_tasks.clear();
        }

        void operator() (bool reverse = false) const {
          auto run = [](auto start, auto end){
            for (auto it = start; it != end; it++) {
              it->second();
            }
          };
          if (!reverse) {
            run(mp_tasks.begin(), mp_tasks.end());
          } else {
            run(mp_tasks.rbegin(), mp_tasks.rend());
          }
        }

      private:
        std::multimap<PRIORITY, std::function<void(void)>> mp_tasks;
      };
    }
  }
}

#endif
