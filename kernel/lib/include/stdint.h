/*
Copyright (©) 2022-2024  Frosty515

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _STDINT_H
#define _STDINT_H

/* START Normal integer types */

typedef unsigned char uint8_t;
typedef   signed char  int8_t;

typedef unsigned short uint16_t;
typedef          short  int16_t;

typedef unsigned int uint32_t;
typedef          int  int32_t;

typedef unsigned long uint64_t;
typedef          long  int64_t;


#define  INT64_MAX  9223372036854775807L
#define  INT64_MIN -9223372036854775808L
#define UINT64_MAX 18446744073709551615UL

#define  INT32_MAX  2147483647
#define  INT32_MIN -2147483648
#define UINT32_MAX  4294967295U

#define  INT16_MAX  32767
#define  INT16_MIN -32768
#define UINT16_MAX  65535

#define  INT8_MAX  127
#define  INT8_MIN -128
#define UINT8_MAX  255

#define  INT64_C(x) x ## L
#define UINT64_C(x) x ## UL

#define  INT32_C(x) x
#define UINT32_C(x) x ## U

#define  INT16_C(x) x
#define UINT16_C(x) x

#define  INT8_C(x) x
#define UINT8_C(x) x

/* END Normal integer types */



/* START INTMAX types */

typedef unsigned long uintmax_t;
typedef          long  intmax_t;

#define  INTMAX_MAX  9223372036854775807L
#define  INTMAX_MIN -9223372036854775807L
#define UINTMAX_MAX 18446744073709551615UL
#define UINTMAX_MIN                    0

#define  INTMAX_C(x) x ## L
#define UINTMAX_C(x) x ## UL

/* END INTMAX types */



/* START INTPTR types */

typedef unsigned long uintptr_t;
typedef          long  intptr_t;

#define  INTPTR_MAX  9223372036854775807L
#define  INTPTR_MIN -9223372036854775807L
#define UINTPTR_MAX 18446744073709551615UL
#define UINTPTR_MIN                    0

/* END INTPTR types */



/* START Least types */

typedef uint8_t uint_least8_t;
typedef  int8_t  int_least8_t;

typedef uint16_t uint_least16_t;
typedef  int16_t  int_least16_t;

typedef uint32_t uint_least32_t;
typedef  int32_t  int_least32_t;

typedef uint64_t uint_least64_t;
typedef  int64_t  int_least64_t;

#define  INT_LEAST64_MAX  9223372036854775807L
#define  INT_LEAST64_MIN -9223372036854775808L
#define UINT_LEAST64_MAX 18446744073709551615UL
#define UINT_LEAST64_MIN                    0

#define  INT_LEAST32_MAX  2147483647
#define  INT_LEAST32_MIN -2147483648
#define UINT_LEAST32_MAX  4294967295
#define UINT_LEAST32_MIN           0

#define  INT_LEAST16_MAX  32767
#define  INT_LEAST16_MIN -32768
#define UINT_LEAST16_MAX  65535
#define UINT_LEAST16_MIN      0

#define  INT_LEAST8_MAX  127
#define  INT_LEAST8_MIN -128
#define UINT_LEAST8_MAX  256
#define UINT_LEAST8_MIN    0

/* END Least types */



/* START Fast types */

typedef  int8_t  int_fast8_t;
typedef uint8_t uint_fast8_t;

typedef  int64_t  int_fast16_t;
typedef uint64_t uint_fast16_t;

typedef  int64_t  int_fast32_t;
typedef uint64_t uint_fast32_t;

typedef  int64_t  int_fast64_t;
typedef uint64_t uint_fast64_t;

#define  INT_FAST64_MAX  9223372036854775807L
#define  INT_FAST64_MIN -9223372036854775808L
#define UINT_FAST64_MAX 18446744073709551615UL
#define UINT_FAST64_MIN                    0

#define  INT_FAST32_MAX  9223372036854775807L
#define  INT_FAST32_MIN -9223372036854775808L
#define UINT_FAST32_MAX 18446744073709551615UL
#define UINT_FAST32_MIN                    0

#define  INT_FAST16_MAX  9223372036854775807L
#define  INT_FAST16_MIN -9223372036854775808L
#define UINT_FAST16_MAX 18446744073709551615UL
#define UINT_FAST16_MIN                    0

#define  INT_LEAST8_MAX  127
#define  INT_LEAST8_MIN -128
#define UINT_LEAST8_MAX  256
#define UINT_LEAST8_MIN    0

/* END Fast types */


/* START Additional macros */

#define SIZE_MAX 18446744073709551615UL

#define PTRDIFF_MAX 9223372036854775807L
#define PTRDIFF_MIN -9223372036854775808L

/* END Additional macros */

#endif /* _STDINT_H */