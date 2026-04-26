#ifndef PORTABLE_ATTRIBUTES_H
#define PORTABLE_ATTRIBUTES_H

/*
 * portable_attributes.h
 *
 * Industrial-grade C attribute abstraction (C11/C17/C23 compatible).
 *
 * Goals:
 *  - Cross GCC / Clang / MSVC
 *  - Provide API-level semantic enhancements
 *  - Safe fallback to empty macros when a feature is unavailable
 *
 * Categories:
 *  - Return value constraints
 *  - Parameter constraints
 *  - Side-effect / optimization
 *  - Format checking
 *  - Control flow
 *  - Inlining / performance
 *  - Visibility
 *  - Diagnostics / deprecation
 *  - Alignment / layout
 *  - Miscellaneous
 */

/*─────────────────────────────── Compiler detection ───────────────────────────────*/

#if defined(__clang__)
#  define ATTR_CLANG 1
#  define ATTR_GCC   0
#  define ATTR_MSVC  0
#elif defined(__GNUC__)
#  define ATTR_CLANG 0
#  define ATTR_GCC   1
#  define ATTR_MSVC  0
#elif defined(_MSC_VER)
#  define ATTR_CLANG 0
#  define ATTR_GCC   0
#  define ATTR_MSVC  1
#else
#  define ATTR_CLANG 0
#  define ATTR_GCC   0
#  define ATTR_MSVC  0
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

/*─────────────────────────────── Return value semantics ───────────────────────────*/

/* NODISCARD: warn when the return value is discarded */
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

/* RETURNS_NONNULL: promise that the returned pointer is never NULL */
#if ATTR_HAS(returns_nonnull)
#  define RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#  define RETURNS_NONNULL
#endif

/* RETURNS_TWICE: function may return more than once (e.g. setjmp, fork) */
#if ATTR_HAS(returns_twice)
#  define RETURNS_TWICE __attribute__((returns_twice))
#else
#  define RETURNS_TWICE
#endif

/*─────────────────────────────── Parameter constraints ────────────────────────────*/

/* NONNULL: pointer arguments that must not be NULL */
#if ATTR_HAS(nonnull) || ATTR_GCC
#  define NONNULL(...)    __attribute__((nonnull(__VA_ARGS__)))
#  define NONNULL_ALL     __attribute__((nonnull))
#else
#  define NONNULL(...)
#  define NONNULL_ALL
#endif

/* ACCESS: describe how a function accesses memory through a pointer argument */
#if ATTR_HAS(access)
#  define ACCESS(mode, idx, ...) __attribute__((access(mode, idx, ##__VA_ARGS__)))
#else
#  define ACCESS(mode, idx, ...)
#endif

/* SAL annotations for MSVC (in-parameter nullness etc.) */
#if ATTR_MSVC
#  include <sal.h>
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

/* SENTINEL: marker for a NULL-terminated variadic argument list */
#if ATTR_HAS(sentinel)
#  define SENTINEL __attribute__((sentinel))
#else
#  define SENTINEL
#endif

/* FORMAT_ARG: indicates an argument that is itself a format string */
#if ATTR_HAS(format_arg)
#  define FORMAT_ARG(idx) __attribute__((format_arg(idx)))
#else
#  define FORMAT_ARG(idx)
#endif

/* ALLOC_ALIGN: identify the parameter that specifies alignment for an allocator */
#if ATTR_HAS(alloc_align)
#  define ALLOC_ALIGN(idx) __attribute__((alloc_align(idx)))
#else
#  define ALLOC_ALIGN(idx)
#endif

/* ASSUME_ALIGNED: inform the compiler that a pointer has a given alignment */
#if ATTR_HAS(assume_aligned)
#  define ASSUME_ALIGNED(n) __attribute__((assume_aligned(n)))
#else
#  define ASSUME_ALIGNED(n)
#endif

/* RESTRICT: portable restrict qualifier for pointer parameters */
#if ATTR_MSVC
#  define RESTRICT __restrict
#else
#  define RESTRICT restrict
#endif

/*─────────────────────────────── Side-effect / optimization ──────────────────────*/

/* PURE: function has no side effects, may read global memory */
#if ATTR_HAS(pure) || ATTR_GCC
#  define PURE __attribute__((pure))
#else
#  define PURE
#endif

/* CONST: function depends only on its parameters (no global memory reads) */
#if ATTR_HAS(const) || ATTR_GCC
#  define CONST __attribute__((const))
#else
#  define CONST
#endif

/* MALLOC_LIKE: returned pointer does not alias any other pointer */
#if ATTR_HAS(malloc)
#  define MALLOC_LIKE __attribute__((malloc))
#else
#  define MALLOC_LIKE
#endif

