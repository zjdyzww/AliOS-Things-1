/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief Zephyr testing suite
 */

/**
 * @brief Zephyr Tests
 * @defgroup all_tests Zephyr Tests
 * @{
 * @}
 */

#ifndef __ZTEST_H__
#define __ZTEST_H__

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <yunit/yunit.h>

#define TC_PASS 0
#define TC_FAIL 1
#define TC_SKIP 2

#define TC_PRINT(x, ...)   do {  } while (0)
//#define TC_PRINT printf

#ifdef __cplusplus
extern "C" {
#endif

static inline void z_zassert(int cond,
			    const char *default_msg,
			    const char *file,
			    int line, const char *func,
			    const char *msg, ...)
{
	if (!(cond)) {
		va_list vargs;

		va_start(vargs, msg);
		printf("\n    Assertion failed at %s:%d: %s: %s\n",
		      file, line, func, default_msg);
		//vprintk(msg, vargs);
		printf("\n");
		va_end(vargs);
		//ztest_test_fail();
	}
#if CONFIG_ZTEST_ASSERT_VERBOSE == 2
	else {
		printf("\n   Assertion succeeded at %s:%d (%s)\n",
		      file, line, func);
	}
#endif
}

/**
 * @defgroup ztest_assert Ztest assertion macros
 * @ingroup ztest
 *
 * This module provides assertions when using Ztest.
 *
 * @{
 */

/**
 * @brief Fail the test, if @a cond is false
 *
 * You probably don't need to call this macro directly. You should
 * instead use zassert_{condition} macros below.
 *
 * @param cond Condition to check
 * @param msg Optional, can be NULL. Message to print if @a cond is false.
 * @param default_msg Message to print if @a cond is false
 */

#define zassert(cond, default_msg, msg, ...)			    \
	z_zassert(cond, msg ? ("(" default_msg ")") : (default_msg), \
		 __FILE__, __LINE__, __func__, msg ? msg : "", ##__VA_ARGS__)

/**
 * @brief Assert that this function call won't be reached
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_unreachable(msg, ...) zassert(0, "Reached unreachable code", \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is true
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_true(cond, msg, ...) zassert(cond, #cond " is false", \
					     msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a cond is false
 * @param cond Condition to check
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_false(cond, msg, ...) zassert(!(cond), #cond " is true", \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a ptr is NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_is_null(ptr, msg, ...) zassert((ptr) == NULL,	    \
					       #ptr " is not NULL", \
					       msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a ptr is not NULL
 * @param ptr Pointer to compare
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_not_null(ptr, msg, ...) zassert((ptr) != NULL,	      \
						#ptr " is NULL", msg, \
						##__VA_ARGS__)

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_equal(a, b, msg, ...) zassert((a) == (b),	      \
					      #a " not equal to " #b, \
					      msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a a does not equal @a b
 *
 * @a a and @a b won't be converted and will be compared directly.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_not_equal(a, b, msg, ...) zassert((a) != (b),	      \
						  #a " equal to " #b, \
						  msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a a equals @a b
 *
 * @a a and @a b will be converted to `void *` before comparing.
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_equal_ptr(a, b, msg, ...)			    \
	zassert((void *)(a) == (void *)(b), #a " not equal to " #b, \
		msg, ##__VA_ARGS__)

/**
 * @brief Assert that @a a is within @a b with delta @a d
 *
 * @param a Value to compare
 * @param b Value to compare
 * @param d Delta
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_within(a, b, d, msg, ...)			     \
	zassert(((a) > ((b) - (d))) && ((a) < ((b) + (d))),	     \
		#a " not within " #b " +/- " #d,		     \
		msg, ##__VA_ARGS__)

/**
 * @brief Assert that 2 memory buffers have the same contents
 *
 * This macro calls the final memory comparison assertion macro.
 * Using double expansion allows providing some arguments by macros that
 * would expand to more than one values (ANSI-C99 defines that all the macro
 * arguments have to be expanded before macro call).
 *
 * @param ... Arguments, see @ref zassert_mem_equal__
 *            for real arguments accepted.
 */
#define zassert_mem_equal(...) \
	zassert_mem_equal__(__VA_ARGS__)

/**
 * @brief Internal assert that 2 memory buffers have the same contents
 *
 * @note This is internal macro, to be used as a second expansion.
 *       See @ref zassert_mem_equal.
 *
 * @param buf Buffer to compare
 * @param exp Buffer with expected contents
 * @param size Size of buffers
 * @param msg Optional message to print if the assertion fails
 */
#define zassert_mem_equal__(buf, exp, size, msg, ...)                    \
	zassert(memcmp(buf, exp, size) == 0, #buf " not equal to " #exp, \
	msg, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __ZTEST_H__ */
