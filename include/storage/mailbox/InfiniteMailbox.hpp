#pragma once

#include <optional>
#include <queue>

#include "storage/mailbox/Mailbox.hpp"

template <typename T> class InfiniteMailbox : public Mailbox<T>
{
  public:
    bool isFull() const override;

    bool isEmpty() const override;

    std::optional<T> peek() const override;

    void send(T value) override;

    T get() override;

  private:
    std::queue<T> buffer;
};

template <typename T> bool InfiniteMailbox<T>::isEmpty() const
{
    return buffer.empty();
}

template <typename T> inline std::optional<T> InfiniteMailbox<T>::peek() const
{
    if (isEmpty())
    {
        return {};
    }
    else
    {
        return buffer.front();
    }
}

template <typename T> void InfiniteMailbox<T>::send(T value)
{
    buffer.push(value);
}

template <typename T> T InfiniteMailbox<T>::get()
{
    auto value = buffer.front();
    buffer.pop();

    return value;
}

template <typename T> bool InfiniteMailbox<T>::isFull() const
{
    return false;
}
