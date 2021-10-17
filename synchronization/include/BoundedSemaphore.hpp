/**
 * @file BoundedSemaphore.hpp
 *
 * @brief This bounded semaphore implementation is used to synchronize
 *        the production and consumption of a resource. By "bounded" 
 *        I mean that the resource also has an upper limit. 
 *        i.e. The resource should not have more than some configurable 
 *        maximum resources "produced" at any given time. 
 * 
 * Inspiration and starting code from:
 * https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
 *
 * @author EJ Dohmen
 *
 */
#ifndef EJD_BOUNDED_SEMAPHORE_HPP
#define EJD_BOUNDED_SEMAPHORE_HPP
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace posix_utils
{

    class BoundedSemaphore 
    {

        public:

            /** 
            * Constructor for the BoundedSemaphore class
            *
            * @param max_resources The maximum number resources we can "unlock" at the same time
            * @param starting_resources The starting number of resources
            * 
            * @return A unique_ptr to the newly constructed object, or nullptr if an error occurred.
            */
            explicit BoundedSemaphore(size_t max_resources, size_t starting_resources=0) noexcept; 

            /** 
            * Unlocks a resource, signaling that resource is ready for consumption.
            * 
            * @return True if a resource was unlocked, false if we already have the maximum
            *          number of resources unlocked
            */
            bool unlock_one();

            /** 
            * Locks a resource, signaling that resource is being used. 
            * This method will block until a resource is available to be locked 
            * 
            */
            void lock();

            /** 
            * Attempts to lock a resource, signaling that resource is ready for consumption.
            * This version will NOT block, and has only locked the resource if the method
            * returns true.
            * 
            * @return True if a resource was locked, false if there were no resource is available.    
            */
            bool try_acquire();

            /** 
            * 
            * @return The number of free resources being guarded by the object
            */
            size_t getNumResources();

            /** 
            * @return True if all available resources have been locked, False otherwise
            */
            bool isFull();

            /** 
            * @return True if none of the available resources have been locked, False otherwise
            */
            bool isEmpty();

        private:
            std::mutex _mutex;
            std::condition_variable _condition;
            size_t _max_resources;
            size_t _num_resources;


    };

}
#endif