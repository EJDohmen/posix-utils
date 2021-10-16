#include <mutex>
#include <condition_variable>
#include <atomic>


namespace posix_utils
{

    class BoundedSemaphore 
    {

        private:
            std::mutex _mutex;
            std::condition_variable _condition;
            size_t _max_resources;
            size_t _num_resources;

        public:

            BoundedSemaphore(size_t max_resources, size_t num_resources=0)
            : _max_resources(max_resources), _num_resources(num_resources) {}

            void unlock_one() 
            {
                std::lock_guard<decltype(_mutex)> lock(_mutex);
                ++_num_resources;
                _condition.notify_one();
            }

            void lock() 
            {
                std::unique_lock<decltype(_mutex)> lock(_mutex);
                while(_num_resources == 0) // Handle spurious wake-ups.
                {
                    _condition.wait(lock);
                }
                --_num_resources;
            }

            bool try_acquire() 
            {
                std::lock_guard<decltype(_mutex)> lock(_mutex);
                if(_num_resources > 0) {
                    --_num_resources;
                    return true;
                }
                return false;
            }

            size_t getNumResources()
            {
                std::lock_guard<decltype(_mutex)> lock(_mutex);
                return _num_resources;
            }

            bool isFull()
            {
                std::lock_guard<decltype(_mutex)> lock(_mutex);
                return _num_resources == _max_resources;
            }

            bool isEmpty()
            {
                std::lock_guard<decltype(_mutex)> lock(_mutex);
                return _num_resources == 0;
            }

    };

}