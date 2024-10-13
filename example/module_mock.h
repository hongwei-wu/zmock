#ifndef _MODULE_MOCK_H_
#define _MODULE_MOCK_H_

void __wrap_func_return_void(void);

int __wrap_func_return_int(int arg);

int *__wrap_func_return_int_ptr(int *arg);

#endif//_MODULE_MOCK_H_