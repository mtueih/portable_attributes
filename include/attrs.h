/**
 * @file attrs.h
 * @brief Cross-compiler C attributes abstraction macros.
 * @version 1.0.0
 *
 * Supports: C89, C95, C99, C11, C17, C23.
 * Compilers: GCC, Clang, MSVC, Intel C/C++, TinyCC, SDCC, ARM/Keil, IAR,
 *            Oracle/Sun CC, PGI/NVIDIA HPC, IBM XL, Embarcadero/Borland.
 *
 * All macros safely degrade to nothing when unsupported.
 */

#ifndef ATTRS_H
#define ATTRS_H

/* ============================================================================
 * 0. Compiler & Standard Detection
 * ============================================================================ */

/* C standard version */
#if defined(__STDC_VERSION__)
#  define ATTRS_C_STD __STDC_VERSION__
#elif defined(__STDC__)
#  define ATTRS_C_STD 198901L   /* C89/C90 */
#else
#  define ATTRS_C_STD 0L        /* Pre-standard */
#endif

/* Compiler detection */
#define ATTRS_COMPILER_UNKNOWN 0
#define ATTRS_COMPILER_GCC     1
#define ATTRS_COMPILER_CLANG   2
#define ATTRS_COMPILER_MSVC    3
#define ATTRS_COMPILER_INTEL   4   /* ICC/ICX */
#define ATTRS_COMPILER_TCC     5   /* TinyCC */
#define ATTRS_COMPILER_SDCC    6   /* Small Device C Compiler */
#define ATTRS_COMPILER_KEIL    7   /* ARM/Keil (armcc, armclang) */
#define ATTRS_COMPILER_IAR     8   /* IAR Embedded Workbench */
#define ATTRS_COMPILER_SUN     9   /* Oracle/Sun Studio */
#define ATTRS_COMPILER_PGI     10  /* PGI / NVIDIA HPC SDK */
#define ATTRS_COMPILER_IBM     11  /* IBM XL C/C++ */
#define ATTRS_COMPILER_BORLAND 12  /* Embarcadero/Borland */
#define ATTRS_COMPILER_WATCOM  13  /* OpenWatcom */
#define ATTRS_COMPILER_DIGITAL 14  /* Digital Mars */

#if defined(__clang__)
#  define ATTRS_COMPILER ATTRS_COMPILER_CLANG
#  define ATTRS_COMPILER_STR "clang"
#  define ATTRS_COMPILER_MAJOR __clang_major__
#  define ATTRS_COMPILER_MINOR __clang_minor__
#  define ATTRS_COMPILER_PATCH __clang_patchlevel__
#elif defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#  define ATTRS_COMPILER ATTRS_COMPILER_INTEL
#  define ATTRS_COMPILER_STR "intel"
#  if defined(__INTEL_COMPILER)
#    define ATTRS_COMPILER_MAJOR (__INTEL_COMPILER / 100)
#    define ATTRS_COMPILER_MINOR (__INTEL_COMPILER % 100 / 10)
#    define ATTRS_COMPILER_PATCH 0
#  else
#    define ATTRS_COMPILER_MAJOR __clang_major__
#    define ATTRS_COMPILER_MINOR __clang_minor__
#    define ATTRS_COMPILER_PATCH __clang_patchlevel__
#  endif
#elif defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#  define ATTRS_COMPILER ATTRS_COMPILER_GCC
#  define ATTRS_COMPILER_STR "gcc"
#  define ATTRS_COMPILER_MAJOR __GNUC__
#  define ATTRS_COMPILER_MINOR __GNUC_MINOR__
#  define ATTRS_COMPILER_PATCH __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
#  define ATTRS_COMPILER ATTRS_COMPILER_MSVC
#  define ATTRS_COMPILER_STR "msvc"
#  define ATTRS_COMPILER_MAJOR (_MSC_VER / 100)
#  define ATTRS_COMPILER_MINOR (_MSC_VER % 100)
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__TINYC__)
#  define ATTRS_COMPILER ATTRS_COMPILER_TCC
#  define ATTRS_COMPILER_STR "tcc"
#  define ATTRS_COMPILER_MAJOR 0
#  define ATTRS_COMPILER_MINOR 0
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__SDCC)
#  define ATTRS_COMPILER ATTRS_COMPILER_SDCC
#  define ATTRS_COMPILER_STR "sdcc"
#  define ATTRS_COMPILER_MAJOR 0
#  define ATTRS_COMPILER_MINOR 0
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#  define ATTRS_COMPILER ATTRS_COMPILER_KEIL
#  define ATTRS_COMPILER_STR "keil"
#  define ATTRS_COMPILER_MAJOR (__ARMCC_VERSION / 1000000)
#  define ATTRS_COMPILER_MINOR (__ARMCC_VERSION % 1000000 / 10000)
#  define ATTRS_COMPILER_PATCH (__ARMCC_VERSION % 10000)
#elif defined(__IAR_SYSTEMS_ICC__)
#  define ATTRS_COMPILER ATTRS_COMPILER_IAR
#  define ATTRS_COMPILER_STR "iar"
#  define ATTRS_COMPILER_MAJOR (__VER__ / 100)
#  define ATTRS_COMPILER_MINOR (__VER__ % 100)
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__SUNPRO_C)
#  define ATTRS_COMPILER ATTRS_COMPILER_SUN
#  define ATTRS_COMPILER_STR "sun"
#  define ATTRS_COMPILER_MAJOR (__SUNPRO_C / 0x100)
#  define ATTRS_COMPILER_MINOR (__SUNPRO_C % 0x100)
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__PGI) || defined(__NVCOMPILER)
#  define ATTRS_COMPILER ATTRS_COMPILER_PGI
#  define ATTRS_COMPILER_STR "pgi"
#  define ATTRS_COMPILER_MAJOR 0
#  define ATTRS_COMPILER_MINOR 0
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__xlC__) || defined(__IBMC__)
#  define ATTRS_COMPILER ATTRS_COMPILER_IBM
#  define ATTRS_COMPILER_STR "ibm"
#  define ATTRS_COMPILER_MAJOR 0
#  define ATTRS_COMPILER_MINOR 0
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__BORLANDC__)
#  define ATTRS_COMPILER ATTRS_COMPILER_BORLAND
#  define ATTRS_COMPILER_STR "borland"
#  define ATTRS_COMPILER_MAJOR (__BORLANDC__ / 0x100)
#  define ATTRS_COMPILER_MINOR (__BORLANDC__ % 0x100)
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__WATCOMC__)
#  define ATTRS_COMPILER ATTRS_COMPILER_WATCOM
#  define ATTRS_COMPILER_STR "watcom"
#  define ATTRS_COMPILER_MAJOR (__WATCOMC__ / 100)
#  define ATTRS_COMPILER_MINOR (__WATCOMC__ % 100)
#  define ATTRS_COMPILER_PATCH 0
#elif defined(__DMC__)
#  define ATTRS_COMPILER ATTRS_COMPILER_DIGITAL
#  define ATTRS_COMPILER_STR "digitalmars"
#  define ATTRS_COMPILER_MAJOR (__DMC__ / 0x100)
#  define ATTRS_COMPILER_MINOR (__DMC__ % 0x100)
#  define ATTRS_COMPILER_PATCH 0
#else
#  define ATTRS_COMPILER ATTRS_COMPILER_UNKNOWN
#  define ATTRS_COMPILER_STR "unknown"
#  define ATTRS_COMPILER_MAJOR 0
#  define ATTRS_COMPILER_MINOR 0
#  define ATTRS_COMPILER_PATCH 0
#endif

