#ifndef __BASIC_TASK_SCHEDULER_PROXY_H__
#define __BASIC_TASK_SCHEDULER_PROXY_H__

/**
 * @file BasicTaskSchedulerProxy.h
 * @author cloudicen cloudicen@foxmail.com)
 * @brief ������������������࣬�����޶��������
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "impl/BasicTaskScheduler.h"
#include <memory>

/**
 * @brief ������������������࣬�����޶��������
 *
 */
class BasicTaskSchedulerProxy {
private:
  std::unique_ptr<BasicTaskScheduler> BasicSchedulerBase;//ʵ�ʵĵ�����ʵ��

public:
  BasicTaskSchedulerProxy() : BasicSchedulerBase(new BasicTaskScheduler()){};
  ~BasicTaskSchedulerProxy() = default;

/**
 * @brief �������
 * 
 * @param task void()���͵�callable����
 * @return int �ɹ���������id��ʧ�ܷ���-1
 */
  int addTask(std::function<void()>&& task) {
    return BasicSchedulerBase->addTask(std::forward<std::function<void()>&&>(task));
  }

  /**
   * @brief ������������
   *
   */
  void clearTask() { BasicSchedulerBase->clearTask(); }

  /**
   * @brief ��ȡ��������
   *
   * @return int ��������
   */
  int getTaskCount() { return BasicSchedulerBase->getTaskCount(); }

  /**
   * @brief ��ȡ���������б�
   *
   * @return std::pair<std::vector<std::function<void()>>, int>
   * <�����б�,�¸���������ʱ��(ms)>
   */
  std::pair<std::vector<std::function<void()>>, int> getReadyTask() {
    return BasicSchedulerBase->getReadyTask();
  }

  /**
   * @brief ��ȡʵ�ʵĵ�����ʵ��ָ��
   *
   * @return TaskSchedulerBase* ������ʵ��ָ��
   */
  TaskSchedulerBase *getSchedulerBase() { return BasicSchedulerBase.get(); }
};

#endif