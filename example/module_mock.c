#include "module_mock.h"
#include "module.h"
#include "../zmock.h"
#include <stdio.h>

extern void __real_func_return_void(void);

void __wrap_func_return_void(void)
{
        zmock_mock(func_return_void, void);
}

extern int __real_func_return_int(int arg);

int __wrap_func_return_int(int arg)
{
        return zmock_mock(func_return_int, int, arg);
}

extern int *__real_func_return_int_ptr(int *arg);

int *__wrap_func_return_int_ptr(int *arg)
{
        return zmock_mock(func_return_int_ptr, int*, arg);
}

