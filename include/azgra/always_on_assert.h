/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 *	ISO C99 Standard: 7.2 Diagnostics	<assert.h> 
 */

/*
    Moravec:
        Always enabled assert.
*/

#ifdef _WIN32
#include <corecrt.h>

_ACRTIMP void __cdecl _always_wassert(
	_In_z_ wchar_t const* _Message,
	_In_z_ wchar_t const* _File,
	_In_   unsigned       _Line
);

#define always_assert(expression) (void)(                                                       \
            (!!(expression)) ||                                                              \
            (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
        )
#else


#include <cassert>
// #define _ASSERT_H 1
#include <features.h>

#ifndef _ASSERT_H_DECLS
#define _ASSERT_H_DECLS
__BEGIN_DECLS

/* This prints an "assertion failed" message and aborts.  */
extern void __assert_fail(const char *__assertion, const char *__file,
                          unsigned int __line, const char *__function)
    __THROW __attribute__((__noreturn__));

/* Likewise, but prints the error text for ERRNUM.  */
extern void __assert_perror_fail(int __errnum, const char *__file,
                                 unsigned int __line, const char *__function)
    __THROW __attribute__((__noreturn__));

/* The following is not at all used here but needed for standard
   compliance.  */
extern void __assert(const char *__assertion, const char *__file, int __line)
    __THROW __attribute__((__noreturn__));

__END_DECLS
#endif /* Not _assert_H_DECLS */

#if defined __cplusplus ? __GNUC_PREREQ(2, 6) : __GNUC_PREREQ(2, 4)
#define __assert_FUNCTION __extension__ __PRETTY_FUNCTION__
#else
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#define __assert_FUNCTION __func__
#else
#define __assert_FUNCTION ((const char *)0)
#endif
#endif

#if defined __cplusplus
#define always_assert(expr)  \
    (static_cast<bool>(expr) \
         ? void(0)           \
         : __assert_fail(#expr, __FILE__, __LINE__, __assert_FUNCTION))
#endif
#if defined __USE_ISOC11 && !defined __cplusplus
#undef static_assert
#define static_assert _Static_assert
#endif
#endif