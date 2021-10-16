#ifndef EJD_SHARED_BUFFER_HPP
#define EJD_SHARED_BUFFER_HPP

#include <cstddef>
#include <tuple>

namespace posix_utils
{
    class Shared_Buffer
    {
        public:
        explicit Shared_Buffer(size_t total_sz_bytes);
        ~Shared_Buffer();

        std::tuple<void *, size_t> getWritePointer();
        void releaseReadPointer(size_t num_bytes_written);

        std::tuple<void *, size_t> getReadPointer();
        void releaseReadPointer();


        private:

        struct Block_Info
        {
            void * buf_ptr;
            size_t bytes_in_block;
        };

    };

}


#endif 