/* ALLOC_SIZE: indicate which arguments determine the size of the allocated block */
#if ATTR_HAS(alloc_size)
#  define ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
#  define ALLOC_SIZE(...)
#endif

/*─────────────────────────────── Format checking ────────────────────────────────*/

/* FORMAT_PRINTF / FORMAT_SCANF: verify format string against arguments */
#if ATTR_HAS(format) || ATTR_GCC
#  define FORMAT_PRINTF(fmt, args) __attribute__((format(printf, fmt, args)))
#  define FORMAT_SCANF(fmt, args)  __attribute__((format(scanf,  fmt, args)))
#else
#  define FORMAT_PRINTF(fmt, args)
#  define FORMAT_SCANF(fmt, args)
#endif

/* PRINTF_FMT: MSVC SAL annotation for printf-like format strings */
#if ATTR_MSVC
#  define PRINTF_FMT _Printf_format_string_
#else
#  define PRINTF_FMT
#endif

/*─────────────────────────────── Control flow ────────────────────────────────────*/

/* NORETURN: function does not return to its caller */
#if ATTR_HAS(noreturn) || ATTR_GCC
#  define NORETURN __attribute__((noreturn))
#elif ATTR_MSVC
#  define NORETURN __declspec(noreturn)
#else
#  define NORETURN
#endif

/* FALLTHROUGH: explicit switch case fall-through */
#if ATTR_HAS(fallthrough)
#  define FALLTHROUGH __attribute__((fallthrough))
#else
#  define FALLTHROUGH ((void)0)
#endif

/* UNREACHABLE: mark a code path as unreachable */
#if ATTR_HAS(unreachable)
#  define UNREACHABLE() __builtin_unreachable()
#elif ATTR_MSVC
#  define UNREACHABLE() __assume(0)
#else
#  define UNREACHABLE() ((void)0)
#endif

/* ASSUME: provide the compiler with an assumption that is expected to be true */
#if ATTR_HAS(assume)
#  define ASSUME(x) __attribute__((assume(x)))
#elif ATTR_MSVC
#  define ASSUME(x) __assume(x)
#else
#  define ASSUME(x) ((void)0)
#endif

/* LIKELY / UNLIKELY: branch prediction hints */
#if ATTR_HAS(likely)
#  define LIKELY(x)   __builtin_expect(!!(x), 1)
#  define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define LIKELY(x)   (x)
#  define UNLIKELY(x) (x)
#endif

/*─────────────────────────────── Inlining & performance ─────────────────────────*/

/* ALWAYS_INLINE: force inlining regardless of optimisation level */
#if ATTR_HAS(always_inline) || ATTR_GCC
#  define ALWAYS_INLINE inline __attribute__((always_inline))
#elif ATTR_MSVC
#  define ALWAYS_INLINE __forceinline
#else
#  define ALWAYS_INLINE inline
#endif

/* NOINLINE: prevent inlining */
#if ATTR_HAS(noinline) || ATTR_GCC
#  define NOINLINE __attribute__((noinline))
#elif ATTR_MSVC
#  define NOINLINE __declspec(noinline)
#else
#  define NOINLINE
#endif

/* HOT: function is frequently called (optimise for speed, keep in hot section) */
#if ATTR_HAS(hot)
#  define HOT __attribute__((hot))
#else
#  define HOT
#endif

/* COLD: function is rarely called (optimise for size, keep out of hot path) */
#if ATTR_HAS(cold)
#  define COLD __attribute__((cold))
#else
#  define COLD
#endif

/* FLATTEN: inline every call inside this function */
#if ATTR_HAS(flatten)
#  define FLATTEN __attribute__((flatten))
#else
#  define FLATTEN
#endif

/* ARTIFICIAL: hide this function from debuggers (useful with ALWAYS_INLINE) */
#if ATTR_HAS(artificial)
#  define ARTIFICIAL __attribute__((artificial))
#else
#  define ARTIFICIAL
#endif

/*─────────────────────────────── Visibility ─────────────────────────────────────*/

/* EXPORT / IMPORT: control symbol visibility for shared libraries */
#if ATTR_MSVC
#  define EXPORT __declspec(dllexport)
#  define IMPORT __declspec(dllimport)
#  define HIDDEN
#else
#  define EXPORT __attribute__((visibility("default")))
#  define IMPORT
#  define HIDDEN __attribute__((visibility("hidden")))
#endif

/*─────────────────────────────── Diagnostics / deprecation ──────────────────────*/

