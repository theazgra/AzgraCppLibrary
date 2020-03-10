#include <azgra/azgra.h>
#include <iostream>
int main(int argc, char **argv)
{

#if (__cplusplus >= 201703L)
    std::cout << "C++17 !!" << '\n';
#else
    std::cout << "NOT C++17 !!" << '\n';
#endif
    return 0;
}
