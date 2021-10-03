#ifndef __TIMED_TASK_SCHEDULER_PROXY_H__
#define __TIMED_TASK_SCHEDULER_PROXY_H__
/**
 * @file TimedTaskSchedulerProxy.h
 * @author cloudicen cloudicen@foxmail.com)
 * @brief ��ʱ��������������࣬�����޶��������
 * @version 0.1
 * @date 2021-10-03
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "impl/TimedTaskScheduler.h"
#include <memory>

/**
 * @brief ��ʱ��������������࣬�����޶��������
 *
 */
class TimedTaskSchedulerProxy {
private:
  std::unique_ptr<TimedTaskScheduler> timedSchedulerBase; //ʵ�ʵĵ�����ʵ��

public:
  TimedTaskSchedulerProxy() : timedSchedulerBase(new TimedTaskScheduler()){};
  ~TimedTaskSchedulerProxy() = default;

  /**
   * @brief �������
   *
   * @param task void()���͵�callable����
   * @param timeout �ȴ�ʱ��
   * @param isRepeatTask �Ƿ�Ϊ�ظ�����
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  int addTask(std::function<void()> &&task, int timeout,
              bool isRepeatTask = false) {
    return timedSchedulerBase->addTask(
        std::forward<std::function<void()> &&>(task),
        {&timeout, &isRepeatTask});
  }

  /**
   * @brief �����������
   *
   * @param id ����id
   * @param timeout �µȴ�ʱ��
   * @param isRepeatTask �Ƿ�Ϊ�ظ�����
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  int adjustTask(int id, int timeout, bool isRepeatTask) {
    return timedSchedulerBase->adjustTask(id, {&timeout, &isRepeatTask});
  }

  /**
   * @brief ��������ȴ�ʱ��
   *
   * @param id ����id
   * @param timeout �µȴ�ʱ��
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  int adjustTask(int id, int timeout) {
    return timedSchedulerBase->adjustTask(id, {&timeout, nullptr});
  }

  /**
   * @brief ���������Ƿ��ظ�
   *
   * @param id ����id
   * @param isRepeatTask �Ƿ�Ϊ�ظ�����
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  int adjustTask(int id, bool isRepeatTask) {
    return timedSchedulerBase->adjustTask(id, {nullptr, &isRepeatTask});
  }

  /**
   * @brief �Ƴ�һ������
   *
   * @param id ����id
   * @param doCall �Ƴ�֮ǰ�Ƿ�ִ������
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  int removeTask(int id, bool doCall) {
    return timedSchedulerBase->removeTask(id, doCall);
  }

  /**
   * @brief ������������
   *
   */
  void clearTask() { timedSchedulerBase->clearTask(); }

  /**
   * @brief ��ȡ��������
   *
   * @return int ��������
   */
  int getTaskCount() { return timedSchedulerBase->getTaskCount(); }

  /**
   * @brief ��ȡ���������б�
   *
   * @return std::pair<std::vector<std::function<void()>>, int>
   * <�����б�,�¸���������ʱ��(ms)>
   */
  std::pair<std::vector<std::function<void()>>, int> getReadyTask() {
    return timedSchedulerBase->getReadyTask();
  }

  /**
   * @brief ��ȡʵ�ʵĵ�����ʵ��ָ��
   *
   * @return TaskSchedulerBase* ������ʵ��ָ��
   */
  TaskSchedulerBase *getSchedulerBase() { return timedSchedulerBase.get(); }
};

#endif