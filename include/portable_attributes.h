#ifndef PORTABLE_ATTRIBUTES_H
#define PORTABLE_ATTRIBUTES_H

/**
 * @file portable_attributes.h
 * @brief C 语言跨编译器、跨标准属性抽象封装。
 *
 * @details
 * 本头文件将编译器特有的属性（attribute）和注解封装成统一的宏，使开发者
 * 可以用一致的方式表达函数特性、编译期约束和优化提示。所有宏在不支持的环境下
 * 都会安全地展开为空，不会导致编译错误。
 *
 * @par 支持的标准
 *   C89、C95、C99、C11、C17、C23
 *
 * @par 支持的编译器
 *   - GCC
 *   - Clang / Apple Clang
 *   - MSVC（Microsoft Visual C++）
 *   - Intel C++（icc / icx）
 *   - EDG eccp
 *   - NVIDIA HPC C++（原 PGI）
 *   - NVIDIA nvcc（设备代码属性见具体说明）
 *   - Cray Compiler
 *
 * @par 属性分类
 *   - @ref group_return   "返回值约束"
 *   - @ref group_params   "参数约束"
 *   - @ref group_pure     "函数纯度与副作用"
 *   - @ref group_format   "格式检查"
 *   - @ref group_control  "控制流"
 *   - @ref group_inline   "内联与性能"
 *   - @ref group_visibility "可见性与链接"
 *   - @ref group_diagnostics "诊断与弃用"
 *   - @ref group_layout   "对齐与布局"
 *   - @ref group_misc     "杂项"
 *
 * @par 使用示例
 * @code{.c}
 * #include <portable_attributes.h>
 *
 * // 不应忽略返回值的函数
 * NODISCARD int calculate_value(void);
 *
 * // 永不返回的函数
 * NORETURN void fatal_error(const char* message);
 *
 * // 带格式字符串检查的函数
 * void log_message(const char* format, ...) FORMAT_PRINTF(1, 2);
 *
 * // 强制内联函数
 * ALWAYS_INLINE int max(int a, int b) {
 *     return (a > b) ? a : b;
 * }
 *
 * // 示例：具有特定对齐方式的结构体
 * typedef struct {
 *     int x;
 *     int y;
 * } ALIGNED(16) Point;
 * @endcode
 *
 * @note
 *   - 本头文件不包含任何实现，可直接包含使用。
 *   - 部分属性在 C89 中由于关键字限制（如 `inline`）会安全回退为空。
 *
 * @author portable_attributes contributors
 * @date   2024
 * @version 2.1.0
 * @license MIT
 */

/*──────────────────────────── 编译器与环境检测 ────────────────────────────*/

/* 检测是否属于支持 __attribute__ 的编译器家族（GCC、Clang、Intel、PGI/NVHPC、Cray、EDG 等） */
#if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER) ||  \
    defined(__ICC) || defined(__ICL) || defined(__NVCOMPILER) ||             \
    defined(__PGI) || defined(_CRAYC) || defined(__EDG__)
#  define PORTABLE_ATTR_GNUC_FAMILY 1
#else
#  define PORTABLE_ATTR_GNUC_FAMILY 0
#endif

/* 检测原生 MSVC（排除 Clang/Intel 伪装的情况） */
#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER) && !defined(__ICL)
#  define PORTABLE_ATTR_MSVC 1
#else
#  define PORTABLE_ATTR_MSVC 0
#endif

/* __has_attribute 支持检测 */
#if defined(__has_attribute)
#  define PORTABLE_ATTR_HAS_GNU(x) __has_attribute(x)
#else
#  define PORTABLE_ATTR_HAS_GNU(x) 0
#endif

/* __has_c_attribute 支持检测（用于标准 C23 属性） */
#if defined(__has_c_attribute)
#  define PORTABLE_ATTR_HAS_C(x) __has_c_attribute(x)
#else
#  define PORTABLE_ATTR_HAS_C(x) 0
#endif

