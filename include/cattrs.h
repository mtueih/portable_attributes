/**
 * @file cattrs.h
 * @brief Cross-compiler C attributes abstraction macros
 * @version 1.0.0
 *
 * Supports: C89, C99, C11, C17, C23
 * Compilers: GCC, Clang, MSVC, Intel C/C++, TinyCC, SDCC, ARM/Keil, IAR,
 *            Oracle/Sun CC, PGI/NVIDIA HPC, IBM XL, Embarcadero/Borland
 *
 * All macros safely degrade to nothing when unsupported.
 */

#ifndef CATTRS_H
#define CATTRS_H

/* ============================================================================
 * 0. Compiler & Standard Detection
 * ============================================================================ */

/* C standard version */
#if defined(__STDC_VERSION__)
#  define CATTRS_C_STD __STDC_VERSION__
#elif defined(__STDC__)
#  define CATTRS_C_STD 198901L   /* C89/C90 */
#else
#  define CATTRS_C_STD 0L        /* Pre-standard or C++ */
#endif

/* C++ standard version (for mixed C/C++ projects) */
#if defined(__cplusplus)
#  if __cplusplus >= 202002L
#    define CATTRS_CXX_STD 202002L
#  elif __cplusplus >= 201703L
#    define CATTRS_CXX_STD 201703L
#  elif __cplusplus >= 201402L
#    define CATTRS_CXX_STD 201402L
#  elif __cplusplus >= 201103L
#    define CATTRS_CXX_STD 201103L
#  else
#    define CATTRS_CXX_STD 199711L
#  endif
#else
#  define CATTRS_CXX_STD 0L
#endif

/* Compiler detection */
#define CATTRS_COMPILER_UNKNOWN 0
#define CATTRS_COMPILER_GCC     1
#define CATTRS_COMPILER_CLANG   2
#define CATTRS_COMPILER_MSVC    3
#define CATTRS_COMPILER_INTEL   4   /* ICC/ICX */
#define CATTRS_COMPILER_TCC     5   /* TinyCC */
#define CATTRS_COMPILER_SDCC    6   /* Small Device C Compiler */
#define CATTRS_COMPILER_KEIL    7   /* ARM/Keil (armcc, armclang) */
#define CATTRS_COMPILER_IAR     8   /* IAR Embedded Workbench */
#define CATTRS_COMPILER_SUN     9   /* Oracle/Sun Studio */
#define CATTRS_COMPILER_PGI     10  /* PGI / NVIDIA HPC SDK */
#define CATTRS_COMPILER_IBM     11  /* IBM XL C/C++ */
#define CATTRS_COMPILER_BORLAND 12  /* Embarcadero/Borland */
#define CATTRS_COMPILER_WATCOM  13  /* OpenWatcom */
#define CATTRS_COMPILER_DIGITAL 14  /* Digital Mars */

#if defined(__clang__)
#  define CATTRS_COMPILER CATTRS_COMPILER_CLANG
#  define CATTRS_COMPILER_STR "clang"
#  define CATTRS_COMPILER_MAJOR __clang_major__
#  define CATTRS_COMPILER_MINOR __clang_minor__
#  define CATTRS_COMPILER_PATCH __clang_patchlevel__
#elif defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#  define CATTRS_COMPILER CATTRS_COMPILER_INTEL
#  define CATTRS_COMPILER_STR "intel"
#  if defined(__INTEL_COMPILER)
#    define CATTRS_COMPILER_MAJOR (__INTEL_COMPILER / 100)
#    define CATTRS_COMPILER_MINOR (__INTEL_COMPILER % 100 / 10)
#    define CATTRS_COMPILER_PATCH 0
#  else
#    define CATTRS_COMPILER_MAJOR __clang_major__
#    define CATTRS_COMPILER_MINOR __clang_minor__
#    define CATTRS_COMPILER_PATCH __clang_patchlevel__
#  endif
#elif defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#  define CATTRS_COMPILER CATTRS_COMPILER_GCC
#  define CATTRS_COMPILER_STR "gcc"
#  define CATTRS_COMPILER_MAJOR __GNUC__
#  define CATTRS_COMPILER_MINOR __GNUC_MINOR__
#  define CATTRS_COMPILER_PATCH __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
#  define CATTRS_COMPILER CATTRS_COMPILER_MSVC
#  define CATTRS_COMPILER_STR "msvc"
#  define CATTRS_COMPILER_MAJOR (_MSC_VER / 100)
#  define CATTRS_COMPILER_MINOR (_MSC_VER % 100)
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__TINYC__)
#  define CATTRS_COMPILER CATTRS_COMPILER_TCC
#  define CATTRS_COMPILER_STR "tcc"
#  define CATTRS_COMPILER_MAJOR 0
#  define CATTRS_COMPILER_MINOR 0
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__SDCC)
#  define CATTRS_COMPILER CATTRS_COMPILER_SDCC
#  define CATTRS_COMPILER_STR "sdcc"
#  define CATTRS_COMPILER_MAJOR 0
#  define CATTRS_COMPILER_MINOR 0
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#  define CATTRS_COMPILER CATTRS_COMPILER_KEIL
#  define CATTRS_COMPILER_STR "keil"
#  define CATTRS_COMPILER_MAJOR (__ARMCC_VERSION / 1000000)
#  define CATTRS_COMPILER_MINOR (__ARMCC_VERSION % 1000000 / 10000)
#  define CATTRS_COMPILER_PATCH (__ARMCC_VERSION % 10000)
#elif defined(__IAR_SYSTEMS_ICC__)
#  define CATTRS_COMPILER CATTRS_COMPILER_IAR
#  define CATTRS_COMPILER_STR "iar"
#  define CATTRS_COMPILER_MAJOR (__VER__ / 100)
#  define CATTRS_COMPILER_MINOR (__VER__ % 100)
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#  define CATTRS_COMPILER CATTRS_COMPILER_SUN
#  define CATTRS_COMPILER_STR "sun"
#  define CATTRS_COMPILER_MAJOR (__SUNPRO_C / 0x100)
#  define CATTRS_COMPILER_MINOR (__SUNPRO_C % 0x100)
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__PGI) || defined(__NVCOMPILER)
#  define CATTRS_COMPILER CATTRS_COMPILER_PGI
#  define CATTRS_COMPILER_STR "pgi"
#  define CATTRS_COMPILER_MAJOR 0
#  define CATTRS_COMPILER_MINOR 0
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__xlC__) || defined(__IBMC__) || defined(__IBMCPP__)
#  define CATTRS_COMPILER CATTRS_COMPILER_IBM
#  define CATTRS_COMPILER_STR "ibm"
#  define CATTRS_COMPILER_MAJOR 0
#  define CATTRS_COMPILER_MINOR 0
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__BORLANDC__) || defined(__CODEGEARC__)
#  define CATTRS_COMPILER CATTRS_COMPILER_BORLAND
#  define CATTRS_COMPILER_STR "borland"
#  define CATTRS_COMPILER_MAJOR (__BORLANDC__ / 0x100)
#  define CATTRS_COMPILER_MINOR (__BORLANDC__ % 0x100)
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__WATCOMC__)
#  define CATTRS_COMPILER CATTRS_COMPILER_WATCOM
#  define CATTRS_COMPILER_STR "watcom"
#  define CATTRS_COMPILER_MAJOR (__WATCOMC__ / 100)
#  define CATTRS_COMPILER_MINOR (__WATCOMC__ % 100)
#  define CATTRS_COMPILER_PATCH 0
#elif defined(__DMC__)
#  define CATTRS_COMPILER CATTRS_COMPILER_DIGITAL
#  define CATTRS_COMPILER_STR "digitalmars"
#  define CATTRS_COMPILER_MAJOR (__DMC__ / 0x100)
#  define CATTRS_COMPILER_MINOR (__DMC__ % 0x100)
#  define CATTRS_COMPILER_PATCH 0
#else
#  define CATTRS_COMPILER CATTRS_COMPILER_UNKNOWN
#  define CATTRS_COMPILER_STR "unknown"
#  define CATTRS_COMPILER_MAJOR 0
#  define CATTRS_COMPILER_MINOR 0
#  define CATTRS_COMPILER_PATCH 0
#endif

