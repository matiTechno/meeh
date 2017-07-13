#pragma once

#include <functional>
#include <assert.h>
#include "NonCpNonMv.hpp"

namespace meeh
{

// RAII pattern for lazy initialized resources
class Raii: public NonCpNonMv
{
public:
    ~Raii() {if(deleter) deleter();}

    template<typename T>
    void set(T&& deleter)
    {
        assert(isSet == 0);
        isSet = true;
        this->deleter = std::forward<T>(deleter);
    }

private:
    bool isSet = false;
    std::function<void(void)> deleter;
};

} // meeh
