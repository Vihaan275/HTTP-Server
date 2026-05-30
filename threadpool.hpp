#pragma once
#include <cassert>
#include <functional>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
class ThreadPool{
    private:
        
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  std::mutex task_mutex;
  std::condition_variable cv,done_cv;
  bool stop = false;

    public:
ThreadPool(int n);
~ThreadPool();
void done_with_tasks();
void thread_work();
void add_task(std::function<void()> task);
};
