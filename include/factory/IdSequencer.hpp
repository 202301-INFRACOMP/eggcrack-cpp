#pragma once

#include <mutex>

class IdSequencer
{
  public:
    std::size_t get();

  private:
    std::size_t id = 0;
    std::mutex mtx;
};

std::size_t IdSequencer::get()
{
    std::scoped_lock lock{mtx};

    return id++;
}
