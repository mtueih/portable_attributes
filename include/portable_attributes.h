#ifndef PORTABLE_ATTRIBUTES_H
#define PORTABLE_ATTRIBUTES_H

/*
 * portable_attributes.h
 *
 * 工业级 C 属性封装（C11/C17/C23 兼容）
 *
 * 目标：
 *  - 跨 GCC / Clang / MSVC
 *  - 提供 API 级语义增强
 *  - 安全降级（不支持则为空）
 *
 * 分类：
 *  - 返回值约束
 *  - 参数约束
 *  - 副作用/优化
 *  - 控制流
 *  - 内存/分配
 *  - 可见性
 *  - 诊断/弃用
 *  - 性能提示
 */

/*─────────────────────────────── 编译器检测 ───────────────────────────────*/

#if defined(__clang__)
#  define ATTR_CLANG 1
#  define ATTR_GCC 0
#  define ATTR_MSVC 0
#elif defined(__GNUC__)
#  define ATTR_CLANG 0
#  define ATTR_GCC 1
#  define ATTR_MSVC 0
#elif defined(_MSC_VER)
#  define ATTR_CLANG 0
#  define ATTR_GCC 0
#  define ATTR_MSVC 1
#else
#  define ATTR_CLANG 0
#  define ATTR_GCC 0
#  define ATTR_MSVC 0
#endif

#if defined(__has_attribute)
#  define ATTR_HAS(x) __has_attribute(x)
#else
#  define ATTR_HAS(x) 0
#endif

#if defined(__has_c_attribute)
#  define ATTR_HAS_C(x) __has_c_attribute(x)
#else
#  define ATTR_HAS_C(x) 0
#endif

/*─────────────────────────────── 返回值语义 ───────────────────────────────*/

#if ATTR_HAS_C(nodiscard)
#  define NODISCARD [[nodiscard]]
#elif ATTR_HAS(warn_unused_result) || ATTR_GCC
#  define NODISCARD __attribute__((warn_unused_result))
#elif ATTR_MSVC
#  include <sal.h>
#  define NODISCARD _Check_return_
#else
#  define NODISCARD
#endif

#if ATTR_HAS(returns_nonnull)
#  define RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#  define RETURNS_NONNULL
#endif

#if ATTR_HAS(returns_twice)
#  define RETURNS_TWICE __attribute__((returns_twice))
#else
#  define RETURNS_TWICE
#endif

/*─────────────────────────────── 参数约束 ───────────────────────────────*/

#if ATTR_HAS(nonnull) || ATTR_GCC
#  define NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#  define NONNULL_ALL __attribute__((nonnull))
#else
#  define NONNULL(...)
#  define NONNULL_ALL
#endif

#if ATTR_HAS(access)
#  define ACCESS(mode, idx, ...) __attribute__((access(mode, idx, ##__VA_ARGS__)))
#else
#  define ACCESS(mode, idx, ...)
#endif

#if ATTR_MSVC
#  include <sal.h>
#  define IN_NONNULL _In_
#  define OUT_NONNULL _Out_
#  define INOUT _Inout_
#  define IN_OPT _In_opt_
#else
#  define IN_NONNULL
#  define OUT_NONNULL
#  define INOUT
#  define IN_OPT
#endif

/*─────────────────────────────── 副作用与优化 ───────────────────────────────*/

#if ATTR_HAS(pure) || ATTR_GCC
#  define PURE __attribute__((pure))
#else
#  define PURE
#endif

#if ATTR_HAS(const) || ATTR_GCC
#  define CONST __attribute__((const))
#else
#  define CONST
#endif

#if ATTR_HAS(malloc)
#  define MALLOC_LIKE __attribute__((malloc))
#else
#  define MALLOC_LIKE
#endif

#if ATTR_HAS(alloc_size)
#  define ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
#  define ALLOC_SIZE(...)
#endif

/*─────────────────────────────── 格式检查 ───────────────────────────────*/

