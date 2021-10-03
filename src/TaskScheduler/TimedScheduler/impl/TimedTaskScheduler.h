#ifndef __TIMED_TASK_SCHEDULER_H__
#define __TIMED_TASK_SCHEDULER_H__
/**
 * @file TimedTaskScheduler.h
 * @author cloudicen cloudicen@foxmail.com)
 * @brief ��ʱ���������ʵ��
 * @version 0.1
 * @date 2021-10-03
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <assert.h>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "../../TaskSchedulerBase.h"

class TimedTaskScheduler;

/**
 * @brief ��ʱ����ڵ�
 *
 */
class TimedTaskNode {
public:
  explicit TimedTaskNode(
      int _id, std::chrono::time_point<std::chrono::system_clock> _expireTime,
      std::function<void()> &&_callBack, int _interval, bool _repeat)
      : id(_id), expireTime(_expireTime), callBack(_callBack),
        interval(_interval), repeatTask(_repeat){};
  ~TimedTaskNode() = default;

  bool operator>(const TimedTaskNode &other) const {
    return this->expireTime > other.expireTime;
  }

private:
  int id;                                                        //����id
  std::chrono::time_point<std::chrono::system_clock> expireTime; //����ʱ��
  int interval;                   //ִ�м��(ms)
  std::function<void()> callBack; //����ص�����
  bool repeatTask;                //�Ƿ�Ϊ�ظ�����

  //��Ԫ��
  friend TimedTaskScheduler;
};

/**
 * @brief ��ʱ���������
 *
 */
class TimedTaskScheduler : public TaskSchedulerBase {
private:
  /**
   * @brief ά������ڵ�С����
   *
   */
  void maintainHeap();

  /**
   * @brief ����ڵ����
   *
   * @param node �ڵ�ָ��
   */
  void pushHeap(TimedTaskNode *node);

  /**
   * @brief ����ڵ����
   *
   */
  void popHeap();

public:
  TimedTaskScheduler() = default;
  ~TimedTaskScheduler() = default;
  TimedTaskScheduler(const TimedTaskScheduler &) = delete;

  /**
   * @brief ����������������
   *
   * @param options ����ѡ��
   * @return int �ɹ����ط��������id��ʧ�ܷ���-1
   */
  int addTask(std::function<void()> &&Task,
              std::initializer_list<void *> options) override;

  /**
   * @brief �����������
   *
   * @param id ����id
   * @param timeout �µȴ�ʱ��
   * @param isRepeatTask �Ƿ�Ϊ�ظ�����
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  int adjustTask(int id, std::initializer_list<void *> options) override;

  /**
   * @brief   �Ƴ�һ������
   *
   * @param taskId ����id
   * @param doCall �Ƴ�֮ǰ�Ƿ�ִ������
   * @return int �ɹ���������id��ʧ�ܷ���-1
   */
  int removeTask(int id, bool doCall) override;

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
  std::vector<TimedTaskNode *> timerHeap; //ʱ�������С����
  std::unordered_map<int, std::unique_ptr<TimedTaskNode>>
      nodeRegestry;        //����ڵ��б�
  std::shared_mutex mutex; //ʵ��ȫ�ֻ�����
};

#endif