/* 变参宏支持检测：C99 原生支持，绝大多数编译器在 C89 模式下亦支持作为扩展 */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    defined(__GNUC__) || defined(__clang__) ||                     \
    defined(_MSC_VER) || defined(__ICC) || defined(__ICL) ||       \
    defined(__NVCOMPILER) || defined(__PGI) ||                     \
    defined(_CRAYC) || defined(__EDG__)
#  define PORTABLE_ATTR_VARIADIC_MACROS 1
#else
#  define PORTABLE_ATTR_VARIADIC_MACROS 0
#endif

/* 逗号吞噬扩展（##__VA_ARGS__）：GNU 家族支持，MSVC 从 VS2010 起类似支持，此处保守处理 */
#if PORTABLE_ATTR_GNUC_FAMILY || (defined(_MSC_VER) && _MSC_VER >= 1600)
#  define PORTABLE_ATTR_VA_OPT_COMMA(...) , ##__VA_ARGS__
#else
#  define PORTABLE_ATTR_VA_OPT_COMMA(...) , __VA_ARGS__
#endif

/* C 标准版本检测 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define PORTABLE_ATTR_C23 1
#  define PORTABLE_ATTR_C17 1
#  define PORTABLE_ATTR_C11 1
#  define PORTABLE_ATTR_C99 1
#  define PORTABLE_ATTR_C89 0
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201710L
#  define PORTABLE_ATTR_C23 0
#  define PORTABLE_ATTR_C17 1
#  define PORTABLE_ATTR_C11 1
#  define PORTABLE_ATTR_C99 1
#  define PORTABLE_ATTR_C89 0
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define PORTABLE_ATTR_C23 0
#  define PORTABLE_ATTR_C17 0
#  define PORTABLE_ATTR_C11 1
#  define PORTABLE_ATTR_C99 1
#  define PORTABLE_ATTR_C89 0
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define PORTABLE_ATTR_C23 0
#  define PORTABLE_ATTR_C17 0
#  define PORTABLE_ATTR_C11 0
#  define PORTABLE_ATTR_C99 1
#  define PORTABLE_ATTR_C89 0
#else
#  define PORTABLE_ATTR_C23 0
#  define PORTABLE_ATTR_C17 0
#  define PORTABLE_ATTR_C11 0
#  define PORTABLE_ATTR_C99 0
#  define PORTABLE_ATTR_C89 1
#endif


/*──────────────────────────── 1. 返回值约束 ────────────────────────────────*/
/**
 * @defgroup group_return 返回值约束
 * @brief 向编译器和调用者传达函数返回值的语义，帮助发现 misuse。
 * @{
 */

/**
 * @brief 标记函数返回值不可丢弃。
 *
 * 若调用者忽略了返回值，编译器将发出警告。
 * 优先使用 C23 `[[nodiscard]]`，其次 GNU `warn_unused_result`，
 * 或 MSVC `_Check_return_`。
 */
#if PORTABLE_ATTR_HAS_C(nodiscard)
#  define NODISCARD [[nodiscard]]
#elif PORTABLE_ATTR_HAS_GNU(warn_unused_result) || PORTABLE_ATTR_GNUC_FAMILY
#  define NODISCARD __attribute__((warn_unused_result))
#elif PORTABLE_ATTR_MSVC
#  if !defined(_Check_return_)
#    include <sal.h>
#  endif
#  define NODISCARD _Check_return_
#else
#  define NODISCARD
#endif

/**
 * @brief 承诺返回的指针永远不会是 NULL。
 *
 * 用于静态分析和优化（如消除不必要的 NULL 检查）。
 */
#if PORTABLE_ATTR_HAS_GNU(returns_nonnull) || PORTABLE_ATTR_GNUC_FAMILY
#  define RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#  define RETURNS_NONNULL
#endif

/**
 * @brief 函数可能多次返回（类似 setjmp、fork）。
 */
