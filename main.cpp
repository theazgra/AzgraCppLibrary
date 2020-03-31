#include <iostream>
#include <azgra/matrix.h>

int main(int argc, char **argv)
{

#if (__cplusplus >= 201703L)
    std::cout << "C++17 !!" << '\n';
#else
    std::cout << "NOT C++17 !!" << '\n';
#endif

    azgra::Matrix<int> mat(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});


    return 0;
}
