#ifndef _UPLINK_HPP_
#define _UPLINK_HPP_

#include "protocol.hpp"
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::chrono_literals;

class Uplink {
private:
  Message datasend; // 发送消息
  Message datarecv; // 接收消息
  Message temp;     // 缓存
  enum {
    GYRO = 0,
    ROBOTPOSITION,
    EXROBOTPOSITION,
    GAME,
    REDROBITHP,
    BLUEROBITHP,
    BUILDINGHP,
    SHOOTSTAUTS,
    ENEMYROBOTPOSITION,
    ENEMYEXROBOTPOSITION,
    AUTOAIM
  };                                 // 功能ID
  void (*PublishFunc[9])(uint8_t *); // 发布消息函数指针

  std::unique_ptr<IoContext> owned_ctx_;

  void InitPublisher(); // 初始化发布者
  bool debug_;          // 调试标志

public:
  /**
   * @brief 默认构造函数
   */
  Uplink();
  /**
   * @brief 向下位机发送数据
   * @template T 数据类型
   * @param type 消息类型
   * @param buffer 数据
   */
  template <typename T> bool Send(uint8_t type, T *buffer) {
    this->datasend.start = 's';
    this->datasend.end = 'e';
    this->datasend.type = type;
    memcpy(this->datasend.buffer, (uint8_t *)buffer, sizeof(T));
    auto data = toVector(this->datasend);
    try {
      stm32_serial->port()->send(data);
      return true;
    } catch (const std::exception &e) {
      std::cout << "!!!!!!!!消息发送失败!!!!!!!!" << std::endl;
      std::cout << e.what() << std::endl;
      return false;
    }
  }
  /**
   * @brief 从下位机接收消息
   */
  void Recv();
  /**
   * @brief 初始化对象 ,消息缓冲区置零
   */
  void Init();

  /**
   * @brief 重开
   */
  bool Reopen();

  /**
   * @brief 发布默认tf树假信息话题进行debug
   * @param pub 发布者
   */
  void PublishGyroDefault();

  /**
   * @brief 发布陀螺仪信息话题
   * @param pub 发布者
   * @param buffer 发送数据
   */
  void PublishGyro(uint8_t *buffer);

  int reopen_count = 0;
};

/**
 * @brief 发布我方机器人位置信息话题(1 2 3 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishRobotPosition(uint8_t *buffer);

/**
 * @brief 发布我方机器人位置信息话题(4 5 7 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishExRobotPosition(uint8_t *buffer);

/**
 * @brief 发布比赛信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishGameInfo(uint8_t *buffer);

/**
 * @brief 发布红方机器人血量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishRedRobotHP(uint8_t *buffer);

/**
 * @brief 发布蓝方机器人血量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishBlueRobotHP(uint8_t *buffer);

/**
 * @brief 发布建筑血量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishBuildingHP(uint8_t *buffer);

/**
 * @brief 发布发射状态量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishShootStauts(uint8_t *buffer);

/**
 * @brief 发布敌方机器人位置信息话题(1 2 3 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishEnemyRobotPosition(uint8_t *buffer);

/**
 * @brief 发布敌方机器人位置信息话题(4 5 7 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishEnemyExRobotPosition(uint8_t *buffer);

#endif