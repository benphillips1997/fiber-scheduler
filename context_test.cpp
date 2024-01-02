#include <iostream>
#include "context/context.hpp"

using namespace std;


int main() 
{
    volatile int x = 0;

    Context context;
    get_context(&context); // save the state to return to later
    cout << "This is a message!" << endl;

    if (x == 0) {
        x++;
        set_context(&context);  // return to the state that was saved
    }

    return 0;
}