/* ============================================================================
 * 1. Feature Detection Helpers
 * ============================================================================ */

/* __has_attribute (Clang, GCC 5+, some others) */
#if defined(__has_attribute)
#  define ATTRS_HAS_ATTR(x) __has_attribute(x)
#else
#  define ATTRS_HAS_ATTR(x) 0
#endif

/* __has_feature (Clang) */
#if defined(__has_feature)
#  define CATTRS_HAS_FEATURE(x) __has_feature(x)
#else
#  define CATTRS_HAS_FEATURE(x) 0
#endif

/* __has_extension (Clang) */
#if defined(__has_extension)
#  define CATTRS_HAS_EXTENSION(x) __has_extension(x)
#else
#  define CATTRS_HAS_EXTENSION(x) 0
#endif

/* __has_c_attribute (C23) */
#if defined(__has_c_attribute)
#  define ATTRS_HAS_C_ATTR(x) __has_c_attribute(x)
#elif ATTRS_C_STD >= 202311L
#  define ATTRS_HAS_C_ATTR(x) 1
#else
#  define ATTRS_HAS_C_ATTR(x) 0
#endif

/* GCC version check */
#if ATTRS_COMPILER == ATTRS_COMPILER_GCC
#  define ATTRS_GCC_AT_LEAST(maj, min, patch) \
       ((ATTRS_COMPILER_MAJOR > (maj)) || \
        (ATTRS_COMPILER_MAJOR == (maj) && ATTRS_COMPILER_MINOR > (min)) || \
        (ATTRS_COMPILER_MAJOR == (maj) && ATTRS_COMPILER_MINOR == (min) && \
         ATTRS_COMPILER_PATCH >= (patch)))
#else
#  define ATTRS_GCC_AT_LEAST(maj, min, patch) 0
#endif

/* Clang version check */
#if ATTRS_COMPILER == ATTRS_COMPILER_CLANG
#  define ATTRS_CLANG_AT_LEAST(maj, min, patch) \
       ((ATTRS_COMPILER_MAJOR > (maj)) || \
        (ATTRS_COMPILER_MAJOR == (maj) && ATTRS_COMPILER_MINOR > (min)) || \
        (ATTRS_COMPILER_MAJOR == (maj) && ATTRS_COMPILER_MINOR == (min) && \
         ATTRS_COMPILER_PATCH >= (patch)))
#else
#  define ATTRS_CLANG_AT_LEAST(maj, min, patch) 0
#endif

/* MSVC version check */
#if ATTRS_COMPILER == ATTRS_COMPILER_MSVC
#  define ATTRS_MSVC_AT_LEAST(ver) (_MSC_VER >= (ver))
#else
#  define ATTRS_MSVC_AT_LEAST(ver) 0
#endif

/* ============================================================================
 * 2. Attribute Syntax Selection
 * ============================================================================ */

/*
 * Priority:
 * 1. C23 [[...]] syntax (if available)
 * 2. GCC __attribute__((...)) syntax
 * 3. MSVC __declspec(...) syntax
 * 4. Empty fallback
 */

/* C23 bracket syntax */
#if ATTRS_C_STD >= 202311L
#  define ATTRS_BRACKET_ATTR(x) [[x]]
#else
#  define ATTRS_BRACKET_ATTR(x)
#endif

/* GCC attribute syntax */
#if ATTRS_COMPILER == ATTRS_COMPILER_GCC || \
ATTRS_COMPILER == ATTRS_COMPILER_CLANG || \
ATTRS_COMPILER == ATTRS_COMPILER_INTEL || \
ATTRS_COMPILER == ATTRS_COMPILER_TCC || \
    (ATTRS_COMPILER == ATTRS_COMPILER_KEIL && defined(__GNUC__)) || \
ATTRS_COMPILER == ATTRS_COMPILER_SUN || \
ATTRS_COMPILER == ATTRS_COMPILER_PGI || \
ATTRS_COMPILER == ATTRS_COMPILER_IBM
#  define ATTRS_GCC_ATTR(x) __attribute__((x))
#else
#  define ATTRS_GCC_ATTR(x)
#endif

/* MSVC declspec syntax */
#if ATTRS_COMPILER == ATTRS_COMPILER_MSVC || \
ATTRS_COMPILER == ATTRS_COMPILER_BORLAND || \
ATTRS_COMPILER == ATTRS_COMPILER_WATCOM || \
ATTRS_COMPILER == ATTRS_COMPILER_DIGITAL
#  define ATTRS_MSVC_ATTR(x) __declspec(x)
#else
#  define ATTRS_MSVC_ATTR(x)
#endif

