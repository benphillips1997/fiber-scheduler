#include <iostream>
#include "context/context.hpp"

using namespace std;


void foo();
void goo();


void foo()
{
    cout << "Called foo" << endl;

    // allocate stack space
    char *data = new char[4096];
    char *sp = data + 4096;
    // align to 16 bytes
    sp = (char*)((uintptr_t)sp & -16L);
    // account for red zone
    sp -= 128;

    Context c;
    c.rsp = sp;
    c.rip = (void*)goo;

    Context s;

    volatile int i = 0;

    get_context(&s);

    if (i < 2) {
        cout << "foo run " << i+1 << endl;
        i++;
        set_context(&s);
    }
    
    swap_context(&s, &c);
}

void goo()
{
    cout << "Called goo" << endl;

    // allocate stack space
    char *data = new char[4096];
    char *sp = data + 4096;
    // align to 16 bytes
    sp = (char*)((uintptr_t)sp & -16L);
    // account for red zone
    sp -= 128;

    exit(1);
}

int main() 
{
    // allocate stack space
    char *data = new char[4096];
    char *sp = data + 4096;
    // align to 16 bytes
    sp = (char*)((uintptr_t)sp & -16L);
    // account for red zone
    sp -= 128;

    Context c;
    c.rsp = sp;

    // set context instruction pointer to foo and switch
    c.rip = (void*)foo;
    set_context(&c);

    return 0;
}