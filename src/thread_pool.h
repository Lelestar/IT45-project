#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();
    void enqueue(std::function<void()> job);
    void wait();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;
    std::mutex jobs_mutex;
    std::condition_variable condition;
    std::condition_variable done_condition;
    bool stop;
    bool done;
    size_t tasks_in_progress;

    void worker();
};

#endif // THREAD_POOL_H
