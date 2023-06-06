#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

#include "factory/Product.hpp"
#include "storage/mailbox/Mailbox.hpp"

class RedWorker
{
  public:
    RedWorker(const std::string &id, const std::size_t productCount, std::shared_ptr<Mailbox<Product>> receiveTo,
              std::shared_ptr<std::mutex> receiveToMutex, std::shared_ptr<std::condition_variable> receiveToSignal);

    void operator()();

  private:
    Product getProduct();

    std::string id;

    const std::size_t productCount;

    std::shared_ptr<Mailbox<Product>> receiveTo;

    std::shared_ptr<std::mutex> receiveToMutex;

    std::shared_ptr<std::condition_variable> receiveToSignal;

    std::size_t currentId = 0;

    std::priority_queue<Product, std::vector<Product>, std::greater<Product>> pq;
};

RedWorker::RedWorker(const std::string &id, const std::size_t productCount, std::shared_ptr<Mailbox<Product>> receiveTo,
                     std::shared_ptr<std::mutex> receiveToMutex,
                     std::shared_ptr<std::condition_variable> receiveToSignal)
    : id{id}, productCount{productCount}, receiveTo{receiveTo}, receiveToMutex{receiveToMutex}, receiveToSignal{
                                                                                                    receiveToSignal}
{
}

void RedWorker::operator()()
{
    std::osyncstream os(std::cout);
    os << id << " has started.\n";
    os.emit();
    for (std::size_t i = 0; i < productCount; i++)
    {
        auto lastProduct = getProduct();

        pq.push(lastProduct);
        auto p = pq.top();

        if (p.id == currentId)
        {
            os << "📕 has printed:\n" << p;
            os.emit();
            pq.pop();

            currentId++;
        }
    }

    for (std::size_t i = currentId; i < productCount; i++)
    {
        auto p = pq.top();
        pq.pop();
        os << "📕 has printed:\n" << p;
        os.emit();
    }

    os << id << " has finished.\n";
    os.emit();
}

Product RedWorker::getProduct()
{
    while (true)
    {
        std::scoped_lock lock{*receiveToMutex};
        if (!receiveTo->isEmpty())
        {
            auto p = receiveTo->get();
            receiveToSignal->notify_all();
            return p;

        }
    }
}
