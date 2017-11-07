#ifndef IS__GENERAL__UTILITY__HANDLER_PRIORITY_BLOCK_H
#define IS__GENERAL__UTILITY__HANDLER_PRIORITY_BLOCK_H

#include <map>
#include <functional>

namespace IS {
  namespace General {
    namespace Utility {
      template <class CONTEXT, class PRIORITY = unsigned> class HandlerPriorityBlock {
      public:
        template <class HANDLER, class ...ARGS> void addHandler (PRIORITY priority, HANDLER handler, ARGS &&...args) {
          mp_handlers.insert(std::make_pair(priority, [handler, args...](CONTEXT &context){
            handler(context, args...);
          }));
        }

        void clear () {
          mp_handlers.clear();
        }

        auto getBindingFunction (CONTEXT &context) {
          auto handlerBlock = *this;
          return [&context, handlerBlock](){
            handlerBlock(context);
          };
        }

        void operator() (CONTEXT &context, bool reverse = false) const {
          auto run = [&context](auto start, auto end){
            for (auto it = start; it != end; it++) {
              it->second(context);
            }
          };
          if (!reverse) {
            run(mp_handlers.begin(), mp_handlers.end());
          } else {
            run(mp_handlers.rbegin(), mp_handlers.rend());
          }
        }

      private:
        std::multimap<PRIORITY, std::function<void(CONTEXT&)>> mp_handlers;
      };
    }
  }
}

#endif
