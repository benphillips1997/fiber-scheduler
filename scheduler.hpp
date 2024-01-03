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
    fiber *cur_fiber_;
public:
    scheduler() {}
    ~scheduler() {}

    void spawn(fiber *f);
    void do_it();
    void yield();
    void fiber_exit();
    void* get_data(); 
};