#include <iostream>
#include <thread>

struct Bulk;
extern Bulk bulk;
size_t connect(size_t N);
void receive(const char *buff, size_t buff_size, const size_t &id);
void disconnect(const size_t &id);

void testMultipleClientThreads() {
    const std::size_t numberOfCommandsPerThread = 1;//10;

    auto worker = [numberOfCommandsPerThread](std::size_t threadNum, std::size_t blockSize) {
        auto handle = connect(blockSize);
        for (std::size_t i = 0; i < numberOfCommandsPerThread; ++i) {
            std::string command{"cmd"};
            command += std::to_string(i + 1) + "_" + std::to_string(threadNum);
            receive(command.data(), command.size(), handle);
        }
        disconnect(handle);
    };

    std::thread t1(worker, 0, 1);
    std::thread t2(worker, 1, 1);

    t1.join();
    t2.join();
}


int main() {

    std::cout << "\n===\n=== Send commands from multiple threads ===\n===\n" << std::endl;
    testMultipleClientThreads();

    return 0;
}
