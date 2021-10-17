/**
 * @file BoundedSemaphore.cpp
 *
 * @brief See header file for more information
 *
 * @author EJ Dohmen
 *
 */

#include "BoundedSemaphore.hpp"

using namespace posix_utils;

BoundedSemaphore::BoundedSemaphore(size_t max_resources, size_t starting_resources) noexcept
: _max_resources(max_resources), _num_resources(starting_resources) 
{
}


bool BoundedSemaphore::unlock_one() 
{
    std::lock_guard<decltype(_mutex)> lock(_mutex);
    if (_num_resources == _max_resources) //We are full
        return false;
    ++_num_resources;
    _condition.notify_one();
    return true;
}

void BoundedSemaphore::lock() 
{
    std::unique_lock<decltype(_mutex)> lock(_mutex);
    while(_num_resources == 0) // Handle spurious wake-ups.
    {
        _condition.wait(lock);
    }
    --_num_resources;
}

bool BoundedSemaphore::try_acquire() 
{
    std::lock_guard<decltype(_mutex)> lock(_mutex);
    if(_num_resources > 0) {
        --_num_resources;
        return true;
    }
    return false;
}

size_t BoundedSemaphore::getNumResources()
{
    std::lock_guard<decltype(_mutex)> lock(_mutex);
    return _num_resources;
}

bool BoundedSemaphore::isFull()
{
    std::lock_guard<decltype(_mutex)> lock(_mutex);
    return _num_resources == _max_resources;
}

bool BoundedSemaphore::isEmpty()
{
    std::lock_guard<decltype(_mutex)> lock(_mutex);
    return _num_resources == 0;
}