/* ============================================================================
 * 1. Feature Detection Helpers
 * ============================================================================ */

/* __has_attribute (Clang, GCC 5+, some others) */
#if defined(__has_attribute)
#  define CATTRS_HAS_ATTR(x) __has_attribute(x)
#else
#  define CATTRS_HAS_ATTR(x) 0
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
#  define CATTRS_HAS_C_ATTR(x) __has_c_attribute(x)
#elif CATTRS_C_STD >= 202311L
#  define CATTRS_HAS_C_ATTR(x) 1
#else
#  define CATTRS_HAS_C_ATTR(x) 0
#endif

/* __has_cpp_attribute (C++11+) */
#if defined(__has_cpp_attribute)
#  define CATTRS_HAS_CPP_ATTR(x) __has_cpp_attribute(x)
#elif CATTRS_CXX_STD >= 201103L
#  define CATTRS_HAS_CPP_ATTR(x) 1
#else
#  define CATTRS_HAS_CPP_ATTR(x) 0
#endif

/* GCC version check */
#if CATTRS_COMPILER == CATTRS_COMPILER_GCC
#  define CATTRS_GCC_AT_LEAST(maj, min, patch) \
       ((CATTRS_COMPILER_MAJOR > (maj)) || \
        (CATTRS_COMPILER_MAJOR == (maj) && CATTRS_COMPILER_MINOR > (min)) || \
        (CATTRS_COMPILER_MAJOR == (maj) && CATTRS_COMPILER_MINOR == (min) && \
         CATTRS_COMPILER_PATCH >= (patch)))
#else
#  define CATTRS_GCC_AT_LEAST(maj, min, patch) 0
#endif

/* Clang version check */
#if CATTRS_COMPILER == CATTRS_COMPILER_CLANG
#  define CATTRS_CLANG_AT_LEAST(maj, min, patch) \
       ((CATTRS_COMPILER_MAJOR > (maj)) || \
        (CATTRS_COMPILER_MAJOR == (maj) && CATTRS_COMPILER_MINOR > (min)) || \
        (CATTRS_COMPILER_MAJOR == (maj) && CATTRS_COMPILER_MINOR == (min) && \
         CATTRS_COMPILER_PATCH >= (patch)))
#else
#  define CATTRS_CLANG_AT_LEAST(maj, min, patch) 0
#endif

/* MSVC version check */
#if CATTRS_COMPILER == CATTRS_COMPILER_MSVC
#  define CATTRS_MSVC_AT_LEAST(ver) (_MSC_VER >= (ver))
#else
#  define CATTRS_MSVC_AT_LEAST(ver) 0
#endif

/* ============================================================================
 * 2. Attribute Syntax Selection
 * ============================================================================ */

/*
 * Priority:
 * 1. C23 [[...]] syntax (if available)
 * 2. C++11/14/17/20/23 [[...]] syntax (if in C++ mode)
 * 3. GCC __attribute__((...)) syntax
 * 4. MSVC __declspec(...) syntax
 * 5. Empty fallback
 */

/* C23/C++11 bracket syntax */
#if CATTRS_C_STD >= 202311L || CATTRS_CXX_STD >= 201103L
#  define CATTRS_BRACKET_ATTR(x) [[x]]
#else
#  define CATTRS_BRACKET_ATTR(x)
#endif

/* GCC attribute syntax */
#if CATTRS_COMPILER == CATTRS_COMPILER_GCC || \
    CATTRS_COMPILER == CATTRS_COMPILER_CLANG || \
    CATTRS_COMPILER == CATTRS_COMPILER_INTEL || \
    CATTRS_COMPILER == CATTRS_COMPILER_TCC || \
    (CATTRS_COMPILER == CATTRS_COMPILER_KEIL && defined(__GNUC__)) || \
    CATTRS_COMPILER == CATTRS_COMPILER_SUN || \
    CATTRS_COMPILER == CATTRS_COMPILER_PGI || \
    CATTRS_COMPILER == CATTRS_COMPILER_IBM
#  define CATTRS_GCC_ATTR(x) __attribute__((x))
#else
#  define CATTRS_GCC_ATTR(x)
#endif

/* MSVC declspec syntax */
#if CATTRS_COMPILER == CATTRS_COMPILER_MSVC || \
    CATTRS_COMPILER == CATTRS_COMPILER_BORLAND || \
    CATTRS_COMPILER == CATTRS_COMPILER_WATCOM || \
    CATTRS_COMPILER == CATTRS_COMPILER_DIGITAL
