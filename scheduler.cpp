#include "scheduler.hpp"

void scheduler::spawn(fiber *f)
{
    fibers_.push_back(f);
}

void scheduler::do_it()
{
    // return here to re-enter scheduler
    get_context(&context_);

    if (!fibers_.empty()) {
        fiber *f = fibers_.front();
        fibers_.pop_front();
        cur_fiber_ = f;

        // jump to task
        Context c = f->get_context();
        set_context(&c);
    }
}

void scheduler::yield()
{
    bool yielded = false;
    // create new context to return to later and another one to store the fibers current context to give back
    Context return_context, original_context;
    get_context(&return_context);

    // check yield status we don't create an infinite loop
    if (!yielded) {
        yielded = true;

        // keep current context to give back later
        original_context = cur_fiber_->get_context();
        // replace the context of the fiber with the new context so we can return later
        cur_fiber_->change_context(return_context);

        // push this fiber to the back of the queue
        fibers_.push_back(cur_fiber_);
        cur_fiber_ = nullptr;

        // jump back to scheduler
        set_context(&context_);
    }

    yielded = false;
    // change context back to original
    cur_fiber_->change_context(original_context);
}

void scheduler::fiber_exit() 
{
    cur_fiber_ = nullptr;

    // jump back to scheduler
    set_context(&context_);
}

void* scheduler::get_data()
{
    return cur_fiber_->get_data();
}