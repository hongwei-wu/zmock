/**
 * ZMock unit test mock framework.
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _ZMOCK_H_
#define _ZMOCK_H_
#include <stdint.h>
#include <stdbool.h>

#define zmock_value uintmax_t
typedef void (*zmock_func)(void);

/**
 * Defines a mock return value for a specific function.
 * This macro sets up a mock return value for a function, allowing
 * tests to specify what value should be returned when the function
 * is called.
 */
#define zmock_will_return(func, val) \
        _zmock_will_return(#func, (zmock_value)val, false, __FILE__, __LINE__, \
                           __func__)

/**
 * Sets a persistent mock return value for a specific function.
 *
 * This macro establishes a return value for a function that will persist
 * across multiple calls. This is useful when the function is expected
 * to return the same value consistently throughout the test, without needing
 * to redefine the return value for each individual call.
 */
#define zmock_will_always_return(func, val) \
        _zmock_will_return(#func, (zmock_value)val, true, __FILE__, __LINE__, \
                           __func__)

/**
 * Defines a custom behavior for a specific function call.
 * This macro sets a mock wrapper for a function, allowing tests to
 * substitute the original function implementation with a custom
 * behavior defined by the wrapper.
 */
#define zmock_will_call(func, wrapper) \
        _zmock_will_call(#func, (zmock_func)wrapper, false, __FILE__, __LINE__,\
                         __func__)

/**
 * Macro to specify that a function will be reset in the mock framework.
 */
#define zmock_will_reset(func) _zmock_will_reset(#func)

/**
 * Sets a persistent custom behavior for a specific function call.
 *
 * This macro assigns a custom wrapper function to a mock that will persist
 * across multiple calls. It allows you to define a consistent behavior
 * for the function throughout the test, without needing to redefine
 * the wrapper for each individual call.
 */
#define zmock_will_always_call(func, wrapper) \
        _zmock_will_call(#func, (zmock_func)wrapper, true, __FILE__, __LINE__, \
                         __func__)

/**
 * Get the mock object for a function.
 * This macro uses a mock object, enabling the simulation of functions.
 */
#define zmock_mock(func, type, ...) \
        (_zmock_mock_type(#func) == zmock_type_none) \
                ? (type)__real_##func(__VA_ARGS__) \
                : (_zmock_mock_type(#func) == zmock_type_return) \
                        ? (type)_zmock_mock_value(#func) \
                        : (type)((typeof(func) *)_zmock_mock_func(#func))(__VA_ARGS__)


enum zmock_type {
        zmock_type_none = 0,
        zmock_type_return,
        zmock_type_call,
};

enum zmock_type _zmock_mock_type(const char *name);

zmock_value _zmock_mock_value(const char *name);

zmock_func _zmock_mock_func(const char *name);

void _zmock_will_reset(const char *name);

void _zmock_will_return(const char *name, zmock_value val, bool always,
                        const char *file, int line, const char *func);

void _zmock_will_call(const char *name, zmock_func wrapper, bool always,
                      const char *file, int line, const char *func);

#endif //_ZMOCK_H_
