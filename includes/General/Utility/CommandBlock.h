#ifndef IS__GENERAL__UTILITY__COMMAND_BLOCK_H
#define IS__GENERAL__UTILITY__COMMAND_BLOCK_H

#include <map>
#include <string>
#include <memory>

namespace IS {
  namespace General {
    namespace Utility {
      template <class CONTEXT, class KEY = std::string> class CommandBlock {
      public:
        using CommandMap = std::map<KEY, std::function<void(CONTEXT&)>>;
        CommandBlock (CONTEXT &context) : mp_context(context), mp_handlers(std::make_shared<CommandMap>()) {}
      public:
        template <class HANDLER> void registerCommand (KEY key, HANDLER handler) {
          (*mp_handlers)[key] = handler;
        }
        void unregisterCommand (KEY key) {
          auto it = mp_handlers->find(key);
          if (it != mp_handlers->end()) {
            mp_handlers->erase(key);
          }
        }

        void operator() (KEY key) {
          auto it = mp_handlers->find(key);
          if (it != mp_handlers->end()) {
            it->second(mp_context);
          }
        }

      private:
        CONTEXT& mp_context;
        std::shared_ptr<CommandMap> mp_handlers;
      };


      //specialization for smart pointers (shared_ptr)
      template <class CONTEXT, class KEY> class CommandBlock<std::shared_ptr<CONTEXT>, KEY> {
      public:
        using CommandMap = std::map<KEY, std::function<void(std::shared_ptr<CONTEXT>&)>>;
        CommandBlock (std::shared_ptr<CONTEXT> contextPtr) : mp_context(contextPtr), mp_handlers(std::make_shared<CommandMap>()) {}
      public:
        template <class HANDLER> void registerCommand (KEY key, HANDLER handler) {
          (*mp_handlers)[key] = handler;
        }
        void unregisterCommand (KEY key) {
          auto it = mp_handlers->find(key);
          if (it != mp_handlers->end()) {
            mp_handlers.erase(key);
          }
        }

        void operator() (KEY key) {
          auto it = mp_handlers->find(key);
          if (it != mp_handlers->end()) {
            it->second(mp_context);
          }
        }

      private:
        std::shared_ptr<CONTEXT> mp_context;
        std::shared_ptr<CommandMap> mp_handlers;
      };
    }
  }
}

#endif