#  define CATTRS_MSVC_ATTR(x) __declspec(x)
#else
#  define CATTRS_MSVC_ATTR(x)
#endif

/* IAR pragma syntax */
#if CATTRS_COMPILER == CATTRS_COMPILER_IAR
#  define CATTRS_IAR_PRAGMA(x) _Pragma(#x)
#else
#  define CATTRS_IAR_PRAGMA(x)
#endif

/* SDCC syntax */
#if CATTRS_COMPILER == CATTRS_COMPILER_SDCC
#  define CATTRS_SDCC_ATTR(x) __attribute__((x))
#else
#  define CATTRS_SDCC_ATTR(x)
#endif

/* Keil armcc (non-GNU mode) syntax */
#if CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CATTRS_KEIL_ATTR(x) __attribute__((x))
#else
#  define CATTRS_KEIL_ATTR(x)
#endif

/* ============================================================================
 * 3. Core Attribute Macros
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 3.1 noreturn - Function does not return                                    */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_C_ATTR(noreturn) || CATTRS_C_STD >= 202311L
#  define CAT_NORETURN [[noreturn]]
#elif CATTRS_HAS_CPP_ATTR(noreturn) && CATTRS_CXX_STD >= 201103L
#  define CAT_NORETURN [[noreturn]]
#elif CATTRS_HAS_ATTR(noreturn) || CATTRS_GCC_AT_LEAST(2, 5, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_NORETURN CATTRS_GCC_ATTR(noreturn)
#elif CATTRS_MSVC_AT_LEAST(1200)
#  define CAT_NORETURN CATTRS_MSVC_ATTR(noreturn)
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_NORETURN CATTRS_KEIL_ATTR(noreturn)
#elif CATTRS_C_STD >= 201112L
#  define CAT_NORETURN _Noreturn   /* C11 keyword */
#else
#  define CAT_NORETURN
#endif

/* -------------------------------------------------------------------------- */
/* 3.2 deprecated - Entity is deprecated                                      */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_C_ATTR(deprecated) || CATTRS_C_STD >= 202311L
#  define CAT_DEPRECATED(msg) [[deprecated(msg)]]
#elif CATTRS_HAS_CPP_ATTR(deprecated) && CATTRS_CXX_STD >= 201402L
#  define CAT_DEPRECATED(msg) [[deprecated(msg)]]
#elif CATTRS_HAS_CPP_ATTR(deprecated) && CATTRS_CXX_STD >= 201103L
#  define CAT_DEPRECATED(msg) [[deprecated]]
#elif CATTRS_HAS_ATTR(deprecated) || CATTRS_GCC_AT_LEAST(4, 0, 0) || \
      CATTRS_CLANG_AT_LEAST(2, 9, 0)
#  define CAT_DEPRECATED(msg) CATTRS_GCC_ATTR(deprecated(msg))
#elif CATTRS_MSVC_AT_LEAST(1300)
#  define CAT_DEPRECATED(msg) CATTRS_MSVC_ATTR(deprecated)
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_DEPRECATED(msg) CATTRS_KEIL_ATTR(deprecated)
#else
#  define CAT_DEPRECATED(msg)
#endif

/* -------------------------------------------------------------------------- */
/* 3.3 nodiscard - Return value should not be discarded                       */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_C_ATTR(nodiscard) || CATTRS_C_STD >= 202311L
#  define CAT_NODISCARD(msg) [[nodiscard(msg)]]
#elif CATTRS_HAS_CPP_ATTR(nodiscard) && CATTRS_CXX_STD >= 202002L
#  define CAT_NODISCARD(msg) [[nodiscard(msg)]]
#elif CATTRS_HAS_CPP_ATTR(nodiscard) && CATTRS_CXX_STD >= 201703L
#  define CAT_NODISCARD(msg) [[nodiscard]]
#elif CATTRS_HAS_ATTR(warn_unused_result) || CATTRS_GCC_AT_LEAST(3, 4, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_NODISCARD(msg) CATTRS_GCC_ATTR(warn_unused_result)
#elif CATTRS_MSVC_AT_LEAST(1700) /* VS 2012+ _Check_return_ */
#  define CAT_NODISCARD(msg) _Check_return_
#else
#  define CAT_NODISCARD(msg)
#endif

/* Simple nodiscard without message */
#if CATTRS_HAS_C_ATTR(nodiscard) || CATTRS_C_STD >= 202311L
#  define CAT_NODISCARD_SIMPLE [[nodiscard]]
#elif CATTRS_HAS_CPP_ATTR(nodiscard) && CATTRS_CXX_STD >= 201703L
#  define CAT_NODISCARD_SIMPLE [[nodiscard]]
#elif CATTRS_HAS_ATTR(warn_unused_result) || CATTRS_GCC_AT_LEAST(3, 4, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_NODISCARD_SIMPLE CATTRS_GCC_ATTR(warn_unused_result)
#else
#  define CAT_NODISCARD_SIMPLE
#endif

/* -------------------------------------------------------------------------- */
/* 3.4 maybe_unused - Suppress unused warnings                                */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_C_ATTR(maybe_unused) || CATTRS_C_STD >= 202311L
#  define CAT_MAYBE_UNUSED [[maybe_unused]]
#elif CATTRS_HAS_CPP_ATTR(maybe_unused) && CATTRS_CXX_STD >= 201703L
#  define CAT_MAYBE_UNUSED [[maybe_unused]]
#elif CATTRS_HAS_ATTR(unused) || CATTRS_GCC_AT_LEAST(2, 7, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_MAYBE_UNUSED CATTRS_GCC_ATTR(unused)
#elif CATTRS_MSVC_AT_LEAST(1910) /* VS 2017+ */
#  define CAT_MAYBE_UNUSED CATTRS_MSVC_ATTR(unreferenced)
#else
#  define CAT_MAYBE_UNUSED
#endif

