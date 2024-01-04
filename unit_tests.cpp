#include <iostream>
#include "simpletest/simpletest.h"
#include "context/context.hpp"
#include "fiber.hpp"
#include "scheduler.hpp"

using namespace std;

char const *groups[] = {
    "data_sharing1",
    "data_sharing2",
    "data_sharing3",
    "yield_data1",
    "yield_data2",
    "yield_data3",
    "yield_data4",
};

scheduler s;

// ----------- Data sharing tests -------------
void data_sharing1_func1();
void data_sharing1_func2();

// test that changing the data in different fibers changes it everywhere
DEFINE_TEST_G(Data_sharing1, data_sharing1)
{
    int d = 10;
    int *dp = &d;

    fiber f1((void*)data_sharing1_func1, dp);
    fiber f2((void*)data_sharing1_func2, dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    TEST_MESSAGE(d == 25, "d should be 25");
}

void data_sharing1_func1()
{
    int *dp = (int*)s.get_data();
    *dp *= 2;
    TEST_MESSAGE(*dp == 20, "Should pass, should equal 20");
    s.fiber_exit();
}

void data_sharing1_func2()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 20, "Should pass, should equal 20");
    *dp += 5;
    s.fiber_exit();
}


void data_sharing2_func1();

// check that when not setting data that it is set to nullptr
DEFINE_TEST_G(Data_sharing2, data_sharing2)
{
    fiber f((void*)data_sharing2_func1);
    s.spawn(&f);

    s.do_it();
}

void data_sharing2_func1()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(dp == nullptr, "Should be a nullptr");
    s.fiber_exit();
}


void data_sharing3_func1();
void data_sharing3_func2();
void data_sharing3_func3();

// another test to check data sharing works
DEFINE_TEST_G(Data_sharing3, data_sharing3)
{
    char c = 'a';
    char *cp = &c;

    fiber f((void*)data_sharing3_func1, cp);
    s.spawn(&f);

    s.do_it();

    TEST_MESSAGE(*cp == 'd', "Should be 'd'");
}

void data_sharing3_func1()
{
    char *cp = (char*)s.get_data();
    TEST_MESSAGE(*cp == 'a', "Should be 'a'");
    *cp = 'b';
    fiber f((void*)data_sharing3_func2, cp);
    s.spawn(&f);
    s.fiber_exit();
}

void data_sharing3_func2()
{
    char *cp = (char*)s.get_data();
    TEST_MESSAGE(*cp == 'b', "Should be 'b'");
    *cp = 'c';
    fiber f((void*)data_sharing3_func3, cp);
    s.spawn(&f);
    s.fiber_exit();
}

void data_sharing3_func3()
{
    char *cp = (char*)s.get_data();
    TEST_MESSAGE(*cp == 'c', "Should be 'c'");
    *cp = 'd';
    s.fiber_exit();
}



// ----------- Yield data sharing tests ------------
void yield_data1_func1();
void yield_data1_func2();

// check the running order is correct when using yield
DEFINE_TEST_G(Yield_data1, yield_data1)
{
    int d = 1;
    int *dp = &d;

    fiber f2((void*)yield_data1_func2, dp);
    fiber f1((void*)yield_data1_func1, dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();
}

void yield_data1_func1()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 1, "Should be 1");
    *dp += 1;
    s.yield();
    TEST_MESSAGE(*dp == 3, "Should be 3");
    s.fiber_exit();
}

void yield_data1_func2()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 2, "Should be 2");
    *dp += 1;
    s.fiber_exit();
}


void yield_data2_func1();
void yield_data2_func2();
void yield_data2_func3();

// test yielding order works correctly
DEFINE_TEST_G(Yield_data2, yield_data2)
{
    char c = 'a';
    char *cp = &c;

    fiber f1((void*)yield_data2_func1, cp);
    fiber f2((void*)yield_data2_func2, cp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();
}

void yield_data2_func1()
{
    char *cp = (char*)s.get_data();
    TEST_MESSAGE(*cp == 'a', "Should be 'a'");
    *cp = 'b';

    fiber f((void*)yield_data2_func3, cp);
    s.spawn(&f);

    s.yield();

    TEST_MESSAGE(*cp == 'd', "Should be 'd'");
    *cp = 'e';
    s.fiber_exit();
}

void yield_data2_func2()
{
    char *cp = (char*)s.get_data();
    TEST_MESSAGE(*cp == 'b', "Should be 'b'");
    *cp = 'c';
    s.fiber_exit();
}

void yield_data2_func3()
{
    char *cp = (char*)s.get_data();
    TEST_MESSAGE(*cp == 'c', "Should be 'c'");
    *cp = 'd';
    s.fiber_exit();
}



void yield_data3_func1();
void yield_data3_func2();
void yield_data3_func3();
void yield_data3_func4();

// another test for yielding and data sharing
DEFINE_TEST_G(Yield_data3, yield_data3)
{
    int d = 1;
    int *dp = &d;

    fiber f1((void*)yield_data3_func1, dp);
    fiber f2((void*)yield_data3_func2, dp);
    fiber f3((void*)yield_data3_func3, dp);
    fiber f4((void*)yield_data3_func4, dp);

    s.spawn(&f1);
    s.spawn(&f2);
    s.spawn(&f3);

    s.do_it();

    s.spawn(&f4);
    s.spawn(&f3);

    s.do_it();

    TEST_MESSAGE(d == 36, "d should be 36");
}

void yield_data3_func1()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 1, "d should be 1");
    *dp += 1;

    s.yield();

    TEST_MESSAGE(*dp == 6, "d should be 6");
    s.fiber_exit();
}

void yield_data3_func2()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 2, "d should be 2");
    *dp += 1;
    s.fiber_exit();
}

void yield_data3_func3()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 3, "d should be 3");
    *dp *= 2;
    s.fiber_exit();
}

void yield_data3_func4()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 6, "d should be 6");
    *dp /= 2;

    s.yield();

    TEST_MESSAGE(*dp == 6, "d should be 6");
    *dp *= 6;
    s.fiber_exit();
}




void yield_data4_func1();
void yield_data4_func2();
void yield_data4_func3();

// test sharing different data between fibers
DEFINE_TEST_G(Yield_data4, yield_data4)
{
    int x = 1;
    int *xp = &x;

    int y = 5;
    int *yp = &y;

    fiber f1((void*)yield_data4_func1, xp);
    fiber f2((void*)yield_data4_func2, yp);
    fiber f3((void*)yield_data4_func3, xp);

    s.spawn(&f1);
    s.spawn(&f2);
    s.spawn(&f3);

    s.do_it();
}

void yield_data4_func1()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 1, "data should be 1");
    *dp += 4;

    s.yield();

    TEST_MESSAGE(*dp == 15, "data should be 15");

    s.fiber_exit();
}

void yield_data4_func2()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 5, "data should be 5");
    *dp /= 5;

    fiber f((void*)yield_data4_func1, dp);
    s.spawn(&f);

    s.yield();

    *dp = 15;

    s.fiber_exit();
}

void yield_data4_func3()
{
    int *dp = (int*)s.get_data();
    TEST_MESSAGE(*dp == 5, "data should be 5");
    *dp += 10;

    s.fiber_exit();
}




int main() 
{
    bool pass = true;

    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }

    return pass ? 0 : 1;
}