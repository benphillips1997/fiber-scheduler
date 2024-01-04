#include <iostream>
#include "scheduler.hpp"

using namespace std;

scheduler s;
void func1();
void func2();
void func3();

void func1() 
{
    int *dp = (int*)s.get_data();
    cout << "fiber 1: " << *dp << endl;
    *dp = *dp += 1;
    fiber f((void*)func2, dp);
    s.spawn(&f);
    s.fiber_exit();
}

void func2()
{
    int *dp = (int*)s.get_data();
    cout << "fiber 2: " << *dp << endl;
    if (*dp < 15) {
        fiber f((void*)func3, dp);
        s.spawn(&f);
    }
    s.fiber_exit();
}

void func3()
{
    int *dp = (int*)s.get_data();
    cout << "fiber 3: " << *dp << endl;
    *dp = *dp += 1;
    fiber f((void*)func1, dp);
    s.spawn(&f);
    s.fiber_exit();
}

int main()
{
    int d = 10;
    int *dp = &d;

    fiber f1((void*)func1, dp);

    s.spawn(&f1);

    s.do_it();

    return 0;
}