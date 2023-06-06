#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>

#include "factory/IdSequencer.hpp"
#include "factory/Product.hpp"
#include "storage/mailbox/Mailbox.hpp"

class OrangeWorker
{
  public:
    OrangeWorker(std::string id, std::size_t productCount, bool printIntermediate,
                 std::shared_ptr<IdSequencer> sequencer, std::shared_ptr<Mailbox<Product>> sendTo,
                 std::shared_ptr<std::mutex> sendToMutex, std::shared_ptr<std::condition_variable> sendToSignal);
    OrangeWorker(std::string id, std::size_t productCount, bool printIntermediate,
                 std::shared_ptr<Mailbox<Product>> receiveTo, std::shared_ptr<std::mutex> receiveToMutex,
                 std::shared_ptr<std::condition_variable> receiveToSignal, std::shared_ptr<Mailbox<Product>> sendTo,
                 std::shared_ptr<std::mutex> sendToMutex, std::shared_ptr<std::condition_variable> sendToSignal);

    void operator()();

  private:
    Product generateProduct();

    Product get();

    void send(const Product &p);

    std::string id;

    std::size_t productCount;

    bool printIntermediate;

    std::shared_ptr<IdSequencer> sequencer;

    std::shared_ptr<Mailbox<Product>> receiveTo;

    std::shared_ptr<std::mutex> receiveToMutex;

    std::shared_ptr<std::condition_variable> receiveToSignal;

    std::shared_ptr<Mailbox<Product>> sendTo;

    std::shared_ptr<std::mutex> sendToMutex;

    std::shared_ptr<std::condition_variable> sendToSignal;
};

OrangeWorker::OrangeWorker(std::string id, std::size_t productCount, bool printIntermediate,
                           std::shared_ptr<IdSequencer> sequencer, std::shared_ptr<Mailbox<Product>> sendTo,
                           std::shared_ptr<std::mutex> sendToMutex,
                           std::shared_ptr<std::condition_variable> sendToSignal)
    : id{id}, productCount{productCount}, printIntermediate{printIntermediate}, sequencer{sequencer}, sendTo{sendTo},
      sendToMutex{sendToMutex}, sendToSignal{sendToSignal}
{
}

OrangeWorker::OrangeWorker(std::string id, std::size_t productCount, bool printIntermediate,
                           std::shared_ptr<Mailbox<Product>> receiveTo, std::shared_ptr<std::mutex> receiveToMutex,
                           std::shared_ptr<std::condition_variable> receiveToSignal,
                           std::shared_ptr<Mailbox<Product>> sendTo, std::shared_ptr<std::mutex> sendToMutex,
                           std::shared_ptr<std::condition_variable> sendToSignal)
    : id{id}, productCount{productCount}, printIntermediate{printIntermediate}, receiveTo{receiveTo},
      receiveToMutex{receiveToMutex}, receiveToSignal{receiveToSignal}, sendTo{sendTo}, sendToMutex{sendToMutex},
      sendToSignal{sendToSignal}
{
}

void OrangeWorker::operator()()
{
    std::osyncstream os(std::cout);

    if (printIntermediate)
    {
        os << id << " has started.\n";
        os.emit();
    }
    for (int i = 0; i < productCount; i++)
    {
        Product p;
        if (sequencer)
        {
            p = generateProduct();
            if (printIntermediate)
            {
                os << id << " has generated:\n" << p;
                os.emit();
            }
        }
        else
        {
            p = get();
            p.message += id;

            if (printIntermediate)
            {
                os << id << " has modified:\n" << p;
                os.emit();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        send(p);
        if (printIntermediate)
        {
            os << id << " has sent:\n" << p;
            os.emit();
        }
    }
    if (printIntermediate)
    {
        os << id << " has finished.\n";
        os.emit();
    }
}

Product OrangeWorker::generateProduct()
{
    auto id = sequencer->get();
    return Product{id, "📙P" + std::to_string(id)};
}

Product OrangeWorker::get()
{
    while (true)
    {
        {
            std::scoped_lock lock{*receiveToMutex};
            if (!receiveTo->isEmpty() && receiveTo->peek().value().message.starts_with("📙"))
            {
                auto p = receiveTo->get();

                receiveToSignal->notify_all();

                return p;
            }
        }
        std::this_thread::yield();
    }
}

void OrangeWorker::send(const Product &p)
{
    while (true)
    {
        {
            std::scoped_lock lock{*sendToMutex};
            if (!sendTo->isFull())
            {
                sendTo->send(p);

                sendToSignal->notify_one();

                break;
            }
        }
        std::this_thread::yield();
    }
}
