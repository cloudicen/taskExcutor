#ifndef __TASK_SCHEDULER_BASE_H__
#define __TASK_SCHEDULER_BASE_H__
/**
 * @file TaskSchedulerBase.h
 * @author cloudicen cloudicen@foxmail.com)
 * @brief �������������
 * @version 0.1
 * @date 2021-10-03
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <functional>
#include <future>
#include <memory>
#include <queue>

/**
 * @brief �������������
 *
 */
class TaskSchedulerBase {
protected:
  int currentTaskId = 0;             //��ǰ�ѷ��������id�����id)
  std::priority_queue<int> reusedId; //������������id�б�

  /**
   * @brief ���һ�����õ�����id
   *
   * @return int ����id
   */
  int getNewTaskId() {
    int id = 0;
    if (this->reusedId.empty()) {
      id = currentTaskId++;
    } else {
      id = this->reusedId.top();
      this->reusedId.pop();
    }
    return id;
  }

  /**
   * @brief  ��������id
   *
   */
  void resetTaskId() {
    currentTaskId = 0;
    reusedId = std::priority_queue<int>();
  }

public:
  TaskSchedulerBase() = default;
  virtual ~TaskSchedulerBase() = default;

  /**
   * @brief ����������������
   *
   * @param options ����ѡ��
   * @return int �ɹ����ط��������id��ʧ�ܷ���-1
   */
  virtual int addTask(std::function<void()> &&,
                      std::initializer_list<void *> options) = 0;

  /**
   * @brief   �Ƴ�һ������
   *
   * @param taskId ����id
   * @param doCall �Ƴ�֮ǰ�Ƿ�ִ������
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  virtual int removeTask(int taskId, bool doCall) = 0;

  /**
   * @brief �����������
   *
   * @param taskId ����id
   * @param options �������ѡ��
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  virtual int adjustTask(int taskId, std::initializer_list<void *> options) = 0;

  /**
   * @brief ������������
   *
   */
  virtual void clearTask() = 0;

  /**
   * @brief ��ȡ��������
   *
   * @return int ��������
   */
  virtual int getTaskCount() = 0;

  /**
   * @brief ��ȡ���������б�
   *
   * @return std::pair<std::vector<std::function<void()>>, int>
   * <�����б�,�¸���������ʱ��(ms)>
   */
  virtual std::pair<std::vector<std::function<void()>>, int> getReadyTask() = 0;
};

#endif