#ifndef BLOCK1_HPP
#define BLOCK1_HPP
#include <gr_registry.hpp>

#include <iostream>

namespace gr::basic {

// register Block1<T> for T in [float, double]
GR_REGISTER_BLOCK(gr::basic::Block1, [T], [ float, double ])

template<typename T>
struct Block1 : public gr::IBlock {
    void doWork() override { std::cout << "Block1<" << typeid(T).name() << "> doWork()\n"; }
};

} // namespace gr::basic

#endif // #ifndef BLOCK1_HPP