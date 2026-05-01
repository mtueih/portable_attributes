# portable_attributes

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C89/C95/C99/C11/C17/C23-blue.svg)](https://cppreference.cn/w/c)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

C 语言*跨平台*、*跨编译器*、*全标准兼容* **属性抽象封装库**。

## 简介

`portable_attributes` 是一个纯头文件的 C
语言库，将编译器特有的属性（attribute）和注解封装成统一的宏，使开发者可以用一致的方式表达函数特性、编译期约束和优化提示。所有宏在不支持的环境下都会安全地展开为空，不会导致编译错误。

该库兼容所有 C 标准，能够自动检测当前使用的编译器和 C 标准版本，并提供相应的属性语法，或在特性不可用时安全地回退为空宏定义。

## 特性

- **跨编译器兼容**：支持的编译器：
    - GCC；
    - Clang / Apple Clang；
    - MSVC（Microsoft Visual C++）；
    - Intel C++（icc / icx）；
    - EDG eccp；
    - NVIDIA HPC C++（原 PGI）；
    - NVIDIA nvcc；
    - Cray Compiler。
- **全标准兼容**：支持 C89、C95、C99、C11、C17 和 C23 标准。
- **纯头文件**：无需编译，直接包含即可使用。
- **全面的属性覆盖**：涵盖返回值约束、参数约束、函数纯度与副作用、格式检查、控制流、内联与性能、可见性与链接、诊断与弃用、对齐与布局等多个类别。

## 编译 & 安装（CMake）

### 使用 CMake 安装

**环境要求**：

- CMake 3.14 或更高版本（用于安装）。

```bash
# 克隆仓库
git clone https://github.com/mtueih/portable_attributes.git
cd portable_attributes

# 创建构建目录
mkdir build && cd build

# 配置并安装
cmake ..
cmake --build .
cmake --install .
```

### 手动安装

只需将 `include/portable_attributes.h` 复制到项目的包含目录中，并在源文件中包含它。

## 使用

### CMake 集成

如果通过 CMake 安装了该库，可以在项目中这样使用：

```cmake
find_package(portable_attributes REQUIRED)
target_link_libraries(your_target PRIVATE portable_attributes::portable_attributes)
```

### 作为 Git 子模块使用

```bash
# 添加为 git 子模块
git submodule add https://github.com/mtueih/portable_attributes.git deps/portable_attributes

# 在你的 CMakeLists.txt 中
add_subdirectory(deps/portable_attributes)
target_link_libraries(your_target PRIVATE portable_attributes::portable_attributes)
```

## 示例

```c
#include <portable_attributes.h>

// 示例：不应忽略返回值的函数
NODISCARD int calculate_value(void);

// 示例：永不返回的函数
NORETURN void fatal_error(const char* message);

// 示例：带格式字符串检查的函数
void log_message(const char* format, ...) FORMAT_PRINTF(1, 2);

// 示例：强制内联函数
ALWAYS_INLINE int max(int a, int b) {
    return (a > b) ? a : b;
}

// 示例：具有特定对齐方式的结构体
typedef struct {
    int x;
    int y;
} ALIGNED(16) Point;
```

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 贡献

欢迎贡献！请随时提交 Pull Request。

## 致谢

- 感谢所有帮助改进此库的贡献者！
- 灵感来源于跨平台 C 开发中对一致属性使用的需求。
