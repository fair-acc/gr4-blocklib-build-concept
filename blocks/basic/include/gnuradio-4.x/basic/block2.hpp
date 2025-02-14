#ifndef BLOCK2_HPP
#define BLOCK2_HPP
#include "gr_registry.hpp"

#include <iostream>

namespace gr::basic {

// register Block2<T, U> for T in [int, float], U in [double]
GR_REGISTER_BLOCK(gr::basic::Block2, ([T], [U]), [ int, float ], [double])

template<typename T, typename U>
struct Block2 : public gr::IBlock {
    void doWork() override { std::cout << "Block2<" << typeid(T).name() << ", " << typeid(U).name() << "> doWork()\n"; }
};

} // namespace gr::basic

#endif // #ifndef BLOCK2_HPP
