#ifndef IS__UTILITY__CALLABLE_CHAIN_H
#define IS__UTILITY__CALLABLE_CHAIN_H

#include <functional>

namespace IS {
  namespace General {
    namespace Utility {
      template <class CHAIN> auto CallableChain (CHAIN chain) {
        return [chain](auto &&...args){return chain(std::forward<decltype(args)>(args)...);};
      }
      template <class CHAIN, class ...NEXT_CHAINS> auto CallableChain (CHAIN chain, NEXT_CHAINS ...chains) {
        return [chain, chains...](auto &&...args){
          return CallableChain(chains...)(chain(std::forward<decltype(args)>(args)...));
        };
      }
    }
  }
}

#endif