#if PORTABLE_ATTR_HAS_GNU(returns_twice) || PORTABLE_ATTR_GNUC_FAMILY
#  define RETURNS_TWICE __attribute__((returns_twice))
#else
#  define RETURNS_TWICE
#endif

/** @} */


/*──────────────────────────── 2. 参数约束 ────────────────────────────────*/
/**
 * @defgroup group_params 参数约束
 * @brief 描述函数参数的属性，例如不可为空、访问模式、哨兵等。
 * @{
 */

/**
 * @brief 指定不能为 NULL 的指针参数。
 *
 * @param ... 参数索引（从 1 开始），若无参数则使用 NONNULL_ALL。
 */
#if PORTABLE_ATTR_VARIADIC_MACROS
#  define NONNULL(...)        __attribute__((nonnull(__VA_ARGS__)))
#  define NONNULL_ALL         __attribute__((nonnull))
#else
/* 严格 C89 下不支持变参宏，仅提供 NONNULL_ALL */
#  define NONNULL_ALL         __attribute__((nonnull))
/* 若使用 NONNULL(1,2) 将因未定义宏而报错，引导用户改用 NONNULL_ALL */
#endif

/**
 * @brief 描述通过指针参数访问内存的模式。
 *
 * @param mode 访问模式（如 read_only、read_write）
 * @param idx  参数索引
 * @param ...  可选参数
 */
#if PORTABLE_ATTR_VARIADIC_MACROS
#  define ACCESS(mode, idx, ...) \
     __attribute__((access(mode, idx PORTABLE_ATTR_VA_OPT_COMMA(__VA_ARGS__))))
#else
#  define ACCESS(mode, idx, ...) /* 降级 */
#endif

/* MSVC SAL 注解 – 仅在原生 MSVC 下有意义 */
#if PORTABLE_ATTR_MSVC
#  if !defined(_In_)
#    include <sal.h>
#  endif
#  define IN_NONNULL  _In_
#  define OUT_NONNULL _Out_
#  define INOUT       _Inout_
#  define IN_OPT      _In_opt_
#else
#  define IN_NONNULL
#  define OUT_NONNULL
#  define INOUT
#  define IN_OPT
#endif

/**
 * @brief 标记可变参数列表以 NULL 哨兵结尾。
 */
#if PORTABLE_ATTR_HAS_GNU(sentinel) || PORTABLE_ATTR_GNUC_FAMILY
#  define SENTINEL __attribute__((sentinel))
#else
#  define SENTINEL
#endif

/**
 * @brief 指示某个参数本身是格式字符串（用于间接格式化函数）。
 *
 * @param idx 参数索引
 */
#if PORTABLE_ATTR_HAS_GNU(format_arg) || PORTABLE_ATTR_GNUC_FAMILY
#  define FORMAT_ARG(idx) __attribute__((format_arg(idx)))
#else
#  define FORMAT_ARG(idx)
#endif

/**
 * @brief 标识哪个参数决定了所分配内存的对齐方式。
 *
 * @param idx 参数索引
 */
#if PORTABLE_ATTR_HAS_GNU(alloc_align)
#  define ALLOC_ALIGN(idx) __attribute__((alloc_align(idx)))
#else
#  define ALLOC_ALIGN(idx)
#endif

/**
 * @brief 告知编译器某个指针已经具有给定的对齐方式。
 *
 * @param n 对齐字节数
 */
#if PORTABLE_ATTR_HAS_GNU(assume_aligned)
#  define ASSUME_ALIGNED(n) __attribute__((assume_aligned(n)))
#else
#  define ASSUME_ALIGNED(n)
#endif

/**
 * @brief 便携的 restrict 限定符。
 *
 * C89 不支持 `restrict`，因此在该标准下展开为空。
 */
#if PORTABLE_ATTR_C99
#  if PORTABLE_ATTR_MSVC
#    define RESTRICT __restrict
#  else
#    define RESTRICT restrict
#  endif
#else
#  define RESTRICT
#endif

/** @} */


