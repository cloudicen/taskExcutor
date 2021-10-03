#ifndef __BASIC_TASK_SCHEDULER_H__
#define __BASIC_TASK_SCHEDULER_H__

/**
 * @file BasicTaskScheduler.h
 * @author cloudicen cloudicen@foxmail.com)
 * @brief �������������
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <vector>

#include "../../TaskSchedulerBase.h"

/**
 * @brief �������������
 * 
 */
class BasicTaskScheduler : public TaskSchedulerBase {
public:
  BasicTaskScheduler() = default;
  ~BasicTaskScheduler() = default;
  BasicTaskScheduler(const BasicTaskScheduler &) = delete;

  /**
   * @brief ����������������
   *
   * @param options ����ѡ��
   * @return int �ɹ����ط��������id��ʧ�ܷ���-1
   */
  int addTask(std::function<void()> &&Task,
              std::initializer_list<void *> options = {}) override;

  int adjustTask(int id, std::initializer_list<void *> options = {}) override {
    return 0;
  };
  int removeTask(int id, bool doCall) override { return 0; };

  /**
   * @brief ������������
   *
   */
  void clearTask() override;

  /**
   * @brief ��ȡ��������
   *
   * @return int ��������
   */
  int getTaskCount() override;

  /**
   * @brief ��ȡ���������б�
   *
   * @return std::pair<std::vector<std::function<void()>>, int>
   * <�����б�,�¸���������ʱ��(ms)>
   */
  std::pair<std::vector<std::function<void()>>, int> getReadyTask() override;

private:
  std::vector<std::function<void()>> taskList; //�����б�
  std::shared_mutex mutex;                     //ʵ��ȫ�ֻ�����
};

#endif