#include "module.h"
#include <stdio.h>

void func_return_void(void)
{
        printf("%s\n", __func__);
}

int func_return_int(int arg)
{
        printf("%s\n", __func__);
        return arg;
}

int *func_return_int_ptr(int *arg)
{
        printf("%s\n", __func__);
        return arg;
}

