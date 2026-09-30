#ifndef TYPES_H
#define TYPES_H

/*
 *
 * def portable typedefs:
 *   i8,  u8,
 *   i16, u16,
 *   i32, u32,
 *   i64, u64
 */

#include <stdint.h>
#include <inttypes.h>
#include <float.h>
#include <limits.h>

/* ===== Integer-types ===== */
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;

_Static_assert(sizeof(i8)  * CHAR_BIT == 8,  "i8 must be 8 bits");
_Static_assert(sizeof(u8)  * CHAR_BIT == 8,  "u8 must be 8 bits");
_Static_assert(sizeof(i16) * CHAR_BIT == 16, "i16 must be 16 bits");
_Static_assert(sizeof(u16) * CHAR_BIT == 16, "u16 must be 16 bits");
_Static_assert(sizeof(i32) * CHAR_BIT == 32, "i32 must be 32 bits");
_Static_assert(sizeof(u32) * CHAR_BIT == 32, "u32 must be 32 bits");
_Static_assert(sizeof(i64) * CHAR_BIT == 64, "i64 must be 64 bits");
_Static_assert(sizeof(u64) * CHAR_BIT == 64, "u64 must be 64 bits");

/* ===== Floating-Point-types ===== */

typedef float  f32;
typedef double f64;

#if LDBL_MANT_DIG > DBL_MANT_DIG
typedef long double f80;
#define HAS_F80 1
#else
#define HAS_F80 0
#endif

_Static_assert(sizeof(f32) <= sizeof(f64), "f32 cannot be larger than f64");

#endif // !TYPES_H