/* IAR pragma syntax */
#if ATTRS_COMPILER == ATTRS_COMPILER_IAR
#  define ATTRS_IAR_PRAGMA(x) _Pragma(#x)
#else
#  define ATTRS_IAR_PRAGMA(x)
#endif

/* SDCC syntax */
#if ATTRS_COMPILER == ATTRS_COMPILER_SDCC
#  define ATTRS_SDCC_ATTR(x) __attribute__((x))
#else
#  define ATTRS_SDCC_ATTR(x)
#endif

/* Keil armcc (non-GNU mode) syntax */
#if ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_KEIL_ATTR(x) __attribute__((x))
#else
#  define ATTRS_KEIL_ATTR(x)
#endif

/* ============================================================================
 * 3. Core Attribute Macros
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 3.1 noreturn - Function does not return                                    */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_C_ATTR(noreturn) || ATTRS_C_STD >= 202311L
#  define ATTRS_NORETURN [[noreturn]]
#elif ATTRS_HAS_ATTR(noreturn) || ATTRS_GCC_AT_LEAST(2, 5, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_NORETURN ATTRS_GCC_ATTR(noreturn)
#elif ATTRS_MSVC_AT_LEAST(1200)
#  define ATTRS_NORETURN ATTRS_MSVC_ATTR(noreturn)
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_NORETURN ATTRS_KEIL_ATTR(noreturn)
#elif ATTRS_C_STD >= 201112L
#  define ATTRS_NORETURN _Noreturn   /* C11 keyword */
#else
#  define ATTRS_NORETURN
#endif

/* -------------------------------------------------------------------------- */
/* 3.2 deprecated - Entity is deprecated                                      */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_C_ATTR(deprecated) || ATTRS_C_STD >= 202311L
#  define ATTRS_DEPRECATED(msg) [[deprecated(msg)]]
#elif ATTRS_HAS_ATTR(deprecated) || ATTRS_GCC_AT_LEAST(4, 0, 0) || \
ATTRS_CLANG_AT_LEAST(2, 9, 0)
#  define ATTRS_DEPRECATED(msg) ATTRS_GCC_ATTR(deprecated(msg))
#elif ATTRS_MSVC_AT_LEAST(1910) /* VS 2017+ supports message */
#  define ATTRS_DEPRECATED(msg) ATTRS_MSVC_ATTR(deprecated(msg))
#elif ATTRS_MSVC_AT_LEAST(1300)
#  define ATTRS_DEPRECATED(msg) ATTRS_MSVC_ATTR(deprecated)
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_DEPRECATED(msg) ATTRS_KEIL_ATTR(deprecated)
#else
#  define ATTRS_DEPRECATED(msg)
#endif

/* -------------------------------------------------------------------------- */
/* 3.3 nodiscard - Return value should not be discarded                       */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_C_ATTR(nodiscard) || ATTRS_C_STD >= 202311L
#  define ATTRS_NODISCARD(msg) [[nodiscard(msg)]]
#elif ATTRS_HAS_ATTR(warn_unused_result) || ATTRS_GCC_AT_LEAST(3, 4, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_NODISCARD(msg) ATTRS_GCC_ATTR(warn_unused_result)
#elif ATTRS_MSVC_AT_LEAST(1700) /* VS 2012+ _Check_return_ */
#  define ATTRS_NODISCARD(msg) _Check_return_
#else
#  define ATTRS_NODISCARD(msg)
#endif

/* Simple nodiscard without message */
#if ATTRS_HAS_C_ATTR(nodiscard) || ATTRS_C_STD >= 202311L
#  define ATTRS_NODISCARD_SIMPLE [[nodiscard]]
#elif ATTRS_HAS_ATTR(warn_unused_result) || ATTRS_GCC_AT_LEAST(3, 4, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_NODISCARD_SIMPLE ATTRS_GCC_ATTR(warn_unused_result)
#else
#  define ATTRS_NODISCARD_SIMPLE
#endif

/* -------------------------------------------------------------------------- */
/* 3.4 maybe_unused - Suppress unused warnings                                */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_C_ATTR(maybe_unused) || ATTRS_C_STD >= 202311L
#  define ATTRS_MAYBE_UNUSED [[maybe_unused]]
#elif ATTRS_HAS_ATTR(unused) || ATTRS_GCC_AT_LEAST(2, 7, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_MAYBE_UNUSED ATTRS_GCC_ATTR(unused)
#elif ATTRS_MSVC_AT_LEAST(1910) /* VS 2017+ */
#  define ATTRS_MAYBE_UNUSED ATTRS_MSVC_ATTR(unreferenced)
#else
#  define ATTRS_MAYBE_UNUSED
#endif

/* -------------------------------------------------------------------------- */
/* 3.5 fallthrough - Intentional switch case fallthrough                      */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_C_ATTR(fallthrough) || ATTRS_C_STD >= 202311L
#  define ATTRS_FALLTHROUGH [[fallthrough]]
#elif ATTRS_HAS_ATTR(fallthrough) || ATTRS_GCC_AT_LEAST(7, 0, 0) || \
ATTRS_CLANG_AT_LEAST(3, 9, 0)
#  define ATTRS_FALLTHROUGH ATTRS_GCC_ATTR(fallthrough)
#elif ATTRS_COMPILER == ATTRS_COMPILER_CLANG && \
      ATTRS_CLANG_AT_LEAST(3, 0, 0)
/* Clang also supports __attribute__((fallthrough)) since 3.0 */
#  define ATTRS_FALLTHROUGH ATTRS_GCC_ATTR(fallthrough)
#else
#  define ATTRS_FALLTHROUGH ((void)0)
#endif

/* -------------------------------------------------------------------------- */
/* 3.6 unsequenced - Function has no side effects (C23)                       */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_C_ATTR(unsequenced) || ATTRS_C_STD >= 202311L
#  define ATTRS_UNSEQUENCED [[unsequenced]]
#else
#  define ATTRS_UNSEQUENCED
#endif

