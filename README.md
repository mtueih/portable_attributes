# Portable Attributes

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C11%2FC17%2FC23-blue.svg)](https://en.cppreference.com/w/c)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

Portable C function attributes for GCC, Clang, and MSVC.

## Overview

`portable_attributes` is a header-only C library that provides cross-compiler attribute abstraction for GCC, Clang, and MSVC. It allows developers to use compiler-specific attributes with a unified API, ensuring code portability across different compilers and platforms.

The library supports C11, C17, and C23 standards and automatically detects the compiler being used to provide the appropriate attribute syntax or fallback to empty macros when a feature is unavailable.

## Features

- **Cross-compiler compatibility**: Works seamlessly with GCC, Clang, and MSVC
- **Standard compliance**: Supports C11, C17, and C23 standards
- **Header-only**: No compilation required, just include the header
- **Comprehensive attribute coverage**: Includes attributes for:
  - Return value constraints (`NODISCARD`, `RETURNS_NONNULL`, etc.)
  - Parameter constraints (`NONNULL`, `ACCESS`, `SENTINEL`, etc.)
  - Side-effect/optimization hints (`PURE`, `CONST`, `MALLOC_LIKE`, etc.)
  - Format checking (`FORMAT_PRINTF`, `FORMAT_SCANF`)
  - Control flow (`NORETURN`, `FALLTHROUGH`, `UNREACHABLE`, etc.)
  - Inlining/performance (`ALWAYS_INLINE`, `NOINLINE`, `HOT`, `COLD`, etc.)
  - Visibility control (`EXPORT`, `IMPORT`, `HIDDEN`)
  - Diagnostics/deprecation (`DEPRECATED`, `ATTR_ERROR`, `ATTR_WARNING`)
  - Alignment/layout (`ALIGNED`, `PACKED`, `SECTION`)
  - Miscellaneous (`WEAK`, `USED`, `UNUSED`, `CONSTRUCTOR`, `DESTRUCTOR`, etc.)

## Requirements

- C compiler supporting C11 or later (GCC, Clang, MSVC)
- CMake 3.14 or higher (for installation)

## Installation

### Using CMake

```bash
# Clone the repository
git clone https://github.com/mtueih/portable_attributes.git
cd portable_attributes

# Create build directory
mkdir build && cd build

# Configure and install
cmake ..
cmake --build .
cmake --install .
```

### Manual Installation

Simply copy `include/portable_attributes.h` to your project's include directory and include it in your source files.

### Using as a Submodule

```bash
# Add as a git submodule
git submodule add https://github.com/mtueih/portable_attributes.git deps/portable_attributes

# In your CMakeLists.txt
add_subdirectory(deps/portable_attributes)
target_link_libraries(your_target PRIVATE portable_attributes::portable_attributes)
```

## Usage

Include the header in your C source files:

```c
#include <portable_attributes.h>

// Example: Function that should not have its return value ignored
NODISCARD int calculate_value(void);

// Example: Function that never returns
NORETURN void fatal_error(const char* message);

// Example: Function with format string checking
void log_message(FORMAT_PRINTF(1, 2) const char* format, ...);

// Example: Force inline function
ALWAYS_INLINE int max(int a, int b) {
    return (a > b) ? a : b;
}

// Example: Structure with specific alignment
typedef struct {
    int x;
    int y;
} ALIGNED(16) Point;
```

### Integration with CMake

If you've installed the library via CMake, you can use it in your project like this:

```cmake
find_package(portable_attributes REQUIRED)
target_link_libraries(your_target PRIVATE portable_attributes::portable_attributes)
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- Thanks to all contributors who help improve this library
- Inspired by the need for cross-platform C development with consistent attribute usage
