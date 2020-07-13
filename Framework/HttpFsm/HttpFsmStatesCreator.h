#include <map>
#include <vector>
#include "HttpFsmStateIFace.h"

namespace HttpFsmStatesCreator
{
    template <typename ... Types>
    std::map< StateType, HttpFsmStateIFace* > CreateStatesInstances()
    {
        std::map< StateType, HttpFsmStateIFace* > instances;
        std::vector<HttpFsmStateIFace*> vec = { new Types()... };
        for (auto i : vec)
            instances[i->getType()] = i;

        return instances;
    }
};