/* DEPRECATED / DEPRECATED_MSG: mark a declaration as deprecated */
#if ATTR_HAS(deprecated)
#  define DEPRECATED         __attribute__((deprecated))
#  define DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#elif ATTR_MSVC
#  define DEPRECATED         __declspec(deprecated)
#  define DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#else
#  define DEPRECATED
#  define DEPRECATED_MSG(msg)
#endif

/* ERROR / WARNING: force a compile-time error or warning if a function is called */
#if ATTR_HAS(error)
#  define ATTR_ERROR(msg) __attribute__((error(msg)))
#else
#  define ATTR_ERROR(msg)
#endif

#if ATTR_HAS(warning)
#  define ATTR_WARNING(msg) __attribute__((warning(msg)))
#else
#  define ATTR_WARNING(msg)
#endif

/*─────────────────────────────── Alignment & layout ─────────────────────────────*/

/* ALIGNED: specify minimum alignment */
#if ATTR_HAS(aligned) || ATTR_GCC
#  define ALIGNED(x) __attribute__((aligned(x)))
#elif ATTR_MSVC
#  define ALIGNED(x) __declspec(align(x))
#else
#  define ALIGNED(x)
#endif

/* PACKED: remove padding between structure/union members */
#if ATTR_HAS(packed) || ATTR_GCC
#  define PACKED __attribute__((packed))
#else
#  define PACKED
#endif

/* SECTION: place a function or variable in a specific section */
#if ATTR_HAS(section)
#  define SECTION(name) __attribute__((section(name)))
#else
#  define SECTION(name)
#endif

/*─────────────────────────────── Miscellaneous ──────────────────────────────────*/

/* WEAK: allow a symbol to be overridden by a strong definition at link time */
#if ATTR_HAS(weak)
#  define WEAK __attribute__((weak))
#else
#  define WEAK
#endif

/* USED: prevent the compiler from discarding a symbol, even if it is unreferenced */
#if ATTR_HAS(used) || ATTR_GCC
#  define USED __attribute__((used))
#else
#  define USED
#endif

/* UNUSED: suppress "unused" warnings for a function, parameter or variable */
#if ATTR_HAS(unused) || ATTR_GCC
#  define UNUSED __attribute__((unused))
#else
#  define UNUSED
#endif

/* MAYBE_UNUSED: standard C23 maybe_unused attribute, fallback to unused */
#if ATTR_HAS_C(maybe_unused)
#  define MAYBE_UNUSED [[maybe_unused]]
#elif ATTR_HAS(unused) || ATTR_GCC
#  define MAYBE_UNUSED __attribute__((unused))
#else
#  define MAYBE_UNUSED
#endif

/* COPY_ATTR: copy attributes from another function (GCC 9+) */
#if ATTR_HAS(copy)
#  define COPY_ATTR(fn) __attribute__((copy(fn)))
#else
#  define COPY_ATTR(fn)
#endif

/* TAINTED_ARGS: mark function parameters as tainted for static analysis (GCC 12+) */
#if ATTR_HAS(tainted_args)
#  define TAINTED_ARGS __attribute__((tainted_args))
#else
#  define TAINTED_ARGS
#endif

/* CONSTRUCTOR / DESTRUCTOR: run a function automatically before main / after exit */
#if ATTR_HAS(constructor)
#  define CONSTRUCTOR            __attribute__((constructor))
#  define CONSTRUCTOR_PRIO(n)    __attribute__((constructor(n)))
#else
#  define CONSTRUCTOR
#  define CONSTRUCTOR_PRIO(n)
#endif

#if ATTR_HAS(destructor)
#  define DESTRUCTOR             __attribute__((destructor))
#  define DESTRUCTOR_PRIO(n)     __attribute__((destructor(n)))
#else
#  define DESTRUCTOR
#  define DESTRUCTOR_PRIO(n)
#endif

/* INTERRUPT: mark a function as an interrupt handler (GCC, architecture-dependent) */
#if ATTR_HAS(interrupt)
#  define INTERRUPT __attribute__((interrupt))
#else
#  define INTERRUPT
#endif

/* CLEANUP: automatically call a cleanup function when a variable goes out of scope */
#if ATTR_HAS(cleanup)
#  define CLEANUP(fn) __attribute__((cleanup(fn)))
#else
#  define CLEANUP(fn)
#endif

/* NO_SANITIZE: disable a specific sanitizer for a function */
#if ATTR_HAS(no_sanitize)
#  define NO_SANITIZE(kind) __attribute__((no_sanitize(kind)))
#else
#  define NO_SANITIZE(kind)
#endif

#endif /* PORTABLE_ATTRIBUTES_H */