/* -------------------------------------------------------------------------- */
/* 3.5 fallthrough - Intentional switch case fallthrough                      */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_C_ATTR(fallthrough) || CATTRS_C_STD >= 202311L
#  define CAT_FALLTHROUGH [[fallthrough]]
#elif CATTRS_HAS_CPP_ATTR(fallthrough) && CATTRS_CXX_STD >= 201703L
#  define CAT_FALLTHROUGH [[fallthrough]]
#elif CATTRS_HAS_ATTR(fallthrough) || CATTRS_GCC_AT_LEAST(7, 0, 0) || \
      CATTRS_CLANG_AT_LEAST(3, 9, 0)
#  define CAT_FALLTHROUGH CATTRS_GCC_ATTR(fallthrough)
#elif CATTRS_COMPILER == CATTRS_COMPILER_CLANG && \
      CATTRS_CLANG_AT_LEAST(3, 0, 0)
/* Clang also supports __attribute__((fallthrough)) since 3.0 */
#  define CAT_FALLTHROUGH CATTRS_GCC_ATTR(fallthrough)
#else
#  define CAT_FALLTHROUGH ((void)0)
#endif

/* -------------------------------------------------------------------------- */
/* 3.6 unsequenced - Function has no side effects (C23)                       */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_C_ATTR(unsequenced) || CATTRS_C_STD >= 202311L
#  define CAT_UNSEQUENCED [[unsequenced]]
#else
#  define CAT_UNSEQUENCED
#endif

/* -------------------------------------------------------------------------- */
/* 3.7 reproducible - Function has no side effects but may depend on state    */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_C_ATTR(reproducible) || CATTRS_C_STD >= 202311L
#  define CAT_REPRODUCIBLE [[reproducible]]
#else
#  define CAT_REPRODUCIBLE
#endif

