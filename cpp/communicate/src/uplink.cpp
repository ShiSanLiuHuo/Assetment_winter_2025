#include "communicate/uplink.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

#include "communicate/protocol.hpp"

Uplink::Uplink() : owned_ctx_{new IoContext(2)} {
  this->debug_ = this->declare_parameter("debug", false);
  InitPublisher();
}

Uplink::~Uplink(){
  delete owned_ctx_;
  owned_ctx_ = nullptr;
};

void Uplink::Init() {
  // 初始化对象，消息缓冲区置零
  memset(&datasend, 0, sizeof(datasend));
  memset(&datarecv, 0, sizeof(datarecv));
  memset(&temp, 0, sizeof(temp));
}

bool Uplink::Reopen() {
  if (reopen_count++ > 5) {
    return false;
  }
  std::cout << "Attempting to reopen" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return Reopen();
}

void Uplink::Recv() {
  if (debug_) {
    PublishGyroDefault();
    return;
  }
  try {
    std::vector<uint8_t> data(sizeof(Message));
    // 模拟接收数据
    datarecv = fromVector(data);
  } catch (const std::exception &e) {
    std::cout << "!!!!!!!!消息接收失败!!!!!!!!" << std::endl;
    std::cout << e.what() << std::endl;
    return;
  }

  if (datarecv.start != 's' || datarecv.end != 'e' || datarecv.type < 0xB0 ||
      datarecv.type > 0xB6) {
    std::cout << "!!!!!!!!数据包校验失败!!!!!!!!" << std::endl;
    return;
  } else {
    datarecv.type -= 0xB0;
    PublishFunc[datarecv.type](datarecv.buffer);
  }
}

void Uplink::InitPublisher() {
  PublishFunc[GYRO] = &PublishGyro;
  PublishFunc[ROBOTPOSITION] = &PublishRobotPosition;
  PublishFunc[EXROBOTPOSITION] = &PublishExRobotPosition;
  PublishFunc[GAME] = &PublishGameInfo;
  PublishFunc[REDROBITHP] = &PublishRedRobotHP;
  PublishFunc[BLUEROBITHP] = &PublishBlueRobotHP;
  PublishFunc[BUILDINGHP] = &PublishBuildingHP;
  PublishFunc[SHOOTSTAUTS] = &PublishShootStauts;
  PublishFunc[ENEMYROBOTPOSITION] = &PublishEnemyRobotPosition;
  PublishFunc[ENEMYEXROBOTPOSITION] = &PublishEnemyExRobotPosition;
}

/**
 * @brief 发布默认tf树假信息话题
 * @param pub 发布者
 */
void Uplink::PublishGyroDefault() {
  std::cout << "Publishing default gyro data: yaw=0, pitch=0" << std::endl;
}

