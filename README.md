# posix-utils
This set of libraries is used to wrap POSIX functionality in an object oriented way. 

## synchronization
Library used to expose Locks, Mutexes and Semaphores with C++ Objects.

## shared-buffer
Various shared buffer implementations.
Planning on adding implementations that are optimized for specific use cases.

### Implemented Shared Buffers

| Class Name         | Predicted Use Case |
| ------------------ | -------------------------------------------------- |
| Shared_Ring_Buffer | Good in producer-consumer environments where producer's transformation targets an output buffer, and that data needs to be passed to the consumer cheaply. Buffer is then available for consumer to use, and could feed another transform. |

# Dependencies 
| Dependency Name         | Notes                    |URL              |
| ----------------------- | ------------------------ | ------------------------------------------------ |
| gTest                   | For running unit tests   | https://github.com/google/googletest |

# Cloning/Building 
`
git clone https://github.com/EJDohmen/posix-utils.git
cd posix-utils
mkdir build/
cd build/
cmake ..
make
` 

You can disable building the unit tests by changing the cmake command:
`
cmake -DBUILD_TESTS=NO ..
`
