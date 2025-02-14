#ifndef MATH_BLOCK0_HPP
#define MATH_BLOCK0_HPP
#include <gr_registry.hpp>

#include <iostream>

namespace gr::math {

// register MathBlock0<T> for T in [float, double]
GR_REGISTER_BLOCK(gr::math::MathBlock0, [T], [ float, double ])

template<typename T>
struct MathBlock0 : public gr::IBlock {
    void doWork() override { std::cout << "MathBlock0<" << typeid(T).name() << "> doWork()\n"; }
};

} // namespace gr::math

#endif // #ifndef MATH_BLOCK0_HPP
