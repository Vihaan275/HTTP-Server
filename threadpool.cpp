#include <cassert>
#include "threadpool.hpp"
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

  // constructor makes a thread pool object with n threads
 ThreadPool::ThreadPool(int n) {
    for (int i = 0; i < n; i++) {
      workers.push_back(std::thread(&ThreadPool::thread_work, this));
    }
  }

void ThreadPool::done_with_tasks(){
  {
    std::unique_lock<std::mutex> mut_lock{task_mutex, std::defer_lock};
    mut_lock.lock();
    done_cv.wait(mut_lock, [this] {return tasks.empty();});
   stop= true;
  }
  cv.notify_all();
  }

void ThreadPool::thread_work() {

    std::unique_lock<std::mutex> mut_lock{task_mutex, std::defer_lock};
    while (true) {
      mut_lock.lock();
      cv.wait(mut_lock, [this] { return !tasks.empty() || stop; });
      if (stop && tasks.empty()) {
        return;
      };
      auto task = tasks.front();
      tasks.pop();
      mut_lock.unlock();
      
      //the actual thread work that will be done by the thread
      task();
      done_cv.notify_all();
    }
  }

void ThreadPool::add_task(std::function<void()> task) {

    std::unique_lock<std::mutex> mut_lock{task_mutex, std::defer_lock};
    mut_lock.lock();
    tasks.push(task);
    mut_lock.unlock();
    cv.notify_one();
  }


ThreadPool::~ThreadPool() {
    {
    std::unique_lock<std::mutex> mut_lock{task_mutex, std::defer_lock};
    mut_lock.lock();
    done_cv.wait(mut_lock, [this] {return tasks.empty() && stop;});
    }
    for (auto& w: workers) {
        w.join();
    }
  }


void print_num(int num){
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout<<num<<std::endl;
}