/*──────────────────────── 3. 函数纯度与副作用 ────────────────────────────*/
/**
 * @defgroup group_pure 函数纯度与副作用
 * @brief 描述函数副作用的缺失程度，帮助编译器进行优化（如消除重复调用）。
 * @{
 */

/**
 * @brief 函数没有副作用，可以读取全局数据。
 */
#if PORTABLE_ATTR_HAS_GNU(pure) || PORTABLE_ATTR_GNUC_FAMILY
#  define PURE __attribute__((pure))
#else
#  define PURE
#endif

/**
 * @brief 函数完全依赖于参数，既不读写全局数据。
 */
#if PORTABLE_ATTR_HAS_GNU(const) || PORTABLE_ATTR_GNUC_FAMILY
#  define CONST __attribute__((const))
#else
#  define CONST
#endif

/**
 * @brief 返回的指针不会与其他任何指针别名（类似 malloc）。
 */
#if PORTABLE_ATTR_HAS_GNU(malloc)
#  define MALLOC_LIKE __attribute__((malloc))
#else
#  define MALLOC_LIKE
#endif

/**
 * @brief 指示哪个参数决定了所分配内存块的大小。
 *
 * @param ... 参数索引
 */
#if PORTABLE_ATTR_VARIADIC_MACROS
#  define ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
#  define ALLOC_SIZE(...) /* 降级 */
#endif

/** @} */


/*──────────────────────────── 4. 格式检查 ────────────────────────────────*/
/**
 * @defgroup group_format 格式检查
 * @brief 验证格式化字符串与可变参数的类型匹配，预防安全漏洞。
 * @{
 */

/**
 * @brief printf 风格的格式字符串检查。
 *
 * @param fmt  格式字符串参数索引
 * @param args 可变参数起始索引
 */
#if PORTABLE_ATTR_HAS_GNU(format) || PORTABLE_ATTR_GNUC_FAMILY
#  define FORMAT_PRINTF(fmt, args) __attribute__((format(printf, fmt, args)))
#  define FORMAT_SCANF(fmt, args)  __attribute__((format(scanf,  fmt, args)))
#else
#  define FORMAT_PRINTF(fmt, args)
#  define FORMAT_SCANF(fmt, args)
#endif

/**
 * @brief MSVC SAL 注解，将参数标记为 printf 格式字符串。
 */
#if PORTABLE_ATTR_MSVC
#  if defined(_Printf_format_string_)
#    define PRINTF_FMT _Printf_format_string_
#  else
#    define PRINTF_FMT
#  endif
#else
#  define PRINTF_FMT
#endif

/** @} */


/*──────────────────────────── 5. 控制流 ────────────────────────────────*/
/**
 * @defgroup group_control 控制流
 * @brief 影响函数调用约定、不可达代码、分支预测的提示。
 * @{
 */

/**
 * @brief 函数永不返回到调用者（如 exit、abort）。
 */
#if PORTABLE_ATTR_HAS_C(noreturn)
#  define NORETURN [[noreturn]]
#elif PORTABLE_ATTR_HAS_GNU(noreturn) || PORTABLE_ATTR_GNUC_FAMILY
#  define NORETURN __attribute__((noreturn))
#elif PORTABLE_ATTR_MSVC
#  define NORETURN __declspec(noreturn)
#elif PORTABLE_ATTR_C11
#  define NORETURN _Noreturn
#else
#  define NORETURN
#endif

/**
 * @brief 显式标记 switch case 的穿透（fall‑through）行为，抑制编译器警告。
 */
#if PORTABLE_ATTR_HAS_C(fallthrough)
#  define FALLTHROUGH [[fallthrough]]
#elif PORTABLE_ATTR_HAS_GNU(fallthrough) || PORTABLE_ATTR_GNUC_FAMILY
#  define FALLTHROUGH __attribute__((fallthrough))
#else
#  define FALLTHROUGH ((void)0)
#endif

/**
 * @brief 标记某代码分支不可达，允许编译器优化。
 */
