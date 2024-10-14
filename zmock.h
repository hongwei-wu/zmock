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
#include <limits.h>

#define zmock_value uintmax_t
typedef void (*zmock_func)(void);

/**
 * Defines a mock return value for a specific function.
 * This macro sets up a mock return value for a function, allowing
 * tests to specify what value should be returned when the function
 * is called.
 */
#define zmock_will_return(wrap, val) \
        _zmock_will_return(#wrap, (zmock_value)val, 1, __FILE__, __LINE__, \
                           __func__)

#define zmock_will_return_count(wrap, val, n) \
        _zmock_will_return(#wrap, (zmock_value)val, n, __FILE__, __LINE__, \
                           __func__)

#define zmock_will_return_always(wrap, val) \
        _zmock_will_return(#wrap, (zmock_value)val, INT_MAX, __FILE__, \
                           __LINE__, __func__)

/**
 * Defines a custom behavior for a specific function call.
 * This macro sets a mock wrapper for a function, allowing tests to
 * substitute the original function implementation with a custom
 * behavior defined by the wrapper.
 */
#define zmock_will_call(wrap, wrapper) \
        _zmock_will_call(#wrap, (zmock_func)wrapper, 1, __FILE__, \
                         __LINE__, __func__)

#define zmock_will_call_count(wrap, wrapper, n) \
        _zmock_will_call(#wrap, (zmock_func)wrapper, n, __FILE__, \
                         __LINE__, __func__)

#define zmock_will_call_always(wrap, wrapper) \
        _zmock_will_call(#wrap, (zmock_func)wrapper, INT_MAX, __FILE__, \
                         __LINE__, __func__)
/**
 * Macro to specify that a function will be reset in the mock framework.
 */
#define zmock_will_reset(wrap) _zmock_will_reset(#wrap)

#define zmock_str(x) #x
/**
 * Get the mock object for a function.
 * This macro uses a mock object, enabling the simulation of functions.
 */
#define zmock_mock(func, type, ...) \
        (_zmock_mock_type(zmock_str(__wrap_##func)) == zmock_type_none) \
                ? (type)__real_##func(__VA_ARGS__) \
                : (_zmock_mock_type(zmock_str(__wrap_##func)) == zmock_type_return) \
                        ? (type)_zmock_mock_value(zmock_str(__wrap_##func)) \
                        : (type)((typeof(__wrap_##func) *)_zmock_mock_func(zmock_str(__wrap_##func)))(__VA_ARGS__)


enum zmock_type {
        zmock_type_none = 0,
        zmock_type_return,
        zmock_type_call,
};

enum zmock_type _zmock_mock_type(const char *name);

zmock_value _zmock_mock_value(const char *name);

zmock_func _zmock_mock_func(const char *name);

void _zmock_will_reset(const char *name);

void _zmock_will_return(const char *name, zmock_value val, int count,
                        const char *file, int line, const char *func);

void _zmock_will_call(const char *name, zmock_func wrapper, int count,
                      const char *file, int line, const char *func);

#endif //_ZMOCK_H_
