#include "fiber.hpp"

fiber::fiber(void *func, void *d)
{
    // assign pointer of data passed
    data_ = d;

    // create stack
    stack_bottom_ = new char[4096];
    stack_top_ = stack_bottom_ + 4096;
    // align to 16 bytes
    char *sp = (char*)((uintptr_t)stack_top_ & -16L);
    // account for red zone
    sp -= 128;

    // set context stack pointer and instruction pointer
    context_.rsp = sp;
    context_.rip = func;
}

Context fiber::get_context()
{
    return context_;
}

void fiber::change_context(Context c)
{
    context_ = c;
}

void* fiber::get_data()
{
    return data_;
}