#if PORTABLE_ATTR_HAS_GNU(unreachable) || PORTABLE_ATTR_GNUC_FAMILY
#  define UNREACHABLE() __builtin_unreachable()
#elif PORTABLE_ATTR_MSVC
#  define UNREACHABLE() __assume(0)
#else
#  define UNREACHABLE() ((void)0)
#endif

/**
 * @brief 向编译器提供预期为真的假设，辅助优化。
 *
 * @param x 假设为真的表达式
 */
#if PORTABLE_ATTR_HAS_GNU(assume)
#  define ASSUME(x) __attribute__((assume(x)))
#elif PORTABLE_ATTR_MSVC
#  define ASSUME(x) __assume(x)
#else
#  define ASSUME(x) ((void)0)
#endif

/**
 * @brief 分支预测提示：LIKELY 表示条件预期为真，UNLIKELY 表示预期为假。
 */
#if PORTABLE_ATTR_GNUC_FAMILY
#  define LIKELY(x)   __builtin_expect(!!(x), 1)
#  define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define LIKELY(x)   (x)
#  define UNLIKELY(x) (x)
#endif

/** @} */


/*──────────────────────────── 6. 内联与性能 ──────────────────────────────*/
/**
 * @defgroup group_inline 内联与性能
 * @brief 控制内联决策、代码布局（热/冷路径）以及调试行为。
 * @{
 */

/**
 * @brief 强制内联，无论优化级别如何。
 *
 * 按优先级：GNU always_inline > MSVC __forceinline > C99 inline > C89 兼容扩展。
 */
#if PORTABLE_ATTR_HAS_GNU(always_inline) || PORTABLE_ATTR_GNUC_FAMILY
#  if PORTABLE_ATTR_C99
#    define ALWAYS_INLINE inline __attribute__((always_inline))
#  else
/* C89 下但编译器支持 __inline__ 扩展 */
#    if defined(__GNUC__) || defined(__clang__)
#      define ALWAYS_INLINE __inline__ __attribute__((always_inline))
#    else
#      define ALWAYS_INLINE inline __attribute__((always_inline)) /* 尝试 */
#    endif
#  endif
#elif PORTABLE_ATTR_MSVC
#  define ALWAYS_INLINE __forceinline
#elif PORTABLE_ATTR_C99
#  define ALWAYS_INLINE inline
#else
#  define ALWAYS_INLINE /* C89 且无扩展 */
#endif

/**
 * @brief 禁止内联。
 */
#if PORTABLE_ATTR_HAS_GNU(noinline) || PORTABLE_ATTR_GNUC_FAMILY
#  define NOINLINE __attribute__((noinline))
#elif PORTABLE_ATTR_MSVC
#  define NOINLINE __declspec(noinline)
#else
#  define NOINLINE
#endif

/**
 * @brief 函数被频繁调用，应优化速度并放置在热代码段。
 */
#if PORTABLE_ATTR_HAS_GNU(hot) || PORTABLE_ATTR_GNUC_FAMILY
#  define HOT __attribute__((hot))
#else
#  define HOT
#endif

/**
 * @brief 函数很少被调用，应优化尺寸并放置在冷代码段。
 */
#if PORTABLE_ATTR_HAS_GNU(cold) || PORTABLE_ATTR_GNUC_FAMILY
#  define COLD __attribute__((cold))
#else
#  define COLD
#endif

/**
 * @brief 将该函数体内的所有调用都内联（谨慎使用）。
 */
#if PORTABLE_ATTR_HAS_GNU(flatten)
#  define FLATTEN __attribute__((flatten))
#else
#  define FLATTEN
#endif

/**
 * @brief 对调试器隐藏该函数（通常与 ALWAYS_INLINE 配合减少调用栈污染）。
 */
#if PORTABLE_ATTR_HAS_GNU(artificial)
#  define ARTIFICIAL __attribute__((artificial))
#else
#  define ARTIFICIAL
#endif

