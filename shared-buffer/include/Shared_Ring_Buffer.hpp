/**
 * @file SharedBuffer.hpp
 *
 * @brief This is a shared ring buffer implementation that exposes 
 *        the internal buffer to the writer and reader in a controlled 
 *        manner that allows a single write to share data between the two.
 *        This is very useful when when a processor is doing some kind of 
 *        a transformation on data, and that transformed data is the input
 *        for the next processor (reader). It is also important to note that 
 *        the reader can then use the buffer as input to successive 
 *        calculations/transformations without needing to retrieve a copy. 
 *
 * @author EJ Dohmen
 *
 */

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

        /** Factory method for construcing instances of the class
        *
        * This method is used to create instances of the class, and can
        * support exception free error handling through the use of the 
        * return value.
        *
        * @param block_sz_bytes The block size (in bytes) of each block in the buffer
        * @param num_blocks The total number of blocks that should be allocated
        * 
        * @return A unique_ptr to the newly constructed object, or nullptr if an error occurred.
        */
        static std::unique_ptr<Shared_Ring_Buffer> create(size_t block_sz_bytes, size_t num_blocks);
        ~Shared_Ring_Buffer();

        /** 
        * This method is used by the writer to retrieve the next block for writing.
        * It is a blocking call and won't return until a block becomes available.
        *
        * @return A pointer to a free buffer block for the writer.
        */
        char * getWritePointer();

        /** 
        * This method is used by the writer to retrieve the next block for writing.
        * It will not block if the shared buffer is full.
        *
        * @return A pointer to a free buffer block for the writer, or nullptr 
        *         if the buffer is full.
        */
        char * getWritePointerNoBlock();

        /** 
        * This method is used by the writer to signal that it is done writing
        * to the block returned by getWritePointer...()
        *
        * @return True if the release was signaled, false if there was an issue.
        *         Note: The only time this can return false is if this method 
        *               is (incorrectly) called multiple times, without requesting 
        *               a new block with getWritePointer...()
        */
        bool releaseWritePointer();
        
        /** 
        * This method is used by the reader to retrieve the next block for reading.
        * It is a blocking call and won't return until a block becomes available.
        *
        * @return A pointer to a block filled by the writer.
        */
        char * getReadPointer();

        /** 
        * This method is used by the reader to retrieve the next block for reading.
        * It will not block if the shared buffer is empty.
        *
        * @return A pointer to a block filled by the writer, or nullptr 
        *         if the buffer is empty.
        */
        char * getReadPointerNoBlock();

        /** 
        * This method is used by the reader to signal that it is done reading
        * from the block returned by getReadPointer...()
        *
        * @return True if the release was signaled, false if there was an issue.
        *         Note: The only time this can return false is if this method 
        *               is (incorrectly) called multiple times, without requesting 
        *               a new block with getReadPointer...()
        */
        bool releaseReadPointer();

        /** 
        * @return The block size of the shared ring buffer.
        */
        size_t getBlockSzBytes();

        /** 
        * @return The maximum number of blocks the buffer can hold.
        */
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