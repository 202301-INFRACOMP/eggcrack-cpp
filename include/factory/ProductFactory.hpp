#pragma once

#include <functional>
#include <thread>
#include <vector>

#include "factory/Product.hpp"
#include "factory/worker/BlueWorker.hpp"
#include "factory/worker/OrangeWorker.hpp"
#include "factory/worker/RedWorker.hpp"
#include "storage/mailbox/FiniteMailbox.hpp"
#include "storage/mailbox/InfiniteMailbox.hpp"

class ProductFactory
{
  public:
    ProductFactory(std::size_t productCount, std::size_t stageGroupSize, std::size_t bufferSize, bool printIntermediate)
    {
        auto lastMailbox = std::make_shared<InfiniteMailbox<Product>>();
        auto lastMutex = std::make_shared<std::mutex>();
        auto lastSignal = std::make_shared<std::condition_variable>();

        // Producer layer
        auto sequencer = std::make_shared<IdSequencer>();

        auto producerMailbox = std::make_shared<FiniteMailbox<Product>>(bufferSize);
        auto producerMutex = std::make_shared<std::mutex>();
        auto producerSignal = std::make_shared<std::condition_variable>();
        for (int i = 0; i < stageGroupSize - 1; i++)
        {
            workers.emplace_back(BlueWorker{
                "📘L0N" + std::to_string(i),
                productCount,
                printIntermediate,
                sequencer,
                producerMailbox,
                producerMutex,
                producerSignal,
            });
        }

        workers.emplace_back(OrangeWorker{"📙L0", productCount, printIntermediate, sequencer, producerMailbox,
                                          producerMutex, producerSignal});

        // Modifier layer
        auto intermediateMailbox = std::make_shared<FiniteMailbox<Product>>(bufferSize);
        auto intermediateMutex = std::make_shared<std::mutex>();
        auto intermediateSignal = std::make_shared<std::condition_variable>();
        for (int i = 0; i < stageGroupSize - 1; i++)
        {
            workers.emplace_back(BlueWorker{"📘L1N" + std::to_string(i), productCount, printIntermediate,
                                            producerMailbox, producerMutex, producerSignal, intermediateMailbox,
                                            intermediateMutex, intermediateSignal});
        }
        workers.emplace_back(OrangeWorker{"📙L1", productCount, printIntermediate, producerMailbox, producerMutex,
                                          producerSignal, intermediateMailbox, intermediateMutex, intermediateSignal});

        for (int i = 0; i < stageGroupSize - 1; i++)
        {
            workers.emplace_back(BlueWorker{"📘L2N" + std::to_string(i), productCount, printIntermediate,
                                            intermediateMailbox, intermediateMutex, intermediateSignal, lastMailbox,
                                            lastMutex, lastSignal});
        }
        workers.emplace_back(OrangeWorker{"📙L2", productCount, printIntermediate, intermediateMailbox,
                                          intermediateMutex, intermediateSignal, lastMailbox, lastMutex, lastSignal});

        // Print layer
        workers.emplace_back(RedWorker{"📕", productCount * stageGroupSize, lastMailbox, lastMutex, lastSignal});
    }

    void operator()();

  private:
    std::vector<std::function<void()>> workers;
};

void ProductFactory::operator()()
{
    std::vector<std::jthread> threads;

    for (const auto &w : workers)
    {
        threads.emplace_back(w);
    }
}
