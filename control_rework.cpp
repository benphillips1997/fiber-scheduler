#include <iostream>
#include "context/context.hpp"
#include "fiber.hpp"

using namespace std;


void foo();
void goo();


void foo()
{
    cout << "Called foo" << endl;

    fiber f((void*)goo);
    Context c = f.get_context();
    set_context(&c);
}

void goo()
{
    cout << "Called goo" << endl;

    exit(1);
}

int main() 
{
    fiber f((void*)foo);
    Context c = f.get_context();
    set_context(&c);

    return 0;
}