#ifndef INVEK_SYSTEMS__POINTER_CREATOR_STRATEGY_H
#define INVEK_SYSTEMS__POINTER_CREATOR_STRATEGY_H

#include <memory>

namespace IS {
  namespace General {
    namespace Strategy {
      template <class ITEM> class PointerCreatorStrategy {
      public:
        template <class ...Args> static std::unique_ptr<ITEM> create (Args &&...args) {
          return std::make_unique<ITEM>(std::forward<Args>(args)...);
        }
      protected:
        PointerCreatorStrategy () {}
      };
    }
  }
}

#endif