/** @} */


/*──────────────────────────── 7. 可见性与链接 ──────────────────────────────*/
/**
 * @defgroup group_visibility 可见性与链接
 * @brief 控制符号的导出、隐藏、弱定义以及抑制未使用警告。
 * @{
 */

/**
 * @brief 导出符号（可见性为 default）。
 */
#if PORTABLE_ATTR_MSVC
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __attribute__((visibility("default")))
#endif

/**
 * @brief 导入符号。
 */
#if PORTABLE_ATTR_MSVC
#  define IMPORT __declspec(dllimport)
#else
#  define IMPORT
#endif

/**
 * @brief 隐藏符号（可见性为 hidden）。
 */
#if PORTABLE_ATTR_MSVC
#  define HIDDEN
#else
#  define HIDDEN __attribute__((visibility("hidden")))
#endif

/**
 * @brief 允许该符号在链接时被强定义覆盖。
 */
#if PORTABLE_ATTR_HAS_GNU(weak) || PORTABLE_ATTR_GNUC_FAMILY
#  define WEAK __attribute__((weak))
#else
#  define WEAK
#endif

/**
 * @brief 防止编译器丢弃未引用的符号。
 */
#if PORTABLE_ATTR_HAS_GNU(used) || PORTABLE_ATTR_GNUC_FAMILY
#  define USED __attribute__((used))
#else
#  define USED
#endif

/**
 * @brief 抑制未使用变量或函数的编译器警告。
 */
#if PORTABLE_ATTR_HAS_GNU(unused) || PORTABLE_ATTR_GNUC_FAMILY
#  define UNUSED __attribute__((unused))
#else
#  define UNUSED
#endif

/**
 * @brief C23 `[[maybe_unused]]` 属性，回退至 GNU unused。
 */
#if PORTABLE_ATTR_HAS_C(maybe_unused)
#  define MAYBE_UNUSED [[maybe_unused]]
#elif PORTABLE_ATTR_HAS_GNU(unused) || PORTABLE_ATTR_GNUC_FAMILY
#  define MAYBE_UNUSED __attribute__((unused))
#else
#  define MAYBE_UNUSED
#endif

/** @} */


/*──────────────────────────── 8. 诊断与弃用 ──────────────────────────────*/
/**
 * @defgroup group_diagnostics 诊断与弃用
 * @brief 在编译期产生警告、错误或标记过时的 API。
 * @{
 */

/**
 * @brief 标记声明为已弃用。
 */
#if PORTABLE_ATTR_HAS_GNU(deprecated) || PORTABLE_ATTR_GNUC_FAMILY
#  define DEPRECATED         __attribute__((deprecated))
#  define DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#elif PORTABLE_ATTR_MSVC
#  define DEPRECATED         __declspec(deprecated)
#  if defined(_MSC_VER) && _MSC_VER >= 1920
#    define DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#  else
#    define DEPRECATED_MSG(msg) __declspec(deprecated)
#  endif
#else
#  define DEPRECATED
#  define DEPRECATED_MSG(msg)
#endif

/**
 * @brief 若函数被调用，则产生编译期错误。
 *
 * @param msg 自定义错误信息
 */
#if PORTABLE_ATTR_HAS_GNU(error) || PORTABLE_ATTR_GNUC_FAMILY
#  define ATTR_ERROR(msg) __attribute__((error(msg)))
#else
#  define ATTR_ERROR(msg)
#endif

/**
 * @brief 若函数被调用，则产生编译期警告。
 *
 * @param msg 自定义警告信息
 */
#if PORTABLE_ATTR_HAS_GNU(warning) || PORTABLE_ATTR_GNUC_FAMILY
#  define ATTR_WARNING(msg) __attribute__((warning(msg)))
#else
#  define ATTR_WARNING(msg)
#endif

/** @} */


/*──────────────────────────── 9. 对齐与布局 ──────────────────────────────*/
/**
 * @defgroup group_layout 对齐与布局
 * @brief 控制变量、结构体成员的对齐和填充，以及节区放置。
 * @{
 */

