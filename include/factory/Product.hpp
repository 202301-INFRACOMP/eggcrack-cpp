#pragma once

#include <ostream>
#include <string>

struct Product
{
    Product() = default;

    Product(const std::size_t id, const std::string &message);

    friend bool operator>(const Product &lhs, const Product &rhs);

    friend std::ostream &operator<<(std::ostream &os, Product &p);

    std::size_t id = 0;

    std::string message;
};

Product::Product(const std::size_t id, const std::string &message) : id{id}, message{message}
{
}

bool operator>(const Product &lhs, const Product &rhs)
{
    return lhs.id > rhs.id;
}

std::ostream &operator<<(std::ostream &os, Product &p)
{
    os << "╔\n"
       << "║Product\n"
       << "║->id: " << p.id << '\n'
       << "║->message: " << p.message << '\n'
       << "╚\n";

    return os;
}