/* -------------------------------------------------------------------------- */
/* 3.7 reproducible - Function has no side effects but may depend on state    */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_C_ATTR(reproducible) || ATTRS_C_STD >= 202311L
#  define ATTRS_REPRODUCIBLE [[reproducible]]
#else
#  define ATTRS_REPRODUCIBLE
#endif

/* ============================================================================
 * 4. Alignment Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 4.1 aligned - Variable/type alignment                                      */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(aligned) || ATTRS_GCC_AT_LEAST(2, 95, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_ALIGNED(n) ATTRS_GCC_ATTR(aligned(n))
#elif ATTRS_MSVC_AT_LEAST(1300)
#  define ATTRS_ALIGNED(n) ATTRS_MSVC_ATTR(align(n))
#elif ATTRS_C_STD >= 201112L
#  define ATTRS_ALIGNED(n) _Alignas(n)
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_ALIGNED(n) ATTRS_KEIL_ATTR(aligned(n))
#elif ATTRS_COMPILER == ATTRS_COMPILER_IAR
#  define ATTRS_ALIGNED(n) ATTRS_IAR_PRAGMA(data_alignment = n)
#elif ATTRS_COMPILER == ATTRS_COMPILER_BORLAND
#  define ATTRS_ALIGNED(n) ATTRS_MSVC_ATTR(align(n))
#else
#  define ATTRS_ALIGNED(n)
#endif

/* -------------------------------------------------------------------------- */
/* 4.2 assume_aligned - Function returns aligned pointer (GCC 4.9+)           */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(assume_aligned) || ATTRS_GCC_AT_LEAST(4, 9, 0) || \
ATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define ATTRS_ASSUME_ALIGNED(align, ...) \
       ATTRS_GCC_ATTR(assume_aligned(align, ## __VA_ARGS__))
#else
#  define ATTRS_ASSUME_ALIGNED(align, ...)
#endif

/* -------------------------------------------------------------------------- */
/* 4.3 alloc_align - Parameter specifies alignment of allocated memory        */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(alloc_align) || ATTRS_GCC_AT_LEAST(4, 9, 0) || \
ATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define ATTRS_ALLOC_ALIGN(n) ATTRS_GCC_ATTR(alloc_align(n))
#else
#  define ATTRS_ALLOC_ALIGN(n)
#endif

/* ============================================================================
 * 5. Function Behavior Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 5.1 const - Pure computation, no memory read (stricter than pure)          */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(const) || ATTRS_GCC_AT_LEAST(2, 5, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_CONST ATTRS_GCC_ATTR(const)
#else
#  define ATTRS_CONST
#endif

/* -------------------------------------------------------------------------- */
/* 5.2 pure - No side effects, may read memory                                */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(pure) || ATTRS_GCC_AT_LEAST(2, 96, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_PURE ATTRS_GCC_ATTR(pure)
#else
#  define ATTRS_PURE
#endif

/* -------------------------------------------------------------------------- */
/* 5.3 malloc-like - Returns freshly allocated memory                         */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(malloc) || ATTRS_GCC_AT_LEAST(2, 96, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_MALLOC ATTRS_GCC_ATTR(malloc)
#elif ATTRS_MSVC_AT_LEAST(1900) /* VS 2015+ */
#  define ATTRS_MALLOC ATTRS_MSVC_ATTR(allocator)
#else
#  define ATTRS_MALLOC
#endif

/* -------------------------------------------------------------------------- */
/* 5.4 alloc_size - Parameters specify allocation size                        */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(alloc_size) || ATTRS_GCC_AT_LEAST(4, 3, 0) || \
ATTRS_CLANG_AT_LEAST(3, 0, 0)
#  define ATTRS_ALLOC_SIZE(...) ATTRS_GCC_ATTR(alloc_size(__VA_ARGS__))
#else
#  define ATTRS_ALLOC_SIZE(...)
#endif

/* -------------------------------------------------------------------------- */
/* 5.5 returns_nonnull - Return value is never NULL                           */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(returns_nonnull) || ATTRS_GCC_AT_LEAST(4, 9, 0) || \
ATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define ATTRS_RETURNS_NONNULL ATTRS_GCC_ATTR(returns_nonnull)
#else
#  define ATTRS_RETURNS_NONNULL
#endif

/* -------------------------------------------------------------------------- */
/* 5.6 nonnull - Specified arguments must not be NULL                         */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(nonnull) || ATTRS_GCC_AT_LEAST(3, 3, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_NONNULL(...) ATTRS_GCC_ATTR(nonnull(__VA_ARGS__))
#  define ATTRS_NONNULL_ALL ATTRS_GCC_ATTR(nonnull)
#else
#  define ATTRS_NONNULL(...)
#  define ATTRS_NONNULL_ALL
#endif

/* -------------------------------------------------------------------------- */
/* 5.7 returns_twice - Function may return multiple times (e.g., setjmp)      */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(returns_twice) || ATTRS_GCC_AT_LEAST(4, 1, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_RETURNS_TWICE ATTRS_GCC_ATTR(returns_twice)
#else
#  define ATTRS_RETURNS_TWICE
#endif

/* -------------------------------------------------------------------------- */
/* 5.8 leaf - Function does not call back into current unit                   */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(leaf) || ATTRS_GCC_AT_LEAST(4, 6, 0) || \
ATTRS_CLANG_AT_LEAST(3, 4, 0)
#  define ATTRS_LEAF ATTRS_GCC_ATTR(leaf)
#else
#  define ATTRS_LEAF
#endif

/* -------------------------------------------------------------------------- */
/* 5.9 sentinel - Variadic function ends with NULL sentinel                   */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(sentinel) || ATTRS_GCC_AT_LEAST(4, 0, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_SENTINEL ATTRS_GCC_ATTR(sentinel)
#  define ATTRS_SENTINEL_POS(n) ATTRS_GCC_ATTR(sentinel(n))
#else
#  define ATTRS_SENTINEL
#  define ATTRS_SENTINEL_POS(n)
#endif

/* -------------------------------------------------------------------------- */
/* 5.10 format - printf/scanf-like format checking                            */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(format) || ATTRS_GCC_AT_LEAST(2, 95, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_FORMAT(archetype, fmt_idx, first_arg) \
       ATTRS_GCC_ATTR(format(archetype, fmt_idx, first_arg))
#  define ATTRS_FORMAT_ARG(n) ATTRS_GCC_ATTR(format_arg(n))
#else
#  define ATTRS_FORMAT(archetype, fmt_idx, first_arg)
#  define ATTRS_FORMAT_ARG(n)
#endif

/* -------------------------------------------------------------------------- */
/* 5.11 hot / cold - Branch frequency hints                                   */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(hot) || ATTRS_GCC_AT_LEAST(4, 3, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_HOT ATTRS_GCC_ATTR(hot)
#  define ATTRS_COLD ATTRS_GCC_ATTR(cold)
#else
#  define ATTRS_HOT
#  define ATTRS_COLD
#endif

/* -------------------------------------------------------------------------- */
/* 5.12 flatten - Inline all calls inside this function                       */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(flatten) || ATTRS_GCC_AT_LEAST(4, 1, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_FLATTEN ATTRS_GCC_ATTR(flatten)
#else
#  define ATTRS_FLATTEN
#endif

/* ============================================================================
 * 6. Inline Control Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 6.1 always_inline - Force inline                                           */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(always_inline) || ATTRS_GCC_AT_LEAST(3, 1, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_ALWAYS_INLINE inline ATTRS_GCC_ATTR(always_inline)
#elif ATTRS_MSVC_AT_LEAST(1200)
#  define ATTRS_ALWAYS_INLINE __forceinline
#elif ATTRS_COMPILER == ATTRS_COMPILER_INTEL
#  define ATTRS_ALWAYS_INLINE __forceinline
#elif ATTRS_COMPILER == ATTRS_COMPILER_BORLAND
#  define ATTRS_ALWAYS_INLINE __forceinline
#else
#  define ATTRS_ALWAYS_INLINE inline
#endif

/* -------------------------------------------------------------------------- */
/* 6.2 noinline - Prevent inline                                              */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(noinline) || ATTRS_GCC_AT_LEAST(3, 1, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_NOINLINE ATTRS_GCC_ATTR(noinline)
#elif ATTRS_MSVC_AT_LEAST(1300)
#  define ATTRS_NOINLINE ATTRS_MSVC_ATTR(noinline)
#elif ATTRS_COMPILER == ATTRS_COMPILER_INTEL
#  define ATTRS_NOINLINE __declspec(noinline)
#else
#  define ATTRS_NOINLINE
#endif

/* -------------------------------------------------------------------------- */
/* 6.3 gnu_inline - GNU inline semantics                                      */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(gnu_inline) || ATTRS_GCC_AT_LEAST(4, 1, 2) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_GNU_INLINE ATTRS_GCC_ATTR(gnu_inline)
#else
#  define ATTRS_GNU_INLINE
#endif

/* ============================================================================
 * 7. Visibility & Linkage Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 7.1 visibility - Symbol visibility                                         */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(visibility) || ATTRS_GCC_AT_LEAST(4, 0, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_VISIBILITY(type) ATTRS_GCC_ATTR(visibility(#type))
#  define ATTRS_HIDDEN     ATTRS_GCC_ATTR(visibility("hidden"))
#  define ATTRS_DEFAULT    ATTRS_GCC_ATTR(visibility("default"))
#  define ATTRS_PROTECTED  ATTRS_GCC_ATTR(visibility("protected"))
#  define ATTRS_INTERNAL   ATTRS_GCC_ATTR(visibility("internal"))
#else
#  define ATTRS_VISIBILITY(type)
#  define ATTRS_HIDDEN
#  define ATTRS_DEFAULT
#  define ATTRS_PROTECTED
#  define ATTRS_INTERNAL
#endif

/* -------------------------------------------------------------------------- */
/* 7.2 weak - Weak symbol                                                     */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(weak) || ATTRS_GCC_AT_LEAST(2, 95, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_WEAK ATTRS_GCC_ATTR(weak)
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_WEAK ATTRS_KEIL_ATTR(weak)
#elif ATTRS_COMPILER == ATTRS_COMPILER_IAR
#  define ATTRS_WEAK _Pragma("weak")
#else
#  define ATTRS_WEAK
#endif

/* -------------------------------------------------------------------------- */
/* 7.3 weakref - Weak reference to another symbol                             */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(weakref) || ATTRS_GCC_AT_LEAST(4, 0, 0)
#  define ATTRS_WEAKREF(target) ATTRS_GCC_ATTR(weakref(#target))
#else
#  define ATTRS_WEAKREF(target)
#endif

/* -------------------------------------------------------------------------- */
/* 7.4 alias - Symbol alias                                                   */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(alias) || ATTRS_GCC_AT_LEAST(2, 95, 0)
#  define ATTRS_ALIAS(target) ATTRS_GCC_ATTR(alias(#target))
#else
#  define ATTRS_ALIAS(target)
#endif

/* -------------------------------------------------------------------------- */
/* 7.5 used - Keep symbol even if unreferenced                                */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(used) || ATTRS_GCC_AT_LEAST(3, 1, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_USED ATTRS_GCC_ATTR(used)
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_USED ATTRS_KEIL_ATTR(used)
#else
#  define ATTRS_USED
#endif

/* -------------------------------------------------------------------------- */
/* 7.6 externally_visible - Prevent optimization removal                      */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(externally_visible) || ATTRS_GCC_AT_LEAST(4, 1, 0)
#  define ATTRS_EXTERNALLY_VISIBLE ATTRS_GCC_ATTR(externally_visible)
#else
#  define ATTRS_EXTERNALLY_VISIBLE
#endif

/* -------------------------------------------------------------------------- */
/* 7.7 section - Place in specific section                                    */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(section) || ATTRS_GCC_AT_LEAST(2, 95, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_SECTION(name) ATTRS_GCC_ATTR(section(#name))
#elif ATTRS_MSVC_AT_LEAST(1200)
#  define ATTRS_SECTION(name) ATTRS_MSVC_ATTR(allocate(#name))
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_SECTION(name) ATTRS_KEIL_ATTR(section(#name))
#else
#  define ATTRS_SECTION(name)
#endif

/* ============================================================================
 * 8. Constructor / Destructor Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 8.1 constructor - Run before main()                                        */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(constructor) || ATTRS_GCC_AT_LEAST(2, 7, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_CONSTRUCTOR ATTRS_GCC_ATTR(constructor)
#  define ATTRS_CONSTRUCTOR_PRIO(p) ATTRS_GCC_ATTR(constructor(p))
#else
#  define ATTRS_CONSTRUCTOR
#  define ATTRS_CONSTRUCTOR_PRIO(p)
#endif

/* -------------------------------------------------------------------------- */
/* 8.2 destructor - Run at exit()                                             */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(destructor) || ATTRS_GCC_AT_LEAST(2, 7, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_DESTRUCTOR ATTRS_GCC_ATTR(destructor)
#  define ATTRS_DESTRUCTOR_PRIO(p) ATTRS_GCC_ATTR(destructor(p))
#else
#  define ATTRS_DESTRUCTOR
#  define ATTRS_DESTRUCTOR_PRIO(p)
#endif

/* ============================================================================
 * 9. Variable & Type Layout Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 9.1 packed - No padding, compact layout                                    */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(packed) || ATTRS_GCC_AT_LEAST(2, 7, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_PACKED ATTRS_GCC_ATTR(packed)
#elif ATTRS_COMPILER == ATTRS_COMPILER_MSVC
/* MSVC uses #pragma pack */
#  define ATTRS_PACKED
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_PACKED ATTRS_KEIL_ATTR(packed)
#else
#  define ATTRS_PACKED
#endif

/* -------------------------------------------------------------------------- */
/* 9.2 cleanup - Auto-call function on scope exit                             */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(cleanup) || ATTRS_GCC_AT_LEAST(3, 3, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_CLEANUP(func) ATTRS_GCC_ATTR(cleanup(func))
#else
#  define ATTRS_CLEANUP(func)
#endif

/* -------------------------------------------------------------------------- */
/* 9.3 transparent_union - Transparent union type                             */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(transparent_union) || ATTRS_GCC_AT_LEAST(2, 95, 0)
#  define ATTRS_TRANSPARENT_UNION ATTRS_GCC_ATTR(transparent_union)
#else
#  define ATTRS_TRANSPARENT_UNION
#endif

/* -------------------------------------------------------------------------- */
/* 9.4 may_alias - Allow type punning                                         */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(may_alias) || ATTRS_GCC_AT_LEAST(3, 3, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_MAY_ALIAS ATTRS_GCC_ATTR(may_alias)
#else
#  define ATTRS_MAY_ALIAS
#endif

/* -------------------------------------------------------------------------- */
/* 9.5 vector_size - SIMD vector type                                         */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(vector_size) || ATTRS_GCC_AT_LEAST(3, 1, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_VECTOR_SIZE(n) ATTRS_GCC_ATTR(vector_size(n))
#else
#  define ATTRS_VECTOR_SIZE(n)
#endif

/* ============================================================================
 * 10. Sanitizer & Debug Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 10.1 no_sanitize - Disable specific sanitizer checks                       */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(no_sanitize) || ATTRS_GCC_AT_LEAST(4, 9, 0) || \
ATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define ATTRS_NO_SANITIZE(type) ATTRS_GCC_ATTR(no_sanitize(type))
#else
#  define ATTRS_NO_SANITIZE(type)
#endif

/* -------------------------------------------------------------------------- */
/* 10.2 no_sanitize_address - Disable AddressSanitizer                        */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(no_sanitize_address) || ATTRS_CLANG_AT_LEAST(3, 0, 0)
#  define ATTRS_NO_SANITIZE_ADDRESS ATTRS_GCC_ATTR(no_sanitize_address)
#elif ATTRS_HAS_ATTR(no_sanitize) || ATTRS_GCC_AT_LEAST(4, 9, 0)
#  define ATTRS_NO_SANITIZE_ADDRESS ATTRS_GCC_ATTR(no_sanitize("address"))
#else
#  define ATTRS_NO_SANITIZE_ADDRESS
#endif

/* -------------------------------------------------------------------------- */
/* 10.3 no_sanitize_thread - Disable ThreadSanitizer                          */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(no_sanitize_thread) || ATTRS_CLANG_AT_LEAST(3, 0, 0)
#  define ATTRS_NO_SANITIZE_THREAD ATTRS_GCC_ATTR(no_sanitize_thread)
#else
#  define ATTRS_NO_SANITIZE_THREAD
#endif

/* -------------------------------------------------------------------------- */
/* 10.4 no_instrument_function - Skip profiling instrumentation               */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(no_instrument_function) || ATTRS_GCC_AT_LEAST(2, 95, 0)
#  define ATTRS_NO_INSTRUMENT_FUNCTION ATTRS_GCC_ATTR(no_instrument_function)
#else
#  define ATTRS_NO_INSTRUMENT_FUNCTION
#endif

/* -------------------------------------------------------------------------- */
/* 10.5 no_profile_instrument_function - Skip PGO instrumentation             */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(no_profile_instrument_function) || ATTRS_GCC_AT_LEAST(6, 0, 0)
#  define ATTRS_NO_PROFILE_INSTRUMENT_FUNCTION \
       ATTRS_GCC_ATTR(no_profile_instrument_function)
#else
#  define ATTRS_NO_PROFILE_INSTRUMENT_FUNCTION
#endif

/* ============================================================================
 * 11. Target & Architecture Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 11.1 target - Compile with specific target options                         */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(target) || ATTRS_GCC_AT_LEAST(4, 4, 0) || \
ATTRS_CLANG_AT_LEAST(2, 9, 0)
#  define ATTRS_TARGET(...) ATTRS_GCC_ATTR(target(__VA_ARGS__))
#else
#  define ATTRS_TARGET(...)
#endif

/* -------------------------------------------------------------------------- */
/* 11.2 target_clones - Generate multiple versions for different targets      */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(target_clones) || ATTRS_GCC_AT_LEAST(6, 0, 0)
#  define ATTRS_TARGET_CLONES(...) ATTRS_GCC_ATTR(target_clones(__VA_ARGS__))
#else
#  define ATTRS_TARGET_CLONES(...)
#endif

/* -------------------------------------------------------------------------- */
/* 11.3 simd - Enable SIMD cloning                                            */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(simd) || ATTRS_GCC_AT_LEAST(6, 0, 0)
#  define ATTRS_SIMD ATTRS_GCC_ATTR(simd)
#  define ATTRS_SIMD_MASK(m) ATTRS_GCC_ATTR(simd(m))
#else
#  define ATTRS_SIMD
#  define ATTRS_SIMD_MASK(m)
#endif

/* -------------------------------------------------------------------------- */
/* 11.4 ifunc - Indirect function (resolver at runtime)                       */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(ifunc) || ATTRS_GCC_AT_LEAST(4, 8, 0)
#  define ATTRS_IFUNC(resolver) ATTRS_GCC_ATTR(ifunc(#resolver))
#else
#  define ATTRS_IFUNC(resolver)
#endif

/* -------------------------------------------------------------------------- */
/* 11.5 tls_model - Thread-local storage model                               */
/* -------------------------------------------------------------------------- */
#if ATTRS_HAS_ATTR(tls_model) || ATTRS_GCC_AT_LEAST(4, 1, 0) || \
ATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define ATTRS_TLS_MODEL(model) ATTRS_GCC_ATTR(tls_model(#model))
#else
#  define ATTRS_TLS_MODEL(model)
#endif

/* ============================================================================
 * 12. MSVC-Specific Attributes (mapped where possible)
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 12.1 dllexport / dllimport                                                 */
/* -------------------------------------------------------------------------- */
#if ATTRS_COMPILER == ATTRS_COMPILER_MSVC || \
ATTRS_COMPILER == ATTRS_COMPILER_BORLAND || \
ATTRS_COMPILER == ATTRS_COMPILER_WATCOM || \
ATTRS_COMPILER == ATTRS_COMPILER_DIGITAL
#  define ATTRS_DLLEXPORT ATTRS_MSVC_ATTR(dllexport)
#  define ATTRS_DLLIMPORT ATTRS_MSVC_ATTR(dllimport)
#elif ATTRS_COMPILER == ATTRS_COMPILER_GCC && defined(_WIN32)
#  define ATTRS_DLLEXPORT ATTRS_GCC_ATTR(dllexport)
#  define ATTRS_DLLIMPORT ATTRS_GCC_ATTR(dllimport)
#elif ATTRS_COMPILER == ATTRS_COMPILER_CLANG && defined(_WIN32)
#  define ATTRS_DLLEXPORT ATTRS_GCC_ATTR(dllexport)
#  define ATTRS_DLLIMPORT ATTRS_GCC_ATTR(dllimport)
#else
#  define ATTRS_DLLEXPORT
#  define ATTRS_DLLIMPORT
#endif

/* -------------------------------------------------------------------------- */
/* 12.2 naked - No compiler prologue/epilogue                                 */
/* -------------------------------------------------------------------------- */
#if ATTRS_COMPILER == ATTRS_COMPILER_MSVC || \
ATTRS_COMPILER == ATTRS_COMPILER_BORLAND
#  define ATTRS_NAKED ATTRS_MSVC_ATTR(naked)
#elif ATTRS_COMPILER == ATTRS_COMPILER_GCC || \
ATTRS_COMPILER == ATTRS_COMPILER_CLANG || \
ATTRS_COMPILER == ATTRS_COMPILER_INTEL
#  define ATTRS_NAKED ATTRS_GCC_ATTR(naked)
#elif ATTRS_COMPILER == ATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define ATTRS_NAKED ATTRS_KEIL_ATTR(naked)
#else
#  define ATTRS_NAKED
#endif

/* -------------------------------------------------------------------------- */
/* 12.3 thread_local - Thread-local storage                                   */
/* -------------------------------------------------------------------------- */
#if ATTRS_C_STD >= 201112L
#  define ATTRS_THREAD_LOCAL _Thread_local
#elif ATTRS_COMPILER == ATTRS_COMPILER_MSVC || \
ATTRS_COMPILER == ATTRS_COMPILER_BORLAND
#  define ATTRS_THREAD_LOCAL ATTRS_MSVC_ATTR(thread)
#elif ATTRS_COMPILER == ATTRS_COMPILER_GCC || \
ATTRS_COMPILER == ATTRS_COMPILER_CLANG || \
ATTRS_COMPILER == ATTRS_COMPILER_INTEL || \
ATTRS_COMPILER == ATTRS_COMPILER_SUN
#  define ATTRS_THREAD_LOCAL __thread
#else
#  define ATTRS_THREAD_LOCAL
#endif

/* -------------------------------------------------------------------------- */
/* 12.4 restrict - Restrict pointer qualifier                                 */
/* -------------------------------------------------------------------------- */
#if ATTRS_C_STD >= 199901L
#  define ATTRS_RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#  define ATTRS_RESTRICT __restrict__
#elif ATTRS_COMPILER == ATTRS_COMPILER_MSVC
#  define ATTRS_RESTRICT __restrict
#else
#  define ATTRS_RESTRICT
#endif

/* ============================================================================
 * 13. Utility Macros
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 13.1 likely / unlikely - Branch prediction hints                           */
/* -------------------------------------------------------------------------- */
#if (ATTRS_COMPILER == ATTRS_COMPILER_GCC && ATTRS_GCC_AT_LEAST(2, 96, 0)) || \
    (ATTRS_COMPILER == ATTRS_COMPILER_CLANG && ATTRS_CLANG_AT_LEAST(1, 0, 0)) || \
ATTRS_COMPILER == ATTRS_COMPILER_INTEL
#  define ATTRS_LIKELY(x)   __builtin_expect(!!(x), 1)
#  define ATTRS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define ATTRS_LIKELY(x)   (x)
#  define ATTRS_UNLIKELY(x) (x)
#endif

/* -------------------------------------------------------------------------- */
/* 13.2 unreachable - Mark unreachable code                                   */
/* -------------------------------------------------------------------------- */
#if defined(__has_builtin)
#  if __has_builtin(__builtin_unreachable)
#    define ATTRS_UNREACHABLE() __builtin_unreachable()
#  else
#    define ATTRS_UNREACHABLE() ((void)0)
#  endif
#elif (ATTRS_COMPILER == ATTRS_COMPILER_GCC && ATTRS_GCC_AT_LEAST(4, 5, 0)) || \
      (ATTRS_COMPILER == ATTRS_COMPILER_CLANG && ATTRS_CLANG_AT_LEAST(1, 0, 0)) || \
      ATTRS_COMPILER == ATTRS_COMPILER_INTEL
#  define ATTRS_UNREACHABLE() __builtin_unreachable()
#elif ATTRS_COMPILER == ATTRS_COMPILER_MSVC
#  define ATTRS_UNREACHABLE() __assume(0)
#else
#  define ATTRS_UNREACHABLE() ((void)0)
#endif

/* -------------------------------------------------------------------------- */
/* 13.3 assume_aligned - Runtime pointer alignment assumption (builtin)        */
/* -------------------------------------------------------------------------- */
#if defined(__has_builtin)
#  if __has_builtin(__builtin_assume_aligned)
#    define ATTRS_BUILTIN_ASSUME_ALIGNED(ptr, align) \
         __builtin_assume_aligned((ptr), (align))
#  else
#    define ATTRS_BUILTIN_ASSUME_ALIGNED(ptr, align) (ptr)
#  endif
#elif (ATTRS_COMPILER == ATTRS_COMPILER_GCC && ATTRS_GCC_AT_LEAST(4, 7, 0)) || \
      (ATTRS_COMPILER == ATTRS_COMPILER_CLANG && ATTRS_CLANG_AT_LEAST(3, 0, 0)) || \
      ATTRS_COMPILER == ATTRS_COMPILER_INTEL
#  define ATTRS_BUILTIN_ASSUME_ALIGNED(ptr, align) \
       __builtin_assume_aligned((ptr), (align))
#elif ATTRS_COMPILER == ATTRS_COMPILER_MSVC
/* MSVC has no direct equivalent; __assume is too weak */
#  define ATTRS_BUILTIN_ASSUME_ALIGNED(ptr, align) (ptr)
#else
#  define ATTRS_BUILTIN_ASSUME_ALIGNED(ptr, align) (ptr)
#endif

/* -------------------------------------------------------------------------- */
/* 13.4 Static assertion                                                      */
/* -------------------------------------------------------------------------- */
#if ATTRS_C_STD >= 201112L
#  define ATTRS_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#elif defined(__COUNTER__)
#  define ATTRS_STATIC_ASSERT(cond, msg) \
       typedef char ATTRS_CONCAT(_attrs_static_assert_, __COUNTER__)[(cond) ? 1 : -1]
#else
#  define ATTRS_STATIC_ASSERT(cond, msg) \
       typedef char ATTRS_CONCAT(_attrs_static_assert_, __LINE__)[(cond) ? 1 : -1]
#endif

/* Token pasting helpers */
#define ATTRS_CONCAT_(a, b) a ## b
#define ATTRS_CONCAT(a, b) ATTRS_CONCAT_(a, b)

/* -------------------------------------------------------------------------- */
/* 13.5 Alignof                                                               */
/* -------------------------------------------------------------------------- */
#if ATTRS_C_STD >= 201112L
#  define ATTRS_ALIGNOF(type) _Alignof(type)
#elif (ATTRS_COMPILER == ATTRS_COMPILER_GCC && ATTRS_GCC_AT_LEAST(4, 0, 0)) || \
      (ATTRS_COMPILER == ATTRS_COMPILER_CLANG && ATTRS_CLANG_AT_LEAST(1, 0, 0)) || \
ATTRS_COMPILER == ATTRS_COMPILER_INTEL
#  define ATTRS_ALIGNOF(type) __alignof__(type)
#elif ATTRS_COMPILER == ATTRS_COMPILER_MSVC
#  define ATTRS_ALIGNOF(type) __alignof(type)
#else
#  define ATTRS_ALIGNOF(type) (sizeof(type)) /* fallback */
#endif

/* ============================================================================
 * 14. Deprecated / Obsolete Macros (for backward compatibility)
 * ============================================================================ */

/* Some old names that might be found in legacy code */
#define ATTRS_ATTRIBUTE_ALIGNED(n)     ATTRS_ALIGNED(n)
#define ATTRS_ATTRIBUTE_NORETURN       ATTRS_NORETURN
#define ATTRS_ATTRIBUTE_DEPRECATED     ATTRS_DEPRECATED
#define ATTRS_ATTRIBUTE_ALWAYS_INLINE  ATTRS_ALWAYS_INLINE
#define ATTRS_ATTRIBUTE_NOINLINE       ATTRS_NOINLINE
#define ATTRS_ATTRIBUTE_PURE           ATTRS_PURE
#define ATTRS_ATTRIBUTE_CONST          ATTRS_CONST
#define ATTRS_ATTRIBUTE_MALLOC         ATTRS_MALLOC
#define ATTRS_ATTRIBUTE_USED           ATTRS_USED
#define ATTRS_ATTRIBUTE_UNUSED         ATTRS_MAYBE_UNUSED
#define ATTRS_ATTRIBUTE_WEAK           ATTRS_WEAK
#define ATTRS_ATTRIBUTE_PACKED         ATTRS_PACKED
#define ATTRS_ATTRIBUTE_SECTION(name)  ATTRS_SECTION(name)
#define ATTRS_ATTRIBUTE_HOT            ATTRS_HOT
#define ATTRS_ATTRIBUTE_COLD           ATTRS_COLD
#define ATTRS_ATTRIBUTE_VISIBILITY(v)  ATTRS_VISIBILITY(v)

#endif /* ATTRS_H */
