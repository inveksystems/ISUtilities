#ifndef IS__GENERAL__UTILITY__HANDLER_BLOCK_H
#define IS__GENERAL__UTILITY__HANDLER_BLOCK_H

#include <list>
#include <functional>
#include <memory>

namespace IS {
  namespace General {
    namespace Utility {
      template <class CONTEXT> class HandlerBlock {
      public:
        using HandlerList = std::list<std::function<void(CONTEXT&)>>;
        HandlerBlock () : mp_handlers(std::make_shared<HandlerList>()) {}
      public:
        template <class HANDLER, class ...ARGS> void addHandler (HANDLER handler, ARGS &&...args) {
          mp_handlers->push_back([handler, args...](CONTEXT &context){
            handler(context, args...);
          });
        }

        void clear () {
          mp_handlers->clear();
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
              (*it)(context);
            }
          };
          if (!reverse) {
            run(mp_handlers->begin(), mp_handlers->end());
          } else {
            run(mp_handlers->rbegin(), mp_handlers->rend());
          }
        }

      private:
        std::shared_ptr<HandlerList> mp_handlers;
      };
    }
  }
}

#endif
