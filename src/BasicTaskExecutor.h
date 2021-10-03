#ifndef __BASIC_TASK_EXECUTOR_H__
#define __BASIC_TASK_EXECUTOR_H__
/**
 * @file BasicTaskExecutor.h
 * @author cloudicen cloudicen@foxmail.com)
 * @brief ��������ִ����
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "GlobalTaskManager.h"
#include "TaskScheduler/BasicScheduler/BasicTaskSchedulerProxy.h"

#include <functional>
#include <future>
#include <memory>
#include <thread>

/**
 * @brief ��������ִ����
 * 
 */
class BasicTaskExecutor {
private:
  /**
   * @brief ����ģʽ��˽�й��캯��
   *
   */
  BasicTaskExecutor() : scheduler(new BasicTaskSchedulerProxy()) {
    GlobalTaskManager::addScheduler(this->scheduler->getSchedulerBase());
  };

public:
  /**
   * @brief ��ȡ����ִ��������ʵ��
   *
   * @return BasicTaskExecutor* ָ��ʵ����ָ��
   */
  static BasicTaskExecutor *getInstance();

  /**
   * @brief ����������ͬʱ��ȫ��TaskManager���Ƴ��õ�����
   *
   */
  ~BasicTaskExecutor() {
    GlobalTaskManager::removeScheduler(this->scheduler->getSchedulerBase());
  };

  /**
   * @brief ��Ӵ�future����ͨ����
   *
   * @tparam F callable
   * @tparam Args ת������
   * @param f callable����
   * @param args callable�������
   * @return std::pair<int, std::future<decltype(f(args...))>> ����id��furture
   */
  template <typename F, typename... Args>
  static auto addTaskWithFuture(F &&f, Args &&... args)
      -> std::pair<int, std::future<decltype(f(args...))>> {
    auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    int id = instance->scheduler->addTask([taskPtr]() { (*taskPtr)(); });
    //֪ͨ������ȹ������������������
    GlobalTaskManager::schedulerOnNewTask(
        instance->scheduler->getSchedulerBase());
    return {id, taskPtr->get_future()};
  }
  /**
   * @brief �����ͨ����
   *
   * @tparam F callable
   * @tparam Args any
   * @param f callable����
   * @param args callable�������
   * @return int ����id
   */
  template <typename F, typename... Args>
  static auto addTask(F &&f, Args &&... args) -> int {
    auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    int id = instance->scheduler->addTask(task);
    //֪ͨ������ȹ������������������
    GlobalTaskManager::schedulerOnNewTask(
        instance->scheduler->getSchedulerBase());
    return id;
  }
  /**
   * @brief ��������б�
   *
   */
  void clearTask() { scheduler->clearTask(); }

private:
  std::unique_ptr<BasicTaskSchedulerProxy> scheduler; //������ʵ��

private:
  static std::once_flag constructFlag; //���ι���flag
  static BasicTaskExecutor *instance;  //����ָ��

  // ------------  �Զ�����  ------------ //
private:
  class Garbo //����Ϊ˽�з�ֹ������
  {
  public:
    ~Garbo() //ʵ��ȥ����new�ĵ�������
    {
      if (BasicTaskExecutor::instance != NULL) {
        delete BasicTaskExecutor::instance;
        BasicTaskExecutor::instance = nullptr;
      }
    }
  };
  static Garbo garbo;
};

#endif