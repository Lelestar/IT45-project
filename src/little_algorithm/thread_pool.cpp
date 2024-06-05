#include "thread_pool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this] { this->worker(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(jobs_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

/**
 * @brief Enqueue a job to be executed by the thread pool.
 * 
 * @param job The job to be executed.
 */
void ThreadPool::enqueue(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(jobs_mutex);
        jobs.push(std::move(job));
        ++tasks_in_progress;
    }
    condition.notify_one();
}

/**
 * @brief Wait for all jobs to finish.
 */
void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(jobs_mutex);
    done_condition.wait(lock, [this] { return jobs.empty() && !done; });
}

/**
 * @brief Worker function that is executed by each thread in the pool.
 */
void ThreadPool::worker() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(jobs_mutex);
            condition.wait(lock, [this] { return this->stop || !this->jobs.empty(); });
            if (this->stop && this->jobs.empty()) {
                done = true;
                done_condition.notify_all();
                return;
            }
            job = std::move(jobs.front());
            jobs.pop();
        }
        job();
        {
            std::unique_lock<std::mutex> lock(jobs_mutex);
            --tasks_in_progress;
            if (tasks_in_progress == 0 && jobs.empty()) {
                done_condition.notify_all();
            }
        }
    }
}
