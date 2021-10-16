#include "Shared_Ring_Buffer.hpp"

using namespace posix_utils;



std::unique_ptr<Shared_Ring_Buffer> Shared_Ring_Buffer::create(size_t block_sz_bytes, size_t num_blocks)
{
    // Using `new` to access a non-public constructor.
    std::unique_ptr<Shared_Ring_Buffer> me_ptr( new Shared_Ring_Buffer(block_sz_bytes, num_blocks) );
    //std::unique_ptr<Shared_Ring_Buffer> me_ptr = std::make_unique<Shared_Ring_Buffer>(block_sz_bytes, num_blocks);



    return me_ptr;
}

Shared_Ring_Buffer::Shared_Ring_Buffer(size_t block_sz_bytes, size_t num_blocks)
: _block_sz_bytes(block_sz_bytes), 
  _num_blocks(num_blocks), 
  _writer_sem{num_blocks, num_blocks}, 
  _reader_sem{num_blocks, 0}
{
    _buffer_ptr = new char[_block_sz_bytes * _num_blocks];
}

Shared_Ring_Buffer::~Shared_Ring_Buffer()
{
    delete [] static_cast<char*>(_buffer_ptr);
}

char * Shared_Ring_Buffer::getWritePointer()
{
    char * result = _buffer_ptr + _writer_index * _block_sz_bytes;
    if(_have_writer_pointer == false)
    {
        _writer_sem.lock();
        _have_writer_pointer = true;
    }
    return result;
}

char * Shared_Ring_Buffer::getWritePointerNoBlock()
{
    if (_writer_sem.isEmpty()) return nullptr;
    return getWritePointer();
}

void Shared_Ring_Buffer::releaseWritePointer()
{
    _have_writer_pointer = false;
    _writer_index = (_writer_index + 1) % _num_blocks;
    _reader_sem.unlock_one();
}

char * Shared_Ring_Buffer::getReadPointer()
{
    char * result = _buffer_ptr + _reader_index * _block_sz_bytes;
    if(_have_reader_pointer == false)
    {
        _reader_sem.lock();
        _have_reader_pointer = true;
    }
    return result;
}

char * Shared_Ring_Buffer::getReadPointerNoBlock()
{
    if (_reader_sem.isFull() ) return nullptr;
    return getReadPointer();
}

void Shared_Ring_Buffer::releaseReadPointer()
{
     _have_reader_pointer = false;
    _reader_index = (_reader_index + 1) % _num_blocks;
    _writer_sem.unlock_one();
}

size_t Shared_Ring_Buffer::getBlockSzBytes()
{
    return _block_sz_bytes;
}

size_t Shared_Ring_Buffer::getNumBlocks()
{
    return _num_blocks;
}




