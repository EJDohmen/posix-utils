#include "gtest/gtest.h"

#include <thread>
#include <time.h>      
#include <chrono>

#include "Shared_Ring_Buffer.hpp"

using namespace posix_utils;


TEST(Shared_Ring_Buffer, multiple_write_to_same_buffer) 
{
    std::unique_ptr<Shared_Ring_Buffer> srb_ptr = Shared_Ring_Buffer::create(2, 1);

    for(unsigned i = 0; i < 100; ++i)
        strcpy(srb_ptr->getWritePointer(), "A");

    strcpy(srb_ptr->getWritePointer(), "B");
    srb_ptr->releaseWritePointer();

    char* data = srb_ptr->getReadPointer();

    EXPECT_TRUE(data[0] == 'B');
}

TEST(Shared_Ring_Buffer, multiple_read_from_same_buffer) 
{
    std::unique_ptr<Shared_Ring_Buffer> srb_ptr = Shared_Ring_Buffer::create(2, 2);

    //Fill shared buffer
    strcpy(srb_ptr->getWritePointer(), "A");
    srb_ptr->releaseWritePointer();
    strcpy(srb_ptr->getWritePointer(), "B");
    srb_ptr->releaseWritePointer();

    //Read from same buffer multiple times
    char* data = srb_ptr->getReadPointer();
    EXPECT_TRUE(data[0] == 'A');
    data = srb_ptr->getReadPointer();
    EXPECT_TRUE(data[0] == 'A');
    srb_ptr->releaseReadPointer();

    EXPECT_TRUE(srb_ptr->getReadPointer()[0] == 'B');
}


TEST(Shared_Ring_Buffer, no_block_writer) 
{
    std::unique_ptr<Shared_Ring_Buffer> srb_ptr = Shared_Ring_Buffer::create(2, 2);

    //Fill shared buffer
    strcpy(srb_ptr->getWritePointerNoBlock(), "A");
    srb_ptr->releaseWritePointer();
    strcpy(srb_ptr->getWritePointerNoBlock(), "B");
    srb_ptr->releaseWritePointer();

    //Because the buffer is full, we expect nullptr to be returned
    EXPECT_TRUE( srb_ptr->getWritePointerNoBlock() == nullptr );
}

TEST(Shared_Ring_Buffer, no_block_reader) 
{
    std::unique_ptr<Shared_Ring_Buffer> srb_ptr = Shared_Ring_Buffer::create(2, 2);
    //Because the buffer is empty, we expect nullptr to be returned
    EXPECT_TRUE( srb_ptr->getReadPointerNoBlock() == nullptr );
}


TEST(Shared_Ring_Buffer, misuse_release_W_R_Pointer) 
{
    std::unique_ptr<Shared_Ring_Buffer> srb_ptr = Shared_Ring_Buffer::create(2, 2);

    //Fill shared buffer, and misuse releaseWritePointer()
    strcpy(srb_ptr->getWritePointerNoBlock(), "A");
    EXPECT_TRUE(srb_ptr->releaseWritePointer());
    EXPECT_FALSE(srb_ptr->releaseWritePointer());
    EXPECT_FALSE(srb_ptr->releaseWritePointer());
    EXPECT_FALSE(srb_ptr->releaseWritePointer());
    strcpy(srb_ptr->getWritePointerNoBlock(), "B");
    EXPECT_TRUE(srb_ptr->releaseWritePointer());
    EXPECT_FALSE(srb_ptr->releaseWritePointer());
    EXPECT_FALSE(srb_ptr->releaseWritePointer());
    EXPECT_FALSE(srb_ptr->releaseWritePointer());

    //We are only suppose to call releaseWritePointer once
    //per call to getWritePointerNoBlock(), however we don't
    //want to break if someone misuses it, so it does not release 
    //on successive calls, and will return false

    EXPECT_STREQ(srb_ptr->getReadPointer(), "A");
    EXPECT_TRUE(srb_ptr->releaseReadPointer());
    EXPECT_FALSE(srb_ptr->releaseReadPointer());
    EXPECT_FALSE(srb_ptr->releaseReadPointer());
    EXPECT_STREQ(srb_ptr->getReadPointer(), "B");
    EXPECT_TRUE(srb_ptr->releaseReadPointer());
    EXPECT_FALSE(srb_ptr->releaseReadPointer());
    EXPECT_FALSE(srb_ptr->releaseReadPointer());
    //Same check as above, but for the reader side

}

TEST(Shared_Ring_Buffer, multithredded1)    
{
    std::shared_ptr<Shared_Ring_Buffer> srb_ptr = Shared_Ring_Buffer::create(1024, 100);

    //Writer Thread
    std::thread wt([srb_ptr](){
        for(unsigned i = 1; i <= 10000; ++i)
        {
            char* block = static_cast<char*> ( srb_ptr->getWritePointer());
            strcpy(block, std::to_string(i).c_str());
            srb_ptr->releaseWritePointer();
        }
    });

    //Reader Thread
    std::thread rt([srb_ptr](){
        int last = 0;
        for(unsigned i = 0; i < 10000; ++i)
        {
            char* block = static_cast<char*> ( srb_ptr->getReadPointer());
            if (std::stoi(block) != last + 1)
                std::cout << "Out of order" << "\n";
            last = std::stoi(block);
            srb_ptr->releaseReadPointer();
        }
    });

    wt.join();
    rt.join();
}


TEST(Shared_Ring_Buffer, multithredded_with_sleeps)    
{
    // This version of the multithredded test will insert some random
    // sleeps to simulate the reader and writers not being ready
    std::shared_ptr<Shared_Ring_Buffer> srb_ptr = Shared_Ring_Buffer::create(1024, 100);

    srand (time(NULL));

    //Writer Thread
    std::thread wt([srb_ptr](){
        for(unsigned i = 1; i <= 10000; ++i)
        {
            char* block = static_cast<char*> ( srb_ptr->getWritePointer());
            strcpy(block, std::to_string(i).c_str());
            srb_ptr->releaseWritePointer();
            if( rand() % 100 == 0) std::this_thread::sleep_for( std::chrono::milliseconds(1) );
            
        }
    });

    //Reader Thread
    std::thread rt([srb_ptr](){
        int last = 0;
        for(unsigned i = 0; i < 10000; ++i)
        {
            char* block = static_cast<char*> ( srb_ptr->getReadPointer());
            if (std::stoi(block) != last + 1)
                std::cout << "Out of order" << "\n";
            last = std::stoi(block);
            srb_ptr->releaseReadPointer();
            if( rand() % 100 == 0) std::this_thread::sleep_for( std::chrono::milliseconds(1) );
        }
    });

    wt.join();
    rt.join();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}