#include <iostream>
#include "scheduler.hpp"

using namespace std;

scheduler s;
void func1();
void func2();
void func3();

void func1() 
{
    cout << "fiber 1 running" << endl;
    fiber f((void*)func3);
    s.spawn(&f);
    s.fiber_exit();
}

void func2()
{
    cout << "fiber 2 running" << endl;
    s.fiber_exit();
}

void func3()
{
    cout << "fiber 3 running" << endl;
    s.fiber_exit();
}

int main()
{
    fiber f2((void*)func2);
    fiber f1((void*)func1);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    fiber f3((void*)func3);
    s.spawn(&f3);

    return 0;
}