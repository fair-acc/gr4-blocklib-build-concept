#ifndef BLOCK3_HPP
#define BLOCK3_HPP
#include "gr_registry.hpp"

#include <iostream>

namespace gr::basic {

// register only Block3<int> // not empty "" -> should fall back to the class type name
GR_REGISTER_BLOCK("", gr::basic::Block3, [T], [int])

template<typename T>
struct Block3 : public gr::IBlock {
    void doWork() override { std::cout << "Block3<" << typeid(T).name() << "> doWork()\n"; }
};

} // namespace gr::basic

#endif // #ifndef BLOCK3_HPP
