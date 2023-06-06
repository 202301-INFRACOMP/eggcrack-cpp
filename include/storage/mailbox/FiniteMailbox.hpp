#pragma once

#include <memory>
#include <optional>

#include "storage/mailbox/Mailbox.hpp"

template <typename T> class FiniteMailbox : public Mailbox<T>
{
  public:
    FiniteMailbox(const std::size_t size);

    bool isFull() const override;

    bool isEmpty() const override;

    std::optional<T> peek() const override;

    void send(T value) override;

    T get() override;

  private:
    void enqueue(T value);

    T dequeue();

    const std::unique_ptr<T[]> buffer;

    const std::size_t size;

    std::size_t front;

    std::size_t rear;
};

template <typename T>
FiniteMailbox<T>::FiniteMailbox(const std::size_t size)
    : buffer{new T[size]}, size{size}, front{std::numeric_limits<std::size_t>::max()},
      rear{std::numeric_limits<std::size_t>::max()}
{
}

template <typename T> bool FiniteMailbox<T>::isFull() const
{
    if (front == 0 && rear == size - 1)
    {
        return true;
    }
    if (front == rear + 1)
    {
        return true;
    }

    return false;
}

template <typename T> bool FiniteMailbox<T>::isEmpty() const
{
    if (front == std::numeric_limits<std::size_t>::max())
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T> std::optional<T> FiniteMailbox<T>::peek() const
{
    if (isEmpty())
    {
        return {};
    }
    else
    {
        return buffer[front];
    }
}

template <typename T> void FiniteMailbox<T>::send(T value)
{
    enqueue(value);
}

template <typename T> T FiniteMailbox<T>::get()
{
    return dequeue();
}

template <typename T> void FiniteMailbox<T>::enqueue(T value)
{
    if (front == std::numeric_limits<std::size_t>::max())
    {
        front = 0;
        rear = 0;
    }
    else
    {
        rear = (rear + 1) % size;
    }

    buffer[rear] = value;
}

template <typename T> T FiniteMailbox<T>::dequeue()
{
    T value = buffer[front];

    if (front == rear)
    {
        front = std::numeric_limits<std::size_t>::max();
        rear = std::numeric_limits<std::size_t>::max();
    }
    else
    {
        front = (front + 1) % size;
    }

    return value;
}
