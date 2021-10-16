#ifndef EJD_SHARED_RING_BUFFER_HPP
#define EJD_SHARED_RING_BUFFER_HPP

#include <cstddef>
#include <memory>
#include <atomic>

#include "BoundedSemaphore.hpp"

namespace posix_utils
{
    class Shared_Ring_Buffer
    {
        public:
        static std::unique_ptr<Shared_Ring_Buffer> create(size_t block_sz_bytes, size_t num_blocks);
        ~Shared_Ring_Buffer();

        char * getWritePointer();
        char * getWritePointerNoBlock();
        void releaseWritePointer();
        

        char * getReadPointer();
        char * getReadPointerNoBlock();
        void releaseReadPointer();

        size_t getBlockSzBytes();
        size_t getNumBlocks();


        private:
        
        explicit Shared_Ring_Buffer(size_t block_sz_bytes, size_t num_blocks);

        size_t _block_sz_bytes;
        size_t _num_blocks;
        BoundedSemaphore _writer_sem;
        BoundedSemaphore _reader_sem;
        size_t _writer_index = 0;
        size_t _reader_index = 0;
        bool _have_writer_pointer = false;
        bool _have_reader_pointer = false;
        char * _buffer_ptr;

    };

}


#endif 