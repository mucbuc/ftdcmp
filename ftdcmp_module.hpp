#pragma once

namespace ftdcmp {
template <typename Base>
struct Module : public Base {

    template <typename... U>
    Module(U... init)
        : Base(init...)
    {
    }

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