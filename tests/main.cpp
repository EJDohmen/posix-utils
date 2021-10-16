#include "gtest/gtest.h"

#include <thread>

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

    strcpy(srb_ptr->getWritePointer(), "A");
    srb_ptr->releaseWritePointer();
    strcpy(srb_ptr->getWritePointer(), "B");
    srb_ptr->releaseWritePointer();

    char* data = srb_ptr->getReadPointer();
    EXPECT_TRUE(data[0] == 'A');
    data = srb_ptr->getReadPointer();
    EXPECT_TRUE(data[0] == 'A');
    srb_ptr->releaseReadPointer();

    EXPECT_TRUE(srb_ptr->getReadPointer()[0] == 'B');
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}