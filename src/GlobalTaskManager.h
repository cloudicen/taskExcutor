#ifndef __GLOBAL_TASK_MANAGER_H__
#define __GLOBAL_TASK_MANAGER_H__

#include "TaskScheduler/TaskSchedulerBase.h"
#include "ThreadPool/ThreadPool.h"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <queue>
#include <set>
#include <thread>

class GlobalTaskManager {
private:
  static GlobalTaskManager *instance;
  std::set<TaskSchedulerBase *> schedulerPolingList;
  std::set<TaskSchedulerBase *> schedulerWaitingList;

  std::thread *schedulerThread;
  bool stop = false;

  static std::once_flag constructFlag;

  std::mutex mtx_;
  std::condition_variable cv_;

  int minPollingInterval = 0;

  GlobalTaskManager(){};
  void polling();

public:
  static GlobalTaskManager *getInstance();

  ~GlobalTaskManager() { stopExcutor(); };

  void stopExcutor() {
    {
      std::unique_lock<std::mutex> ul(this->mtx_);
      this->stop = true;
    }
    this->cv_.notify_all();
    this->schedulerThread->join();
  }

  static void addScheduler(TaskSchedulerBase *);

  static void removeScheduler(TaskSchedulerBase *);

  static void schedulerOnNewTask(TaskSchedulerBase *);

private:
  class Garbo //设置为私有防止外界访问
  {
  public:
    ~Garbo() //实际去析构new的单例对象
    {
      if (GlobalTaskManager::instance != NULL) {
        delete GlobalTaskManager::instance;
        GlobalTaskManager::instance = nullptr;
      }
    }
  };
  static Garbo garbo;
};

#endif