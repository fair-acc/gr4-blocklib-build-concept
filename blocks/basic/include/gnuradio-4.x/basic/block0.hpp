#ifndef BLOCK0_HPP
#define BLOCK0_HPP
#include <gr_registry.hpp>

#include <iostream>

namespace gr::basic {

// register non-templated Block0
GR_REGISTER_BLOCK(gr::basic::Block0)
//GR_REGISTER_BLOCK(gr::basic::BlockError) // uncomment me to see a compiler error output -> should you link you back here

struct Block0 : public gr::IBlock {
    void doWork() override { std::cout << "Block0 doWork()\n"; }
};

} // namespace gr::basic

#endif // #ifndef BLOCK0_HPP
