#pragma once

#include "ftdcmp.hpp"

namespace ftdcmp {
template <typename Base>
struct Module : public Base {

    template <typename... U>
    Module(U&&... init)
        : Base(std::forward<U>(init)...)
    {
    }

    Module(Module&&) = default;

    void init(auto done)
    {
        ftdcmp::init();

        done([]() {
            ftdcmp::release();
        });
    }

    void run(auto& gs, float ft, auto cancel)
    {
    }
};

}