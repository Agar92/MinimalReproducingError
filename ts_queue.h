#include <condition_variable>
#include <queue>

template<typename T>
class ts_queue {
    std::queue<T> Buffer;
    bool Done = false;
    mutable std::mutex mut;
    std::condition_variable cv;

    bool done() const { return Done; }

public:
    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        Buffer.push(std::move(new_value));
        cv.notify_one();
    }

    bool wait_and_pop(T &value) {
        //std::cout<<"wait_and_pop 1"<<std::endl;
        std::unique_lock<std::mutex> lk(mut);
        cv.wait(lk, [this] { return !Buffer.empty() || done(); });
        if (Buffer.empty()) {
            //std::cout<<"wait_and_pop 21"<<std::endl;
            return false;
        } else {
            value = std::move(Buffer.front());
            Buffer.pop();
            //std::cout<<"wait_and_pop 22"<<std::endl;
            return true;
        }
    }

    void wake_and_done() {
        std::unique_lock<std::mutex> lk{mut};
        Done = true;
        lk.unlock();
        cv.notify_all();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return Buffer.empty();
    }
};