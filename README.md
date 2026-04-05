# portable_attributes

Portable C function attributes for GCC, Clang, and MSVC.

## Features

- Cross-compiler attribute abstraction
- Supports C11 / C17 / C23
- Header-only

## Usage

```c
#include <portable_attributes.h>

NODISCARD int foo(void);

```