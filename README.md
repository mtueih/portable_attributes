# attrs

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C89/C95/C99/C11/C17/C23-blue.svg)](https://cppreference.cn/w/c)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

C 语言*跨平台*、*跨编译器*、*全标准兼容* **属性抽象封装库**。

## 介绍

`attrs` 是一个**纯 C 语言**、*跨平台*、*跨编译器*、*全标准兼容*的**属性抽象封装库**。

### 设计原则

| 优先级   | 策略                             |
|-------|--------------------------------|
| **1** | 优先使用 C23 `[[...]]` 标准语法。       |
| **2** | 其次 GCC `__attribute__((...))`。 |
| **3** | 再其次 MSVC `__declspec(...)`。    |
| **4** | 再其次编译器专用语法（IAR pragma、Keil 等）。 |
| **5** | 最后安全退化为空宏。                     |

### 支持范围

| 维度       | 覆盖                                                                                                                                   |
|----------|--------------------------------------------------------------------------------------------------------------------------------------|
| **C 标准** | C89 → C95 → C99 → C11 → C17 → C23。                                                                                                   |
| **编译器**  | GCC、Clang、MSVC、Intel ICC/ICX、TinyCC、SDCC、ARM/Keil、IAR、Oracle/Sun、PGI/NVIDIA HPC、IBM  XL、Embarcadero/Borland、OpenWatcom、Digital Mars。 |
| **退化安全** | 所有宏在不支持时展开为空，不会导致编译错误。                                                                                                               |

### 宏命名规范

所有宏以 `ATTRS_` 前缀，按功能分类：

| 类别    | 前缀示例                                                                          |
|-------|-------------------------------------------------------------------------------|
| 标准属性  | `ATTRS_NORETURN`, `ATTRS_DEPRECATED`, `ATTRS_NODISCARD`。                      |
| 对齐    | `ATTRS_ALIGNED`, `ATTRS_ASSUME_ALIGNED`, `ATTRS_ALLOC_ALIGN`。                 |
| 函数行为  | `ATTRS_CONST`, `ATTRS_PURE`, `ATTRS_MALLOC`, `ATTRS_NONNULL`。                 |
| 内联控制  | `ATTRS_ALWAYS_INLINE`, `ATTRS_NOINLINE`。                                      |
| 可见性   | `ATTRS_VISIBILITY`, `ATTRS_HIDDEN`, `ATTRS_WEAK`, `ATTRS_DLLIMPORT`。          |
| 构造/析构 | `ATTRS_CONSTRUCTOR`, `ATTRS_DESTRUCTOR`。                                      |
| 布局    | `ATTRS_PACKED`, `ATTRS_CLEANUP`, `ATTRS_MAY_ALIAS`。                           |
| 工具    | `ATTRS_LIKELY`, `ATTRS_UNLIKELY`, `ATTRS_UNREACHABLE`, `ATTRS_STATIC_ASSERT`。 |

## 安装

**环境要求**：

- CMake 3.14 或更高版本。

```bash
# 克隆仓库。
git clone https://github.com/mtueih/attrs.git
cd attrs

# 创建构建目录。
mkdir build && cd build

# 配置并安装。
cmake ..
cmake --build .
cmake --install .
```

## 使用

如果通过 CMake 安装了该库，可以在项目中这样使用：

```cmake
find_package(attrs REQUIRED)
# 由于此项目头文件通常被 PUBLIC 头文件使用，因此使用 PUBLIC 链接该库以确保头文件能正确传递。
target_link_libraries(your_target PUBLIC attrs::attrs)
```

## 示例

```c
#include <attrs.h>

/* 1. 分配器：返回 64 字节对齐内存，不返回 NULL。 */
ATTRS_MALLOC ATTRS_ALLOC_SIZE(1) ATTRS_ALLOC_ALIGN(1)
ATTRS_ASSUME_ALIGNED(64) ATTRS_RETURNS_NONNULL
void* my_alloc(size_t size, size_t align);

/* 2. 纯数学函数。 */
ATTRS_CONST ATTRS_ALWAYS_INLINE
double fast_sqrt(double x) {
    return /* ... */;
}

/* 3. 热路径循环。 */
ATTRS_HOT ATTRS_NOINLINE
void process_data(float* ATTRS_RESTRICT data, size_t n) {
    float* aligned = ATTRS_BUILTIN_ASSUME_ALIGNED(data, 64);
    for (size_t i = 0; i < n; i++) {
        aligned[i] *= 2.0f;
    }
}

/* 4. 弃用旧 API。 */
ATTRS_DEPRECATED("Use new_api() instead")
void old_api(void);

/* 5. 编译时断言。 */
ATTRS_STATIC_ASSERT(sizeof(int) == 4, "int must be 32-bit");

/* 6. 分支预测。 */
if (ATTRS_UNLIKELY(ptr == NULL)) {
    ATTRS_UNREACHABLE(); /* 已知不会走到。 */
}

/* 7. 线程局部存储。 */
ATTRS_THREAD_LOCAL static int counter = 0;
```

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 贡献

欢迎贡献！请随时提交 Pull Request。

## 致谢

- 感谢所有帮助改进此库的贡献者！
- 灵感来源于跨平台 C 开发中对一致属性使用的需求。
