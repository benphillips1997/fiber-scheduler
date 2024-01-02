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

    // set context pointers
    context_.rsp = sp;
    context_.rip = func;
}

fiber::~fiber() {}

Context fiber::get_context()
{
    return context_;
}

void* fiber::get_data()
{
    return data_;
}