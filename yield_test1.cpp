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
    s.yield();
    cout << "fiber 1 after yield" << endl;
    s.fiber_exit();
}

void func2()
{
    cout << "fiber 2 entered" << endl;
    s.fiber_exit();
}

int main()
{
    fiber f2((void*)func2);
    fiber f1((void*)func1);
    fiber f11((void*)func1);

    s.spawn(&f1);
    s.spawn(&f2);
    s.spawn(&f11);
    s.spawn(&f2);

    s.do_it();


    return 0;
}