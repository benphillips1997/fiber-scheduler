#include <iostream>
#include "scheduler.hpp"
#include "context/context.hpp"

using namespace std;

scheduler s;
void func1();
void func2();


void func1()
{
    cout << "fiber 1 before yield" << endl;
    int *dp = (int*)s.get_data();
    cout << "data is " << *dp << endl;
    s.yield();
    cout << "fiber 1 after yield" << endl;
    dp = (int*)s.get_data();
    cout << "data is " << *dp << endl;
    s.fiber_exit();
}

void func2()
{
    cout << "fiber 2 entered" << endl;
    int *dp = (int*)s.get_data();
    *dp = 3;
    s.fiber_exit();
}

int main()
{
    int d = 42;
    int *dp = &d;

    fiber f2((void*)func2, dp);
    fiber f1((void*)func1, dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();


    return 0;
}