#include "gr_registry.hpp"

namespace gr {

BlockRegistry& BlockRegistry::instance() {
    static BlockRegistry reg;
    return reg;
}

} // namespace gr