#if ATTR_HAS(format) || ATTR_GCC
#  define FORMAT_PRINTF(fmt, args) __attribute__((format(printf, fmt, args)))
#  define FORMAT_SCANF(fmt, args)  __attribute__((format(scanf, fmt, args)))
#else
#  define FORMAT_PRINTF(fmt, args)
#  define FORMAT_SCANF(fmt, args)
#endif

#if ATTR_MSVC
#  define PRINTF_FMT _Printf_format_string_
#else
#  define PRINTF_FMT
#endif

/*─────────────────────────────── 控制流 ───────────────────────────────*/

#if ATTR_HAS(noreturn) || ATTR_GCC
#  define NORETURN __attribute__((noreturn))
#elif ATTR_MSVC
#  define NORETURN __declspec(noreturn)
#else
#  define NORETURN
#endif

#if ATTR_HAS(fallthrough)
#  define FALLTHROUGH __attribute__((fallthrough))
#else
#  define FALLTHROUGH ((void)0)
#endif

#if ATTR_HAS(unreachable)
#  define UNREACHABLE() __builtin_unreachable()
#elif ATTR_MSVC
#  define UNREACHABLE() __assume(0)
#else
#  define UNREACHABLE() ((void)0)
#endif

#if ATTR_HAS(assume)
#  define ASSUME(x) __attribute__((assume(x)))
#elif ATTR_MSVC
#  define ASSUME(x) __assume(x)
#else
#  define ASSUME(x) ((void)0)
#endif

/*─────────────────────────────── 内联与性能 ───────────────────────────────*/

#if ATTR_HAS(always_inline) || ATTR_GCC
#  define ALWAYS_INLINE inline __attribute__((always_inline))
#elif ATTR_MSVC
#  define ALWAYS_INLINE __forceinline
#else
#  define ALWAYS_INLINE inline
#endif

#if ATTR_HAS(noinline) || ATTR_GCC
#  define NOINLINE __attribute__((noinline))
#elif ATTR_MSVC
#  define NOINLINE __declspec(noinline)
#else
#  define NOINLINE
#endif

#if ATTR_HAS(hot)
#  define HOT __attribute__((hot))
#else
#  define HOT
#endif

#if ATTR_HAS(cold)
#  define COLD __attribute__((cold))
#else
#  define COLD
#endif

#if ATTR_HAS(likely)
#  define LIKELY(x)   __builtin_expect(!!(x), 1)
#  define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define LIKELY(x)   (x)
#  define UNLIKELY(x) (x)
#endif

/*─────────────────────────────── 可见性（跨平台）──────────────────────────────*/

#if ATTR_MSVC
#  define EXPORT __declspec(dllexport)
#  define IMPORT __declspec(dllimport)
#else
#  define EXPORT __attribute__((visibility("default")))
#  define IMPORT
#endif

/*─────────────────────────────── 弃用与诊断 ───────────────────────────────*/

#if ATTR_HAS(deprecated)
#  define DEPRECATED __attribute__((deprecated))
#  define DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#elif ATTR_MSVC
#  define DEPRECATED __declspec(deprecated)
#  define DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#else
#  define DEPRECATED
#  define DEPRECATED_MSG(msg)
#endif

/*─────────────────────────────── 对齐与布局 ───────────────────────────────*/

#if ATTR_HAS(aligned) || ATTR_GCC
#  define ALIGNED(x) __attribute__((aligned(x)))
#elif ATTR_MSVC
#  define ALIGNED(x) __declspec(align(x))
#else
#  define ALIGNED(x)
#endif

#if ATTR_HAS(packed) || ATTR_GCC
#  define PACKED __attribute__((packed))
#else
#  define PACKED
#endif

/*─────────────────────────────── 其他辅助 ───────────────────────────────*/

#if ATTR_HAS(weak)
#  define WEAK __attribute__((weak))
#else
#  define WEAK
#endif

/*─────────────────────────────── 结束 ───────────────────────────────*/

#endif /* PORTABLE_ATTRIBUTES_H */