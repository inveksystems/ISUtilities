#ifndef IS__GENERAL__UTILITY__TASK_BLOCK_H
#define IS__GENERAL__UTILITY__TASK_BLOCK_H

#include <list>
#include <functional>
#include <memory>

namespace IS {
  namespace General {
    namespace Utility {
      class TaskBlock {
      public:
        using TaskList = std::list<std::function<void(void)>>;
        TaskBlock () : mp_tasks(std::make_shared<TaskList>()) {}
      public:
        template <class TASK, class ...ARGS> void addTask (TASK task, ARGS &&...args) {
          mp_tasks->push_back([task, &args...](){
            task(std::forward<ARGS>(args)...);
          });
        }

        void clear () {
          mp_tasks->clear();
        }

        void operator() (bool reverse = false) const {
          auto run = [](auto start, auto end){
            for (auto it = start; it != end; it++) {
              (*it)();
            }
          };
          if (!reverse) {
            run(mp_tasks->begin(), mp_tasks->end());
          } else {
            run(mp_tasks->rbegin(), mp_tasks->rend());
          }
        }

      private:
        std::shared_ptr<TaskList> mp_tasks;
      };
    }
  }
}

#endif