/**
 * @brief 发布陀螺仪信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void Uplink::PublishGyro(uint8_t *buffer) {
  GyroFeedback_s *data = (GyroFeedback_s *)(buffer);
  std::cout << "Publishing gyro data: yaw=" << data->yaw
            << ", pitch=" << data->pitch << std::endl;
}

/**
 * @brief 发布我方机器人位置信息话题(1 2 3 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishRobotPosition(uint8_t *buffer) {
  RobotPositionFeedbackBuffer *data = (RobotPositionFeedbackBuffer *)buffer;
  std::cout << "Publishing robot position data: "
            << "standard_1_x=" << data->standard_1_x
            << ", standard_1_y=" << data->standard_1_y << ", "
            << "standard_2_x=" << data->standard_2_x
            << ", standard_2_y=" << data->standard_2_y << ", "
            << "standard_3_x=" << data->standard_3_x
            << ", standard_3_y=" << data->standard_3_y << std::endl;
}
/**
 * @brief 发布我方机器人位置信息话题(4 5 7 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishExRobotPosition(uint8_t *buffer) {
  ExRobotPositionFeedbackBuffer *data = (ExRobotPositionFeedbackBuffer *)buffer;
  std::cout << "Publishing ex-robot position data: "
            << "standard_4_x=" << data->standard_4_x
            << ", standard_4_y=" << data->standard_4_y << ", "
            << "standard_5_x=" << data->standard_5_x
            << ", standard_5_y=" << data->standard_5_y << ", "
            << "standard_7_x=" << data->standard_7_x
            << ", standard_7_y=" << data->standard_7_y << std::endl;
}
/**
 * @brief 发布比赛信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishGameInfo(uint8_t *buffer) {
  GameFeedbackBuffer *data = (GameFeedbackBuffer *)buffer;
  std::cout << "Publishing game info data: "
            << "enemy_team_color=" << data->enemy_team_color
            << ", game_time=" << data->game_time << ", "
            << "game_economy=" << data->game_economy
            << ", allowance_capacity=" << data->allowance_capacity << ", "
            << "left_purchase=" << data->left_purchase
            << ", right_purchase=" << data->right_purchase << std::endl;
}
/**
 * @brief 发布红方机器人血量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishRedRobotHP(uint8_t *buffer) {
  RedRobotHPFeedbackBuffer *data = (RedRobotHPFeedbackBuffer *)buffer;
  std::cout << "Publishing red robot HP data: "
            << "red_1_robot_HP=" << data->red_1_robot_HP
            << ", red_2_robot_HP=" << data->red_2_robot_HP << ", "
            << "red_3_robot_HP=" << data->red_3_robot_HP
            << ", red_4_robot_HP=" << data->red_4_robot_HP << ", "
            << "red_5_robot_HP=" << data->red_5_robot_HP
            << ", red_7_robot_HP=" << data->red_7_robot_HP << std::endl;
}
/**
 * @brief 发布蓝方机器人血量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishBlueRobotHP(uint8_t *buffer) {
  BlueRobotHPFeedbackBuffer *data = (BlueRobotHPFeedbackBuffer *)buffer;
  std::cout << "Publishing blue robot HP data: "
            << "blue_1_robot_HP=" << data->blue_1_robot_HP
            << ", blue_2_robot_HP=" << data->blue_2_robot_HP << ", "
            << "blue_3_robot_HP=" << data->blue_3_robot_HP
            << ", blue_4_robot_HP=" << data->blue_4_robot_HP << ", "
            << "blue_5_robot_HP=" << data->blue_5_robot_HP
            << ", blue_7_robot_HP=" << data->blue_7_robot_HP << std::endl;
}
/**
 * @brief 发布建筑血量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishBuildingHP(uint8_t *buffer) {
  BuildingHPFeedbackBuffer *data = (BuildingHPFeedbackBuffer *)buffer;
  std::cout << "Publishing building HP data: "
            << "red_outpost_HP=" << data->red_outpost_HP
            << ", red_base_HP=" << data->red_base_HP << ", "
            << "blue_outpost_HP=" << data->blue_outpost_HP
            << ", blue_base_HP=" << data->blue_base_HP << std::endl;
}
/**
 * @brief 发布发射状态量信息话题
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishShootStauts(uint8_t *buffer) {
  ShootStautsBuffer *data = (ShootStautsBuffer *)buffer;
  std::cout << "Publishing shoot status data: "
            << "left_real_heat=" << data->left_real_heat
            << ", right_real_heat=" << data->right_real_heat << ", "
            << "left_bullet_speed=" << data->left_bullet_speed
            << ", right_bullet_speed=" << data->right_bullet_speed << ", "
            << "game_progress=" << data->game_progress << std::endl;
}

/**
 * @brief 发布敌方机器人位置信息话题(1 2 3 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishEnemyRobotPosition(uint8_t *buffer) {
  EnemyRobotPositionFeedbackBuffer *data =
      (EnemyRobotPositionFeedbackBuffer *)buffer;
  std::cout << "Publishing enemy robot position data: "
            << "enemy1_x_position=" << data->enemy1_x_position
            << ", enemy1_y_position=" << data->enemy1_y_position << ", "
            << "enemy2_x_position=" << data->enemy2_x_position
            << ", enemy2_y_position=" << data->enemy2_y_position << ", "
            << "enemy3_x_position=" << data->enemy3_x_position
            << ", enemy3_y_position=" << data->enemy3_y_position << std::endl;
}

/**
 * @brief 发布敌方机器人位置信息话题(4 5 7 号机器人)
 * @param pub 发布者
 * @param buffer 发送数据
 */
void PublishEnemyExRobotPosition(uint8_t *buffer) {
  EnemyExRobotPositionFeedbackBuffer *data =
      (EnemyExRobotPositionFeedbackBuffer *)buffer;
  std::cout << "Publishing enemy ex-robot position data: "
            << "enemy4_x_position=" << data->enemy4_x_position
            << ", enemy4_y_position=" << data->enemy4_y_position << ", "
            << "enemy5_x_position=" << data->enemy5_x_position
            << ", enemy5_y_position=" << data->enemy5_y_position << ", "
            << "enemy7_x_position=" << data->enemy7_x_position
            << ", enemy7_y_position=" << data->enemy7_y_position << std::endl;
}
