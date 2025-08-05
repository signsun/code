#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int my_func(int a, int b)
{
    printf("ret:%d\n", a + b);
    return 0;
}

// 1. 先定义函数类型，通过类型定义指针
void test01()
{
    typedef int(FUNC_TYPE)(int, int);
    FUNC_TYPE *f = my_func;
    // 如何调用？
    (*f)(10, 20);
    f(10, 20);
}

// 2. 定义函数指针类型
void test02()
{
    typedef int (*FUNC_POINTER)(int, int);
    FUNC_POINTER f = my_func;
    // 如何调用？
    (*f)(10, 20);
    f(10, 20);
}

// 3. 直接定义函数指针变量
void test03()
{

    int (*f)(int, int) = my_func;
    // 如何调用？
    (*f)(10, 20);
    f(10, 20);
}

int main()
{
    test01();
    test02();
    test03();
    // test04();
    // test05();
    // test06();

    return 0;
}
