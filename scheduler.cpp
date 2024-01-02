#include "scheduler.hpp"

void scheduler::spawn(fiber *f, void *d)
{
    data_ = d;
    fibers_.push_back(f);
}

void scheduler::do_it()
{
    // return here to re-enter scheduler
    get_context(&context_);

    if (!fibers_.empty()) {
        // std::cout << "fiber running" << std::endl;
        fiber *f = fibers_.front();
        // fibers.pop_front();

        // set data to data from current fiber
        data_ = f->get_data();

        // jump to task
        Context c = f->get_context();
        set_context(&c);
    }

    // if (!rescheduled.empty()) {
    //     std::cout << "Running rescheduled" << std::endl;
    //     Context c = *(rescheduled.front());
    //     rescheduled.pop_front();

    //     set_context(&c);
    // }
}

void scheduler::yield()
{
    std::cout << "Yield called" << std::endl;
    // reschedule the fiber
    std::cout << "1" << std::endl;
    Context c = fibers_.front()->get_context();
    set_context(&c);
    fiber f = *(fibers_.front());

    std::cout << "2" << std::endl;
    fibers_.pop_front();
    std::cout << "3" << std::endl;
    fibers_.push_back(&f);
    std::cout << "4" << std::endl;

    set_context(&context_);
}

void scheduler::fiber_exit() 
{
    data_ = nullptr;
    // curFiber = nullptr;

    fibers_.pop_front();

    // jump back to scheduler
    set_context(&context_);
}

void* scheduler::get_data()
{
    return data_;
}