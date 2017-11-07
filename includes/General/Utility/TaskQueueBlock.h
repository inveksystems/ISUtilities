#ifndef IS__GENERAL__UTILITY__TASK_QUEUE_BLOCK_H
#define IS__GENERAL__UTILITY__TASK_QUEUE_BLOCK_H

#include <queue>
#include <functional>
#include <memory>

namespace IS {
  namespace General {
    namespace Utility {
      class TaskQueueBlock {
      public:
        using TaskQueue = std::queue<std::function<void(void)>>;
        TaskQueueBlock () : mp_tasks(std::make_shared<TaskQueue>()) {}
      public:
        template <class TASK, class ...ARGS> void addTask (TASK task, ARGS &&...args) {
          mp_tasks->push([task, &args...](){
            task(std::forward<ARGS>(args)...);
          });
        }

        void clear () {
          while (mp_tasks->size()) {
            mp_tasks->pop();
          }
        }

        void operator() () const {
          while (mp_tasks->size()) {
            mp_tasks->front()();
            mp_tasks->pop();
          }
        }

      private:
        std::shared_ptr<TaskQueue> mp_tasks;
      };
    }
  }
}

#endif
