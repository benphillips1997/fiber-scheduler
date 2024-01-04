#include <iostream>
#include "scheduler.hpp"
#include "context/context.hpp"

using namespace std;

scheduler s;
void fib();
void print();
void loop();
void inc();


void fib()
{
    int *dp = (int*)s.get_data();

    // spawn fiber to increment data
    fiber incr((void*)inc, dp);
    s.spawn(&incr);

    s.yield();

    // spawn loop fiber
    fiber lp((void*)loop, dp);
    s.spawn(&lp);

    s.fiber_exit();
}

void print()
{    
    int *dp = (int*)s.get_data();
    cout << "data is: " << *dp << endl;

    s.fiber_exit();
}

void loop()
{
    int *dp = (int*)s.get_data();

    // continue to loop and spawn fibers if data is less than 5
    if (*dp < 5) {
        fiber f((void*)fib, dp);
        s.spawn(&f);

        s.yield();

        // spawn fiber to print data
        fiber pr((void*)print, dp);
        s.spawn(&pr);
    }
    
    s.fiber_exit();
}

void inc()
{
    int *dp = (int*)s.get_data();
    // increment data
    *dp += 1;
    s.fiber_exit();
}

int main()
{
    int d = 0;
    int *dp = &d;

    // initially spawn the loop fiber
    fiber f((void*)loop, dp);

    s.spawn(&f);

    s.do_it();

    return 0;
}