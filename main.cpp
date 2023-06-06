#include <iostream>

#include "factory/ProductFactoryBuilder.hpp"
#include "factory/ProductFactory.hpp"

int main(int argc, char **argv)
{
    std::cout <<
        R"(========================================================================
                                                 _    
                                                | |   
             ___  __ _  __ _  ___ _ __ __ _  ___| | __
            / _ \/ _` |/ _` |/ __| '__/ _` |/ __| |/ /
           |  __/ (_| | (_| | (__| | | (_| | (__|   < 
            \___|\__, |\__, |\___|_|  \__,_|\___|_|\_\
                  __/ | __/ |                         
                 |___/ |___/                          
========================================================================

)";
    std::cout << "Welcome to case study No.1 of INFRACOMP202301\n";
    std::cout << "Code name: Project eggcrack\n";
    std::cout << "Implemented on C++ via STL\n\n";

    ProductFactoryBuilder pfb;

    auto pf = pfb.build();

    pf();

    std::cout << "\nWork finished!!!\n";
    std::cout << "Press any key to finish...";
    std::cin.get();

    return 0;
}
