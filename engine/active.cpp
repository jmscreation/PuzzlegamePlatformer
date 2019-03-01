#include "engine.h"

namespace Engine {

    std::vector<Active*> Active::list;
    bool Active::iterating = false;

    Active::Active() {
        list.push_back(this);
    }

    Active::~Active() {
        auto pos = std::find(list.begin(),list.end(),this);
        if(iterating)
            *pos = NULL;
        else
            list.erase(pos);
    }

    std::vector<ActiveEnd*> ActiveEnd::list;
    bool ActiveEnd::iterating = false;

    ActiveEnd::ActiveEnd() {
        list.push_back(this);
    }

    ActiveEnd::~ActiveEnd() {
        auto pos = std::find(list.begin(),list.end(),this);
        if(iterating)
            *pos = NULL;
        else
            list.erase(pos);
    }

}