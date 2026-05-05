# cattrs

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C89/C95/C99/C11/C17/C23-blue.svg)](https://cppreference.cn/w/c)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

C 语言*跨平台*、*跨编译器*、*全标准兼容* **属性抽象封装库**。

## 介绍

`cattrs` 是一个纯头文件的 C
语言库，将编译器特有的属性（attribute）和注解封装成统一的宏，使开发者可以用一致的方式表达函数特性、编译期约束和优化提示。所有宏在不支持的环境下都会安全地展开为空，不会导致编译错误。

该库兼容所有 C 标准，能够自动检测当前使用的编译器和 C 标准版本，并提供相应的属性语法，或在特性不可用时安全地回退为空宏定义。

### 设计原则

| 优先级 | 策略                                        |
| ------ | ------------------------------------------- |
| **1**  | 优先使用 C23 `[[...]]` 标准语法             |
| **2**  | 其次 C++11+ `[[...]]` 语法（混合项目）      |
| **3**  | 再其次 GCC `__attribute__((...))`           |
| **4**  | 再其次 MSVC `__declspec(...)`               |
| **5**  | 再其次编译器专用语法（IAR pragma、Keil 等） |
| **6**  | 最后安全退化为空宏                          |

### 支持范围

| 维度         | 覆盖                                                         |
| ------------ | ------------------------------------------------------------ |
| **C 标准**   | C89 → C99 → C11 → C17 → C23                                  |
| **C++ 标准** | C++98 → C++11 → C++14 → C++17 → C++20 → C++23                |
| **编译器**   | GCC、Clang、MSVC、Intel ICC/ICX、TinyCC、SDCC、ARM/Keil、IAR、Oracle/Sun、PGI/NVIDIA HPC、IBM  XL、Embarcadero/Borland、OpenWatcom、Digital Mars |
| **退化安全** | 所有宏在不支持时展开为空，不会导致编译错误                   |

### 宏命名规范

所有宏以 `CAT_` 前缀，按功能分类：

| 类别      | 前缀示例                                                     |
| --------- | ------------------------------------------------------------ |
| 标准属性  | `CAT_NORETURN`, `CAT_DEPRECATED`, `CAT_NODISCARD`            |
| 对齐      | `CAT_ALIGNED`, `CAT_ASSUME_ALIGNED`, `CAT_ALLOC_ALIGN`       |
| 函数行为  | `CAT_CONST`, `CAT_PURE`, `CAT_MALLOC`, `CAT_NONNULL`         |
| 内联控制  | `CAT_ALWAYS_INLINE`, `CAT_NOINLINE`                          |
| 可见性    | `CAT_VISIBILITY`, `CAT_HIDDEN`, `CAT_WEAK`, `CAT_DLLIMPORT`  |
| 构造/析构 | `CAT_CONSTRUCTOR`, `CAT_DESTRUCTOR`                          |
| 布局      | `CAT_PACKED`, `CAT_CLEANUP`, `CAT_MAY_ALIAS`                 |
| 工具      | `CAT_LIKELY`, `CAT_UNLIKELY`, `CAT_UNREACHABLE`, `CAT_STATIC_ASSERT` |

## 编译 & 安装

### 使用 CMake 安装

**环境要求**：

- CMake 3.14 或更高版本。

```bash
# 克隆仓库
git clone https://github.com/mtueih/cattrs.git
cd cattrs

# 创建构建目录
mkdir build && cd build

# 配置并安装
cmake ..
cmake --build .
cmake --install .
```

### 手动安装

只需将 `include/cattrs.h` 复制到项目的包含目录中，并在源文件中包含它。

## 使用

### CMake 集成

如果通过 CMake 安装了该库，可以在项目中这样使用：

```cmake
find_package(cattrs REQUIRED)
target_link_libraries(your_target PRIVATE cattrs::cattrs)
```

### 作为 Git 子模块使用

```bash
# 添加为 git 子模块
git submodule add https://github.com/mtueih/cattrs.git deps/cattrs

# 在你的 CMakeLists.txt 中
add_subdirectory(deps/cattrs)
target_link_libraries(your_target PRIVATE cattrs::cattrs)
```

## 示例

```c
#include "cattrs.h"

/* 1. 分配器：返回 64 字节对齐内存，不返回 NULL */
CAT_MALLOC CAT_ALLOC_SIZE(1) CAT_ALLOC_ALIGN(1)
CAT_ASSUME_ALIGNED(64) CAT_RETURNS_NONNULL
void* my_alloc(size_t size, size_t align);

/* 2. 纯数学函数 */
CAT_CONST CAT_ALWAYS_INLINE
double fast_sqrt(double x) {
    return /* ... */;
}

/* 3. 热路径循环 */
CAT_HOT CAT_NOINLINE
void process_data(float* CAT_RESTRICT data, size_t n) {
    float* aligned = CAT_BUILTIN_ASSUME_ALIGNED(data, 64);
    for (size_t i = 0; i < n; i++) {
        aligned[i] *= 2.0f;
    }
}

/* 4. 弃用旧 API */
CAT_DEPRECATED("Use new_api() instead")
void old_api(void);

/* 5. 编译时断言 */
CAT_STATIC_ASSERT(sizeof(int) == 4, "int must be 32-bit");

/* 6. 分支预测 */
if (CAT_UNLIKELY(ptr == NULL)) {
    CAT_UNREACHABLE(); /* 已知不会走到 */
}

/* 7. 线程局部存储 */
CAT_THREAD_LOCAL static int counter = 0;
```

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 贡献

欢迎贡献！请随时提交 Pull Request。

## 致谢

- 感谢所有帮助改进此库的贡献者！
- 灵感来源于跨平台 C 开发中对一致属性使用的需求。