/**
 * @brief 指定最小对齐方式。
 *
 * @param x 对齐字节数
 */
#if PORTABLE_ATTR_HAS_GNU(aligned) || PORTABLE_ATTR_GNUC_FAMILY
#  define ALIGNED(x) __attribute__((aligned(x)))
#elif PORTABLE_ATTR_MSVC
#  define ALIGNED(x) __declspec(align(x))
#elif PORTABLE_ATTR_C11
#  define ALIGNED(x) _Alignas(x)
#else
#  define ALIGNED(x)
#endif

/**
 * @brief 移除结构体/联合体成员间的填充（紧凑布局）。
 */
#if PORTABLE_ATTR_HAS_GNU(packed) || PORTABLE_ATTR_GNUC_FAMILY
#  define PACKED __attribute__((packed))
#else
#  define PACKED
#endif

/**
 * @brief 将函数或变量放置在指定的节区（section）。
 *
 * @param name 节区名称
 */
#if PORTABLE_ATTR_HAS_GNU(section)
#  define SECTION(name) __attribute__((section(name)))
#else
#  define SECTION(name)
#endif

/** @} */


/*──────────────────────────── 10. 杂项 ────────────────────────────────*/
/**
 * @defgroup group_misc 杂项
 * @brief 构造/析构函数、中断、清理、诊断抑制等特殊用途属性。
 * @{
 */

/**
 * @brief 在 main() 之前或退出之后自动执行的函数。
 */
#if PORTABLE_ATTR_HAS_GNU(constructor) || PORTABLE_ATTR_GNUC_FAMILY
#  define CONSTRUCTOR            __attribute__((constructor))
#  define CONSTRUCTOR_PRIO(n)    __attribute__((constructor(n)))
#else
#  define CONSTRUCTOR
#  define CONSTRUCTOR_PRIO(n)
#endif

#if PORTABLE_ATTR_HAS_GNU(destructor) || PORTABLE_ATTR_GNUC_FAMILY
#  define DESTRUCTOR             __attribute__((destructor))
#  define DESTRUCTOR_PRIO(n)     __attribute__((destructor(n)))
#else
#  define DESTRUCTOR
#  define DESTRUCTOR_PRIO(n)
#endif

/**
 * @brief 标记函数为中断处理程序（架构依赖，通常由 GCC 提供）。
 */
#if PORTABLE_ATTR_HAS_GNU(interrupt)
#  define INTERRUPT __attribute__((interrupt))
#else
#  define INTERRUPT
#endif

/**
 * @brief 当变量超出作用域时自动调用清理函数。
 *
 * @param fn 清理函数指针
 */
#if PORTABLE_ATTR_HAS_GNU(cleanup)
#  define CLEANUP(fn) __attribute__((cleanup(fn)))
#else
#  define CLEANUP(fn)
#endif

/**
 * @brief 为函数禁用特定的 sanitizer（如 address、thread）。
 *
 * @param kind sanitizer 名称
 */
#if PORTABLE_ATTR_HAS_GNU(no_sanitize)
#  define NO_SANITIZE(kind) __attribute__((no_sanitize(kind)))
#else
#  define NO_SANITIZE(kind)
#endif

/**
 * @brief 从另一个函数复制属性（GCC 9+）。
 *
 * @param fn 源函数名
 */
#if PORTABLE_ATTR_HAS_GNU(copy)
#  define COPY_ATTR(fn) __attribute__((copy(fn)))
#else
#  define COPY_ATTR(fn)
#endif

/**
 * @brief 标记函数参数为受污染的，用于静态分析（GCC 12+）。
 */
#if PORTABLE_ATTR_HAS_GNU(tainted_args)
#  define TAINTED_ARGS __attribute__((tainted_args))
#else
#  define TAINTED_ARGS
#endif

/** @} */

#endif /* PORTABLE_ATTRIBUTES_H */
