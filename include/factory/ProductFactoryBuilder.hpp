#pragma once

#include <iostream>

#include "factory/ProductFactory.hpp"

struct ProductFactoryBuilder
{
    ProductFactory build();
};

ProductFactory ProductFactoryBuilder::build()
{
    std::size_t productCount;
    std::cout << "Enter the product count: ";
    std::cin >> productCount;

    std::size_t threadCount;
    std::cout << "Enter the number of threads per stage: ";
    std::cin >> threadCount;

    std::size_t bufferSize;
    std::cout << "Enter the size of the buffer: ";
    std::cin >> bufferSize;

    std::string opPrint;
    std::cout << "Do you wanna print the intermediate messages (y/n): ";
    std::cin >> opPrint;
    bool printIntermediate = opPrint == "y";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return ProductFactory{productCount, threadCount, bufferSize, printIntermediate};
}
