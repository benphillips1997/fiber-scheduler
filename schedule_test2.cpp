#include <iostream>
#include "scheduler.hpp"

using namespace std;

scheduler s;
void func1();
void func2();

void func1() 
{
    int *dp = (int*)s.get_data();
    cout << "fiber 1: " << *dp << endl;
    *dp = *dp += 1;
    s.fiber_exit();
}

void func2()
{
    int *dp = (int*)s.get_data();
    cout << "fiber 2: " << *dp << endl;
    s.fiber_exit();
}

int main()
{
    int d = 10;
    int *dp = &d;

    fiber f2((void*)func2, dp);
    fiber f1((void*)func1, dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}