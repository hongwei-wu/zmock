#include <stdio.h>
#include <assert.h>
#include "module.h"
#include "../zmock.h"

static void mock_func_return_void(void)
{
        printf("%s\n", __func__);
}

static int mock_func_return_int(int arg)
{
        printf("%s\n", __func__);
        return arg + 1;
}

static int *dummy_ptr;
static int *mock_func_return_int_ptr(int *arg)
{
        (void)arg;

        printf("%s\n", __func__);
        return dummy_ptr;
}

int main(int argc, char **argv)
{
        (void)argc;
        (void)argv;

        printf("========= real function\n");
        func_return_void();

        int val = func_return_int(1);
        assert(val == 1);

        int *ptr = func_return_int_ptr(&val);
        assert(ptr == &val);

        printf("========= mock function\n");
        zmock_will_call(func_return_void, mock_func_return_void);
        func_return_void();

        zmock_will_call_count(func_return_int, mock_func_return_int, 2);
        val = func_return_int(1);
        assert(val == 2);

        val = func_return_int(1);
        assert(val == 2);

        zmock_will_call_always(func_return_int_ptr, mock_func_return_int_ptr);
        ptr = func_return_int_ptr(&val);
        assert(ptr == dummy_ptr);

        ptr = func_return_int_ptr(&val);
        assert(ptr == dummy_ptr);

        // reset mocks
        zmock_will_reset(func_return_void);
        zmock_will_reset(func_return_int);
        zmock_will_reset(func_return_int_ptr);

        printf("========= mock return\n");
        zmock_will_return(func_return_void, 0);
        func_return_void();

        zmock_will_return_count(func_return_int, 3, 2);
        val = func_return_int(1);
        assert(val == 3);

        val = func_return_int(1);
        assert(val == 3);

        zmock_will_return_always(func_return_int_ptr, mock_func_return_int_ptr);
        ptr = func_return_int_ptr(&val);
        assert(ptr == (int *)mock_func_return_int_ptr);

        return 0;
}