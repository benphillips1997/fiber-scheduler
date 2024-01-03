#pragma once

#include <iostream>
#include "context/context.hpp"

class fiber
{
private:
    Context context_;
    char *stack_bottom_;
    char *stack_top_;
    void *data_;
public:
    fiber(void *func, void *d = nullptr);
    ~fiber() {}

    Context get_context();
    void change_context(Context c);

    void* get_data();
};