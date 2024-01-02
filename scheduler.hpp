#pragma once

#include <deque>
#include <iostream>
#include "fiber.hpp"
#include "context/context.hpp"

class scheduler
{
private:
    std::deque<fiber*> fibers_;
    Context context_;
    void *data_;
    fiber *curFiber;
public:
    scheduler() {}
    ~scheduler() {}

    void spawn(fiber *f, void *d = nullptr);
    void do_it();
    void yield();
    void fiber_exit();
    void* get_data(); 
};