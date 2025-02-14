#ifndef BLOCKN_HPP
#define BLOCKN_HPP
#include "gr_registry.hpp"

#include <cstddef>
#include <iostream>

namespace gr :: basic {

template<typename T>
struct AlgoImpl1 {};

template<typename T>
struct AlgoImpl2 {};

// register block with arbitrary NTTPs (here: 3UZ) and expand T in [float,double], U in [short, int, long, long long]
GR_REGISTER_BLOCK(gr::basic::BlockN, ([T], [U], 3UZ), [ float, double ], [ short, int, long, long long ])
// register block with arbitrary NTTPs (here: 4UZ) and expand T for [short], U for [short] only
GR_REGISTER_BLOCK("CustomBlockNameN", gr::basic::BlockN, ([T], [U], 4UZ, gr::basic::AlgoImpl2<[T]>), [ short ], [ short ])

template<typename T, typename U, std::size_t N, typename Alog = AlgoImpl1<T>>
struct BlockN : public gr::IBlock {
    void doWork() override { std::cout << "BlockN<" << typeid(T).name() << ", " << typeid(U).name() << ", " << N << "> doWork()\n"; }
};

} // namespace gr::basic

#endif // #ifndef BLOCKN_HPP