/* ============================================================================
 * 4. Alignment Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 4.1 aligned - Variable/type alignment                                      */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(aligned) || CATTRS_GCC_AT_LEAST(2, 95, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_ALIGNED(n) CATTRS_GCC_ATTR(aligned(n))
#elif CATTRS_MSVC_AT_LEAST(1300)
#  define CAT_ALIGNED(n) CATTRS_MSVC_ATTR(align(n))
#elif CATTRS_C_STD >= 201112L
#  define CAT_ALIGNED(n) _Alignas(n)
#elif CATTRS_CXX_STD >= 201103L
#  define CAT_ALIGNED(n) alignas(n)
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_ALIGNED(n) CATTRS_KEIL_ATTR(aligned(n))
#elif CATTRS_COMPILER == CATTRS_COMPILER_IAR
#  define CAT_ALIGNED(n) CATTRS_IAR_PRAGMA(data_alignment = n)
#elif CATTRS_COMPILER == CATTRS_COMPILER_BORLAND
#  define CAT_ALIGNED(n) CATTRS_MSVC_ATTR(align(n))
#else
#  define CAT_ALIGNED(n)
#endif

/* -------------------------------------------------------------------------- */
/* 4.2 assume_aligned - Function returns aligned pointer (GCC 4.9+)           */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(assume_aligned) || CATTRS_GCC_AT_LEAST(4, 9, 0) || \
      CATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define CAT_ASSUME_ALIGNED(align, ...) \
       CATTRS_GCC_ATTR(assume_aligned(align, ## __VA_ARGS__))
#else
#  define CAT_ASSUME_ALIGNED(align, ...)
#endif

/* -------------------------------------------------------------------------- */
/* 4.3 alloc_align - Parameter specifies alignment of allocated memory        */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(alloc_align) || CATTRS_GCC_AT_LEAST(4, 9, 0) || \
      CATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define CAT_ALLOC_ALIGN(n) CATTRS_GCC_ATTR(alloc_align(n))
#else
#  define CAT_ALLOC_ALIGN(n)
#endif

/* ============================================================================
 * 5. Function Behavior Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 5.1 const - Pure computation, no memory read (stricter than pure)          */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(const) || CATTRS_GCC_AT_LEAST(2, 5, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_CONST CATTRS_GCC_ATTR(const)
#else
#  define CAT_CONST
#endif

/* -------------------------------------------------------------------------- */
/* 5.2 pure - No side effects, may read memory                                */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(pure) || CATTRS_GCC_AT_LEAST(2, 96, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_PURE CATTRS_GCC_ATTR(pure)
#else
#  define CAT_PURE
#endif

/* -------------------------------------------------------------------------- */
/* 5.3 malloc-like - Returns freshly allocated memory                         */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(malloc) || CATTRS_GCC_AT_LEAST(2, 96, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_MALLOC CATTRS_GCC_ATTR(malloc)
#elif CATTRS_MSVC_AT_LEAST(1900) /* VS 2015+ */
#  define CAT_MALLOC CATTRS_MSVC_ATTR(allocator)
#else
#  define CAT_MALLOC
#endif

/* -------------------------------------------------------------------------- */
/* 5.4 alloc_size - Parameters specify allocation size                        */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(alloc_size) || CATTRS_GCC_AT_LEAST(4, 3, 0) || \
      CATTRS_CLANG_AT_LEAST(3, 0, 0)
#  define CAT_ALLOC_SIZE(...) CATTRS_GCC_ATTR(alloc_size(__VA_ARGS__))
#else
#  define CAT_ALLOC_SIZE(...)
#endif

/* -------------------------------------------------------------------------- */
/* 5.5 returns_nonnull - Return value is never NULL                           */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(returns_nonnull) || CATTRS_GCC_AT_LEAST(4, 9, 0) || \
      CATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define CAT_RETURNS_NONNULL CATTRS_GCC_ATTR(returns_nonnull)
#else
#  define CAT_RETURNS_NONNULL
#endif

/* -------------------------------------------------------------------------- */
/* 5.6 nonnull - Specified arguments must not be NULL                         */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(nonnull) || CATTRS_GCC_AT_LEAST(3, 3, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_NONNULL(...) CATTRS_GCC_ATTR(nonnull(__VA_ARGS__))
#  define CAT_NONNULL_ALL CATTRS_GCC_ATTR(nonnull)
#else
#  define CAT_NONNULL(...)
#  define CAT_NONNULL_ALL
#endif

/* -------------------------------------------------------------------------- */
/* 5.7 returns_twice - Function may return multiple times (e.g., setjmp)      */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(returns_twice) || CATTRS_GCC_AT_LEAST(4, 1, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_RETURNS_TWICE CATTRS_GCC_ATTR(returns_twice)
#else
#  define CAT_RETURNS_TWICE
#endif

/* -------------------------------------------------------------------------- */
/* 5.8 leaf - Function does not call back into current unit                   */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(leaf) || CATTRS_GCC_AT_LEAST(4, 6, 0) || \
      CATTRS_CLANG_AT_LEAST(3, 4, 0)
#  define CAT_LEAF CATTRS_GCC_ATTR(leaf)
#else
#  define CAT_LEAF
#endif

/* -------------------------------------------------------------------------- */
/* 5.9 sentinel - Variadic function ends with NULL sentinel                   */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(sentinel) || CATTRS_GCC_AT_LEAST(4, 0, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_SENTINEL CATTRS_GCC_ATTR(sentinel)
#  define CAT_SENTINEL_POS(n) CATTRS_GCC_ATTR(sentinel(n))
#else
#  define CAT_SENTINEL
#  define CAT_SENTINEL_POS(n)
#endif

/* -------------------------------------------------------------------------- */
/* 5.10 format - printf/scanf-like format checking                            */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(format) || CATTRS_GCC_AT_LEAST(2, 95, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_FORMAT(archetype, fmt_idx, first_arg) \
       CATTRS_GCC_ATTR(format(archetype, fmt_idx, first_arg))
#  define CAT_FORMAT_ARG(n) CATTRS_GCC_ATTR(format_arg(n))
#else
#  define CAT_FORMAT(archetype, fmt_idx, first_arg)
#  define CAT_FORMAT_ARG(n)
#endif

/* -------------------------------------------------------------------------- */
/* 5.11 hot / cold - Branch frequency hints                                   */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(hot) || CATTRS_GCC_AT_LEAST(4, 3, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_HOT CATTRS_GCC_ATTR(hot)
#  define CAT_COLD CATTRS_GCC_ATTR(cold)
#else
#  define CAT_HOT
#  define CAT_COLD
#endif

/* -------------------------------------------------------------------------- */
/* 5.12 flatten - Inline all calls inside this function                       */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(flatten) || CATTRS_GCC_AT_LEAST(4, 1, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_FLATTEN CATTRS_GCC_ATTR(flatten)
#else
#  define CAT_FLATTEN
#endif

/* ============================================================================
 * 6. Inline Control Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 6.1 always_inline - Force inline                                           */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(always_inline) || CATTRS_GCC_AT_LEAST(3, 1, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_ALWAYS_INLINE inline CATTRS_GCC_ATTR(always_inline)
#elif CATTRS_MSVC_AT_LEAST(1200)
#  define CAT_ALWAYS_INLINE __forceinline
#elif CATTRS_COMPILER == CATTRS_COMPILER_INTEL
#  define CAT_ALWAYS_INLINE __forceinline
#elif CATTRS_COMPILER == CATTRS_COMPILER_BORLAND
#  define CAT_ALWAYS_INLINE __forceinline
#else
#  define CAT_ALWAYS_INLINE inline
#endif

/* -------------------------------------------------------------------------- */
/* 6.2 noinline - Prevent inline                                              */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(noinline) || CATTRS_GCC_AT_LEAST(3, 1, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_NOINLINE CATTRS_GCC_ATTR(noinline)
#elif CATTRS_MSVC_AT_LEAST(1300)
#  define CAT_NOINLINE CATTRS_MSVC_ATTR(noinline)
#elif CATTRS_COMPILER == CATTRS_COMPILER_INTEL
#  define CAT_NOINLINE __declspec(noinline)
#else
#  define CAT_NOINLINE
#endif

/* -------------------------------------------------------------------------- */
/* 6.3 gnu_inline - GNU inline semantics                                      */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(gnu_inline) || CATTRS_GCC_AT_LEAST(4, 1, 2) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_GNU_INLINE CATTRS_GCC_ATTR(gnu_inline)
#else
#  define CAT_GNU_INLINE
#endif

/* ============================================================================
 * 7. Visibility & Linkage Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 7.1 visibility - Symbol visibility                                         */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(visibility) || CATTRS_GCC_AT_LEAST(4, 0, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_VISIBILITY(type) CATTRS_GCC_ATTR(visibility(#type))
#  define CAT_HIDDEN     CATTRS_GCC_ATTR(visibility("hidden"))
#  define CAT_DEFAULT    CATTRS_GCC_ATTR(visibility("default"))
#  define CAT_PROTECTED  CATTRS_GCC_ATTR(visibility("protected"))
#  define CAT_INTERNAL   CATTRS_GCC_ATTR(visibility("internal"))
#else
#  define CAT_VISIBILITY(type)
#  define CAT_HIDDEN
#  define CAT_DEFAULT
#  define CAT_PROTECTED
#  define CAT_INTERNAL
#endif

/* -------------------------------------------------------------------------- */
/* 7.2 weak - Weak symbol                                                     */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(weak) || CATTRS_GCC_AT_LEAST(2, 95, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_WEAK CATTRS_GCC_ATTR(weak)
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_WEAK CATTRS_KEIL_ATTR(weak)
#elif CATTRS_COMPILER == CATTRS_COMPILER_IAR
#  define CAT_WEAK _Pragma("weak")
#else
#  define CAT_WEAK
#endif

/* -------------------------------------------------------------------------- */
/* 7.3 weakref - Weak reference to another symbol                             */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(weakref) || CATTRS_GCC_AT_LEAST(4, 0, 0)
#  define CAT_WEAKREF(target) CATTRS_GCC_ATTR(weakref(#target))
#else
#  define CAT_WEAKREF(target)
#endif

/* -------------------------------------------------------------------------- */
/* 7.4 alias - Symbol alias                                                   */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(alias) || CATTRS_GCC_AT_LEAST(2, 95, 0)
#  define CAT_ALIAS(target) CATTRS_GCC_ATTR(alias(#target))
#else
#  define CAT_ALIAS(target)
#endif

/* -------------------------------------------------------------------------- */
/* 7.5 used - Keep symbol even if unreferenced                                */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(used) || CATTRS_GCC_AT_LEAST(3, 1, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_USED CATTRS_GCC_ATTR(used)
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_USED CATTRS_KEIL_ATTR(used)
#else
#  define CAT_USED
#endif

/* -------------------------------------------------------------------------- */
/* 7.6 externally_visible - Prevent optimization removal                      */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(externally_visible) || CATTRS_GCC_AT_LEAST(4, 1, 0)
#  define CAT_EXTERNALLY_VISIBLE CATTRS_GCC_ATTR(externally_visible)
#else
#  define CAT_EXTERNALLY_VISIBLE
#endif

/* -------------------------------------------------------------------------- */
/* 7.7 section - Place in specific section                                    */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(section) || CATTRS_GCC_AT_LEAST(2, 95, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_SECTION(name) CATTRS_GCC_ATTR(section(#name))
#elif CATTRS_MSVC_AT_LEAST(1200)
#  define CAT_SECTION(name) CATTRS_MSVC_ATTR(allocate(#name))
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_SECTION(name) CATTRS_KEIL_ATTR(section(#name))
#else
#  define CAT_SECTION(name)
#endif

/* ============================================================================
 * 8. Constructor / Destructor Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 8.1 constructor - Run before main()                                        */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(constructor) || CATTRS_GCC_AT_LEAST(2, 7, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_CONSTRUCTOR CATTRS_GCC_ATTR(constructor)
#  define CAT_CONSTRUCTOR_PRIO(p) CATTRS_GCC_ATTR(constructor(p))
#else
#  define CAT_CONSTRUCTOR
#  define CAT_CONSTRUCTOR_PRIO(p)
#endif

/* -------------------------------------------------------------------------- */
/* 8.2 destructor - Run at exit()                                             */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(destructor) || CATTRS_GCC_AT_LEAST(2, 7, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_DESTRUCTOR CATTRS_GCC_ATTR(destructor)
#  define CAT_DESTRUCTOR_PRIO(p) CATTRS_GCC_ATTR(destructor(p))
#else
#  define CAT_DESTRUCTOR
#  define CAT_DESTRUCTOR_PRIO(p)
#endif

/* ============================================================================
 * 9. Variable & Type Layout Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 9.1 packed - No padding, compact layout                                    */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(packed) || CATTRS_GCC_AT_LEAST(2, 7, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_PACKED CATTRS_GCC_ATTR(packed)
#elif CATTRS_COMPILER == CATTRS_COMPILER_MSVC
/* MSVC uses #pragma pack */
#  define CAT_PACKED
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_PACKED CATTRS_KEIL_ATTR(packed)
#else
#  define CAT_PACKED
#endif

/* -------------------------------------------------------------------------- */
/* 9.2 cleanup - Auto-call function on scope exit                             */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(cleanup) || CATTRS_GCC_AT_LEAST(3, 3, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_CLEANUP(func) CATTRS_GCC_ATTR(cleanup(func))
#else
#  define CAT_CLEANUP(func)
#endif

/* -------------------------------------------------------------------------- */
/* 9.3 transparent_union - Transparent union type                             */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(transparent_union) || CATTRS_GCC_AT_LEAST(2, 95, 0)
#  define CAT_TRANSPARENT_UNION CATTRS_GCC_ATTR(transparent_union)
#else
#  define CAT_TRANSPARENT_UNION
#endif

/* -------------------------------------------------------------------------- */
/* 9.4 may_alias - Allow type punning                                         */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(may_alias) || CATTRS_GCC_AT_LEAST(3, 3, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_MAY_ALIAS CATTRS_GCC_ATTR(may_alias)
#else
#  define CAT_MAY_ALIAS
#endif

/* -------------------------------------------------------------------------- */
/* 9.5 vector_size - SIMD vector type                                         */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(vector_size) || CATTRS_GCC_AT_LEAST(3, 1, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_VECTOR_SIZE(n) CATTRS_GCC_ATTR(vector_size(n))
#else
#  define CAT_VECTOR_SIZE(n)
#endif

/* ============================================================================
 * 10. Sanitizer & Debug Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 10.1 no_sanitize - Disable specific sanitizer checks                       */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(no_sanitize) || CATTRS_GCC_AT_LEAST(4, 9, 0) || \
      CATTRS_CLANG_AT_LEAST(3, 6, 0)
#  define CAT_NO_SANITIZE(type) CATTRS_GCC_ATTR(no_sanitize(type))
#else
#  define CAT_NO_SANITIZE(type)
#endif

/* -------------------------------------------------------------------------- */
/* 10.2 no_sanitize_address - Disable AddressSanitizer                        */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(no_sanitize_address) || CATTRS_CLANG_AT_LEAST(3, 0, 0)
#  define CAT_NO_SANITIZE_ADDRESS CATTRS_GCC_ATTR(no_sanitize_address)
#elif CATTRS_HAS_ATTR(no_sanitize) || CATTRS_GCC_AT_LEAST(4, 9, 0)
#  define CAT_NO_SANITIZE_ADDRESS CATTRS_GCC_ATTR(no_sanitize("address"))
#else
#  define CAT_NO_SANITIZE_ADDRESS
#endif

/* -------------------------------------------------------------------------- */
/* 10.3 no_sanitize_thread - Disable ThreadSanitizer                          */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(no_sanitize_thread) || CATTRS_CLANG_AT_LEAST(3, 0, 0)
#  define CAT_NO_SANITIZE_THREAD CATTRS_GCC_ATTR(no_sanitize_thread)
#else
#  define CAT_NO_SANITIZE_THREAD
#endif

/* -------------------------------------------------------------------------- */
/* 10.4 no_instrument_function - Skip profiling instrumentation               */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(no_instrument_function) || CATTRS_GCC_AT_LEAST(2, 95, 0)
#  define CAT_NO_INSTRUMENT_FUNCTION CATTRS_GCC_ATTR(no_instrument_function)
#else
#  define CAT_NO_INSTRUMENT_FUNCTION
#endif

/* -------------------------------------------------------------------------- */
/* 10.5 no_profile_instrument_function - Skip PGO instrumentation             */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(no_profile_instrument_function) || CATTRS_GCC_AT_LEAST(6, 0, 0)
#  define CAT_NO_PROFILE_INSTRUMENT_FUNCTION \
       CATTRS_GCC_ATTR(no_profile_instrument_function)
#else
#  define CAT_NO_PROFILE_INSTRUMENT_FUNCTION
#endif

/* ============================================================================
 * 11. Target & Architecture Attributes
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 11.1 target - Compile with specific target options                         */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(target) || CATTRS_GCC_AT_LEAST(4, 4, 0) || \
      CATTRS_CLANG_AT_LEAST(2, 9, 0)
#  define CAT_TARGET(...) CATTRS_GCC_ATTR(target(__VA_ARGS__))
#else
#  define CAT_TARGET(...)
#endif

/* -------------------------------------------------------------------------- */
/* 11.2 target_clones - Generate multiple versions for different targets      */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(target_clones) || CATTRS_GCC_AT_LEAST(6, 0, 0)
#  define CAT_TARGET_CLONES(...) CATTRS_GCC_ATTR(target_clones(__VA_ARGS__))
#else
#  define CAT_TARGET_CLONES(...)
#endif

/* -------------------------------------------------------------------------- */
/* 11.3 simd - Enable SIMD cloning                                            */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(simd) || CATTRS_GCC_AT_LEAST(6, 0, 0)
#  define CAT_SIMD CATTRS_GCC_ATTR(simd)
#  define CAT_SIMD_MASK(m) CATTRS_GCC_ATTR(simd(m))
#else
#  define CAT_SIMD
#  define CAT_SIMD_MASK(m)
#endif

/* -------------------------------------------------------------------------- */
/* 11.4 ifunc - Indirect function (resolver at runtime)                       */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(ifunc) || CATTRS_GCC_AT_LEAST(4, 8, 0)
#  define CAT_IFUNC(resolver) CATTRS_GCC_ATTR(ifunc(#resolver))
#else
#  define CAT_IFUNC(resolver)
#endif

/* -------------------------------------------------------------------------- */
/* 11.5 tls_model - Thread-local storage model                               */
/* -------------------------------------------------------------------------- */
#if CATTRS_HAS_ATTR(tls_model) || CATTRS_GCC_AT_LEAST(4, 1, 0) || \
      CATTRS_CLANG_AT_LEAST(1, 0, 0)
#  define CAT_TLS_MODEL(model) CATTRS_GCC_ATTR(tls_model(#model))
#else
#  define CAT_TLS_MODEL(model)
#endif

/* ============================================================================
 * 12. MSVC-Specific Attributes (mapped where possible)
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 12.1 dllexport / dllimport                                                 */
/* -------------------------------------------------------------------------- */
#if CATTRS_COMPILER == CATTRS_COMPILER_MSVC || \
    CATTRS_COMPILER == CATTRS_COMPILER_BORLAND || \
    CATTRS_COMPILER == CATTRS_COMPILER_WATCOM || \
    CATTRS_COMPILER == CATTRS_COMPILER_DIGITAL
#  define CAT_DLLEXPORT CATTRS_MSVC_ATTR(dllexport)
#  define CAT_DLLIMPORT CATTRS_MSVC_ATTR(dllimport)
#elif CATTRS_COMPILER == CATTRS_COMPILER_GCC && defined(_WIN32)
#  define CAT_DLLEXPORT CATTRS_GCC_ATTR(dllexport)
#  define CAT_DLLIMPORT CATTRS_GCC_ATTR(dllimport)
#elif CATTRS_COMPILER == CATTRS_COMPILER_CLANG && defined(_WIN32)
#  define CAT_DLLEXPORT CATTRS_GCC_ATTR(dllexport)
#  define CAT_DLLIMPORT CATTRS_GCC_ATTR(dllimport)
#else
#  define CAT_DLLEXPORT
#  define CAT_DLLIMPORT
#endif

/* -------------------------------------------------------------------------- */
/* 12.2 naked - No compiler prologue/epilogue                                 */
/* -------------------------------------------------------------------------- */
#if CATTRS_COMPILER == CATTRS_COMPILER_MSVC || \
    CATTRS_COMPILER == CATTRS_COMPILER_BORLAND
#  define CAT_NAKED CATTRS_MSVC_ATTR(naked)
#elif CATTRS_COMPILER == CATTRS_COMPILER_GCC || \
      CATTRS_COMPILER == CATTRS_COMPILER_CLANG || \
      CATTRS_COMPILER == CATTRS_COMPILER_INTEL
#  define CAT_NAKED CATTRS_GCC_ATTR(naked)
#elif CATTRS_COMPILER == CATTRS_COMPILER_KEIL && !defined(__GNUC__)
#  define CAT_NAKED CATTRS_KEIL_ATTR(naked)
#else
#  define CAT_NAKED
#endif

/* -------------------------------------------------------------------------- */
/* 12.3 thread_local - Thread-local storage                                   */
/* -------------------------------------------------------------------------- */
#if CATTRS_C_STD >= 201112L
#  define CAT_THREAD_LOCAL _Thread_local
#elif CATTRS_CXX_STD >= 201103L
#  define CAT_THREAD_LOCAL thread_local
#elif CATTRS_COMPILER == CATTRS_COMPILER_MSVC || \
      CATTRS_COMPILER == CATTRS_COMPILER_BORLAND
#  define CAT_THREAD_LOCAL CATTRS_MSVC_ATTR(thread)
#elif CATTRS_COMPILER == CATTRS_COMPILER_GCC || \
      CATTRS_COMPILER == CATTRS_COMPILER_CLANG || \
      CATTRS_COMPILER == CATTRS_COMPILER_INTEL || \
      CATTRS_COMPILER == CATTRS_COMPILER_SUN
#  define CAT_THREAD_LOCAL __thread
#else
#  define CAT_THREAD_LOCAL
#endif

/* -------------------------------------------------------------------------- */
/* 12.4 restrict - Restrict pointer qualifier                                 */
/* -------------------------------------------------------------------------- */
#if CATTRS_C_STD >= 199901L
#  define CAT_RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#  define CAT_RESTRICT __restrict__
#elif CATTRS_COMPILER == CATTRS_COMPILER_MSVC
#  define CAT_RESTRICT __restrict
#else
#  define CAT_RESTRICT
#endif

/* ============================================================================
 * 13. Utility Macros
 * ============================================================================ */

/* -------------------------------------------------------------------------- */
/* 13.1 likely / unlikely - Branch prediction hints                           */
/* -------------------------------------------------------------------------- */
#if (CATTRS_COMPILER == CATTRS_COMPILER_GCC && CATTRS_GCC_AT_LEAST(2, 96, 0)) || \
    (CATTRS_COMPILER == CATTRS_COMPILER_CLANG && CATTRS_CLANG_AT_LEAST(1, 0, 0)) || \
    CATTRS_COMPILER == CATTRS_COMPILER_INTEL
#  define CAT_LIKELY(x)   __builtin_expect(!!(x), 1)
#  define CAT_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define CAT_LIKELY(x)   (x)
#  define CAT_UNLIKELY(x) (x)
#endif

/* -------------------------------------------------------------------------- */
/* 13.2 unreachable - Mark unreachable code                                   */
/* -------------------------------------------------------------------------- */
#if defined(__has_builtin)
#  if __has_builtin(__builtin_unreachable)
#    define CAT_UNREACHABLE() __builtin_unreachable()
#  else
#    define CAT_UNREACHABLE() ((void)0)
#  endif
#elif (CATTRS_COMPILER == CATTRS_COMPILER_GCC && CATTRS_GCC_AT_LEAST(4, 5, 0)) || \
      (CATTRS_COMPILER == CATTRS_COMPILER_CLANG && CATTRS_CLANG_AT_LEAST(1, 0, 0)) || \
      CATTRS_COMPILER == CATTRS_COMPILER_INTEL
#  define CAT_UNREACHABLE() __builtin_unreachable()
#elif CATTRS_COMPILER == CATTRS_COMPILER_MSVC
#  define CAT_UNREACHABLE() __assume(0)
#else
#  define CAT_UNREACHABLE() ((void)0)
#endif

/* -------------------------------------------------------------------------- */
/* 13.3 assume_aligned - Runtime pointer alignment assumption (builtin)        */
/* -------------------------------------------------------------------------- */
#if defined(__has_builtin)
#  if __has_builtin(__builtin_assume_aligned)
#    define CAT_BUILTIN_ASSUME_ALIGNED(ptr, align) \
         __builtin_assume_aligned((ptr), (align))
#  else
#    define CAT_BUILTIN_ASSUME_ALIGNED(ptr, align) (ptr)
#  endif
#elif (CATTRS_COMPILER == CATTRS_COMPILER_GCC && CATTRS_GCC_AT_LEAST(4, 7, 0)) || \
      (CATTRS_COMPILER == CATTRS_COMPILER_CLANG && CATTRS_CLANG_AT_LEAST(3, 0, 0)) || \
      CATTRS_COMPILER == CATTRS_COMPILER_INTEL
#  define CAT_BUILTIN_ASSUME_ALIGNED(ptr, align) \
       __builtin_assume_aligned((ptr), (align))
#elif CATTRS_COMPILER == CATTRS_COMPILER_MSVC
/* MSVC has no direct equivalent; __assume is too weak */
#  define CAT_BUILTIN_ASSUME_ALIGNED(ptr, align) (ptr)
#else
#  define CAT_BUILTIN_ASSUME_ALIGNED(ptr, align) (ptr)
#endif

/* -------------------------------------------------------------------------- */
/* 13.4 Static assertion                                                      */
/* -------------------------------------------------------------------------- */
#if CATTRS_C_STD >= 201112L
#  define CAT_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#elif CATTRS_CXX_STD >= 201103L
#  define CAT_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#elif defined(__COUNTER__)
#  define CAT_STATIC_ASSERT(cond, msg) \
       typedef char CAT_CONCAT(_cat_static_assert_, __COUNTER__)[(cond) ? 1 : -1]
#else
#  define CAT_STATIC_ASSERT(cond, msg) \
       typedef char CAT_CONCAT(_cat_static_assert_, __LINE__)[(cond) ? 1 : -1]
#endif

/* Token pasting helpers */
#define CAT_CONCAT_(a, b) a ## b
#define CAT_CONCAT(a, b) CAT_CONCAT_(a, b)

/* -------------------------------------------------------------------------- */
/* 13.5 Alignof                                                               */
/* -------------------------------------------------------------------------- */
#if CATTRS_C_STD >= 201112L
#  define CAT_ALIGNOF(type) _Alignof(type)
#elif CATTRS_CXX_STD >= 201103L
#  define CAT_ALIGNOF(type) alignof(type)
#elif (CATTRS_COMPILER == CATTRS_COMPILER_GCC && CATTRS_GCC_AT_LEAST(4, 0, 0)) || \
      (CATTRS_COMPILER == CATTRS_COMPILER_CLANG && CATTRS_CLANG_AT_LEAST(1, 0, 0)) || \
      CATTRS_COMPILER == CATTRS_COMPILER_INTEL
#  define CAT_ALIGNOF(type) __alignof__(type)
#elif CATTRS_COMPILER == CATTRS_COMPILER_MSVC
#  define CAT_ALIGNOF(type) __alignof(type)
#else
#  define CAT_ALIGNOF(type) (sizeof(type)) /* fallback */
#endif

/* ============================================================================
 * 14. Deprecated / Obsolete Macros (for backward compatibility)
 * ============================================================================ */

/* Some old names that might be found in legacy code */
#define CAT_ATTRIBUTE_ALIGNED(n)     CAT_ALIGNED(n)
#define CAT_ATTRIBUTE_NORETURN       CAT_NORETURN
#define CAT_ATTRIBUTE_DEPRECATED     CAT_DEPRECATED
#define CAT_ATTRIBUTE_ALWAYS_INLINE  CAT_ALWAYS_INLINE
#define CAT_ATTRIBUTE_NOINLINE       CAT_NOINLINE
#define CAT_ATTRIBUTE_PURE           CAT_PURE
#define CAT_ATTRIBUTE_CONST          CAT_CONST
#define CAT_ATTRIBUTE_MALLOC         CAT_MALLOC
#define CAT_ATTRIBUTE_USED           CAT_USED
#define CAT_ATTRIBUTE_UNUSED         CAT_MAYBE_UNUSED
#define CAT_ATTRIBUTE_WEAK           CAT_WEAK
#define CAT_ATTRIBUTE_PACKED         CAT_PACKED
#define CAT_ATTRIBUTE_SECTION(name)  CAT_SECTION(name)
#define CAT_ATTRIBUTE_HOT            CAT_HOT
#define CAT_ATTRIBUTE_COLD           CAT_COLD
#define CAT_ATTRIBUTE_VISIBILITY(v)  CAT_VISIBILITY(v)

#endif /* CATTRS_H */
