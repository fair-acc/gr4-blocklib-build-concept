// main.cpp
#include "gr_registry.hpp"

#include <iostream>
#include <typeinfo>   // for typeid

#include <gnuradio-4.x/basic/block1.hpp>
#include <gnuradio-4.x/basic/block2.hpp>


int main() {
    std::cout << "=== List All Registered Blocks ===\n";
    auto& registry = gr::BlockRegistry::instance();
    auto  types    = registry.listTypes();
    for (auto& t : types) {
        std::cout << "  " << t << "\n";
    }

    auto block = registry.create(typeid(gr::basic::Block1<float>).name());
    if (block) {
        block->doWork();
    } else {
        std::cout << "ERROR: Block1<float> not registered!\n";
    }

    auto block2 = registry.create(typeid(gr::basic::Block2<int, double>).name());
    if (block2) {
        block2->doWork();
    } else {
        std::cout << "ERROR: Block2<int,double> not registered!\n";
    }

    return 0;
}
