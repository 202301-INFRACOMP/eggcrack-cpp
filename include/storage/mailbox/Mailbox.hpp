#pragma once

#include <optional>

template <typename T> struct Mailbox
{
    virtual bool isFull() const = 0;

    virtual bool isEmpty() const = 0;

    virtual std::optional<T> peek() const = 0;

    virtual void send(T value) = 0;

    virtual T get() = 0;

    virtual ~Mailbox()
    {
    }
};
