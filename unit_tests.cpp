#include <iostream>
#include "simpletest/simpletest.h"
#include "context/context.hpp"
#include "fiber.hpp"
#include "scheduler.hpp"

using namespace std;

char const *groups[] = {
    "test",
};

DEFINE_TEST_G(Test, test)
{
    int x = 0;
    TEST_MESSAGE(x == 0, "Should pass");
    TEST_MESSAGE(x != 0, "Should fail");
}

int main() 
{
    bool pass = true;

    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }

    return pass ? 0 : 1;
}