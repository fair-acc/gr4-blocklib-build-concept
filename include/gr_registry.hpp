#ifndef GR_REGISTRY_HPP
#define GR_REGISTRY_HPP

#include <format>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_MYPLUGIN
#define GR_API __declspec(dllexport)
#else
#define GR_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define GR_API __attribute__((visibility("default")))
#else
#define GR_API
#endif
#endif

#include<iostream>
#include<format>

namespace gr {

struct GR_API IBlock {
    virtual ~IBlock()     = default;
    virtual void doWork() = 0;
};

class GR_API BlockRegistry {
public:
    static BlockRegistry& instance();
    using CreatorFunc = std::unique_ptr<IBlock> (*)();

    void registerType(std::string typeName, CreatorFunc func) {
        registry_[std::move(typeName)] = func;
    }

    std::unique_ptr<IBlock> create(const std::string& typeName) const {
        if (auto it = registry_.find(typeName); it != registry_.end()) {
            return it->second();
        }
        return nullptr;
    }

    std::vector<std::string> listTypes() const {
        std::vector<std::string> types;
        types.reserve(registry_.size());
        for (const auto& kv : registry_) {
            types.push_back(kv.first);
        }
        return types;
    }

private:
    std::map<std::string, CreatorFunc> registry_;
};

template<typename BlockType>
bool registerBlock(std::string_view customName = {}) {
    BlockRegistry::instance().registerType(customName.empty() ? std::string(typeid(BlockType).name()) : std::string(customName), +[]() { return std::unique_ptr<IBlock>(new BlockType()); });
    return true;
}

#define GR_REGISTER_BLOCK(...) /* Marker macro for parse_registrations */

} // namespace gr

#endif // GR_REGISTRY_HPP