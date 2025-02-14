# **GNU Radio 4.0 Block Registry & Plugin System - PoC**

A minimal proof-of-concept prototype for building GR4 module plugin-registry (needed for **runtime polymorphism**):

- **explicit registration macros** (`GR_REGISTER_BLOCK(...)`) that generate plugin code at build time. Example syntax:
  ```cpp
  template<typename T> struct AlgoImpl1 {};
  template<typename T> struct AlgoImpl2 {};

  // register block with arbitrary NTTPs (here: 3UZ) and expand T in [float,double], U in [short, int, long, long long]
  GR_REGISTER_BLOCK(gr::basic::BlockN, ([T], [U], 3UZ), [ float, double ], [ short, int, long, long long ])
  // register block with arbitrary NTTPs (here: 4UZ) and expand T for [short], U for [short] only
  GR_REGISTER_BLOCK("CustomBlockNameN", gr::basic::BlockN, ([T], [U], 4UZ, gr::basic::AlgoImpl2<[T]>), [ short ], [ short ])
  
  template<typename T, typename U, std::size_t N, typename Alog = AlgoImpl1<T>>
  struct BlockN : public gr::IBlock { /* .. */  };

  } // namespace gr::basic
  
  /// other macro variants options:
  ///   GR_REGISTER_BLOCK("MyBlockName", gr::basic::Block1, ([T], [U]), [ float, double ], [int])
  ///   GR_REGISTER_BLOCK(gr::basic::Block0)
  ///   GR_REGISTER_BLOCK("blockN.hpp", gr::basic::BlockN, ([T],[U],3UZ,SomeAlgo<[T]>), [ short, int], [double])
  ```
- **incremental/parallel** builds, each block’s `.hpp` producing a dedicated `.cpp` for runtime registration.
- **shared library** (or multiple libraries) containing all generated instantiations, so other code can create blocks polymorphically at runtime.

---

## **Project Structure Overview**

```
my_project/
├─ CMakeLists.txt           <-- Top-level build logic, parser tool, aggregator libs
├─ tools/
│   ├─ CMakeLists.txt       <-- external project build file (needs to be invoked before main build)
│   └─ parse_registrations.cpp   <-- The custom parser that scans each .hpp for GR_REGISTER_BLOCK macros
├─ include/
│   └─ gr_registry.hpp      <-- Global registry interface & marker macro
├─ src/
│   ├─ CMakeLists.txt       <-- (optional) compiles e.g. gr_registry.cpp
│   └─ gr_registry.cpp      <-- actual definition of BlockRegistry::instance()
├─ blocks/
│   ├─ CMakeLists.txt       <-- enumerates submodules (basic, math, etc.)
│   ├─ basic/
│   │   ├─ CMakeLists.txt   <-- lists basic block .hpp files, calls create_block_library(grBasic)
│   │   ├─ include/
│   │   │   └─ gnuradio-4.x/basic/block{0,1,2,3,N}.hpp  <-- block templates
│   │   └─ src/ (...)       <-- optional .cpp or sub-tests
│   ├─ math/
│   │   ├─ CMakeLists.txt   <-- lists math block .hpp files, calls create_block_library(grMath)
│   │   └─ include/ (...etc.)
├─ example/
│   ├─ CMakeLists.txt       <-- builds a test executable linking block libraries
│   └─ main.cpp             <-- usage example: enumerates & creates blocks
└─ build/ (created by CMake)
    ├─ generated_plugins/
    │   ├─ grBasic/   <-- one .cpp file per block header
    │   ├─ grMath/
    │   └─ ...
    └─ ...
```

### **Key Components**

1. **`parse_registrations.cpp`**
    - scans exactly one header file for lines with `GR_REGISTER_BLOCK(...)`.
    - produces `.cpp` containing calls to `registerBlock<...>()`.
    - macros must be on single lines.
    - single or multiple `.cpp` generation is controlled via the command-line argument `--spit` or `-s`, or via the
      cmake option `-DENABLE_SPLIT=<ON|OFF>` (default: on).
    - Example parser output:
    ```cpp
    parsing header: '<project root>/gr4-blocklib-build-concept/blocks/basic/include/gnuradio-4.x/basic/blockN.hpp' -> '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic'  split: Yes 
        found macro on line 17: 'GR_REGISTER_BLOCK(gr::basic::BlockN, ([T], [U], 3UZ), [ float, double ], [ short, int, long, long long ])'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_0.cpp'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_1.cpp'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_2.cpp'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_3.cpp'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_4.cpp'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_5.cpp'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_6.cpp'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_0_7.cpp'

        found macro on line 19: 'GR_REGISTER_BLOCK("CustomBlockNameN", gr::basic::BlockN, ([T], [U], 4UZ, gr::basic::AlgoImpl2<[T]>), [ short ], [ short ])'
        => Generating file: '<project root>/gr4-blocklib-build-concept/build/generated_plugins/grBasic/blockN_1_0.cpp'

    parse_registrations: Wrote 9 file(s) for 2 macro definition(s).
    ```
    - **known issue**: if a macro or block template signature is modified the `cmake ..` has to be re-invoked.

2. **`gr_registry.hpp`** *N.B. this is a mock MVP implementation*
    - declares `BlockRegistry`, a global map from string to factory function.
    - declares `GR_REGISTER_BLOCK(...)` as an empty marker macro for the parser.

3. **`gr_registry.cpp`**
    - Defines `BlockRegistry::instance()`, storing a `static BlockRegistry`.

4. **`create_block_library(...)`** (CMake function)
    - For each `.hpp`, calls `parse_registrations` => a generated `.cpp`.
    - Builds a shared library from all generated `.cpp`s plus optional user `.cpp` sources.

5. **`main.cpp`**
    - Demonstrates linking the library and creating blocks at runtime by type.

---

## **Usage & Build System**

1. **Configure** & **Build**:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
    - top-level `CMakeLists.txt` script manually configures and builds the the `parse_registrations` tool from the
      `tools/` directory directly into the build directory (e.g. `build/tools_build`).
    - for each block subdirectory, CMake loops over block headers, generating a `.cpp` per header.
    - a final `.so` library (e.g. `libgrBasic.so`, `libgrMath.so`) is created with all instantiations.
    - the `example/test_registry` and other executables are built, linking those libs.

2. **Incremental & Parallel**:
    - Each block header => one custom command => generates `.cpp`s, which can be compiled independently.
    - If, for example, `block2.hpp` is modified, only the corresponding `.cpp`s are regenerated and recompiled.

3. **Runtime Polymorphism**:
    - The global registry stores factory functions keyed by a **custom name** (or simplified `typeid(...)` fallback).
    - `listTypes()` enumerates all registered blocks.
    - `create("MyBlock<float>")` or `create(...)` returns a new instance of that block’s type, letting you handle them via a common interface (`IBlock`).

4. **`GR_REGISTER_BLOCK(BaseName, TemplateName, (paramPack)?, [ expansions ]...)`**:
    - Must be **one line**.
    - The parser extracts the baseName, expansions, placeholders, etc.
    - Produces code like `registerBlock<Template< float >>("BaseName<float>")` for each expansion.

---

## **License & Contributing**

- This code is provided as-is for demonstration & discussion in the **GNU Radio 4.0** community.
- Feel free to open issues, pull requests, propose enhancements such as a more robust build logic.