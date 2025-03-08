#include "communicate/downlink.hpp"

#include <bits/types/struct_tm.h>
#include <std_msgs/msg/detail/float32_multi_array__struct.hpp>

Downlink::Downlink(Uplink* uplink):
    Node("communicate_subscription"),
    uplink_(uplink) {
    this->PTZSubTopic = "/shoot_info";
    this->MainPTZSubTopic = "/behaviortree/main";
    this->ChassisVelSubTopic = "cmd_vel";
    this->InteractionSubTopic = "/behaviortree/interaction";
    this->ModuleSubTopic = "/behaviortree/module";
    this->ShootStatusSubTopic = "/communicate/shootstatus";

    this->debug = this->declare_parameter("debug", false);

    // PTZ_sub = this->create_subscription<communicate::msg::SerialInfo>(
    //     PTZSubTopic,
    //     rclcpp::SensorDataQoS(),
    //     std::bind(&Downlink::PTZCB, this, std::placeholders::_1)
    // );
    MainPTZ_sub = this->create_subscription<std_msgs::msg::Float32MultiArray>(
        MainPTZSubTopic,
        10,
        std::bind(&Downlink::MainPTZCB, this, std::placeholders::_1)
    );
    ChassisVel_sub = this->create_subscription<geometry_msgs::msg::Twist>(
        ChassisVelSubTopic,
        10,
        std::bind(&Downlink::ChassisVelCB, this, std::placeholders::_1)
    );
    Interaction_sub = this->create_subscription<std_msgs::msg::Int32MultiArray>(
        InteractionSubTopic,
        10,
        std::bind(&Downlink::InteractionCB, this, std::placeholders::_1)
    );
    Module_sub = this->create_subscription<std_msgs::msg::Int32MultiArray>(
        ModuleSubTopic,
        10,
        std::bind(&Downlink::ModuleCB, this, std::placeholders::_1)
    );
    // ShootStauts_sub = this->create_subscription<std_msgs::msg::Int32MultiArray>(
    //     ShootStautsSubTopic,
    //     10,
    //     std::bind(&Downlink::ShootStautsCB, this, std::placeholders::_1)
    // );
}

void Downlink::PTZCB(const communicate::msg::SerialInfo::SharedPtr msg) {
    if (this->debug) {
        return;
    }
    PtzControlBuffer tmp;
    tmp.find_bool = msg->is_find.data;
    tmp.yaw = msg->euler[0];
    tmp.pitch = msg->euler[1];
    tmp.distance = msg->distance;
    uplink_->Send(0xA0, &tmp);
}

void Downlink::MainPTZCB(const std_msgs::msg::Float32MultiArray::SharedPtr msg) {
    if (this->debug) {
        return;
    }
    MainptzControlBuffer tmp;
    tmp.yaw = msg->data[0];
    uplink_->Send(0xA1, &tmp);
}

void Downlink::ChassisVelCB(const geometry_msgs::msg::Twist::SharedPtr msg) {
    if (this->debug) {
        return;
    }
    ChassisControlBuffer tmp;
    tmp.vx = msg->linear.x;
    tmp.vy = msg->linear.y;
    uplink_->Send(0xA2, &tmp);
}

void Downlink::InteractionCB(const std_msgs::msg::Int32MultiArray::SharedPtr msg) {
    if (this->debug) {
        return;
    }
    InteractionControlBuffer tmp;
    tmp.type = msg->data[0];
    tmp.content = msg->data[1];
    uplink_->Send(0xA3, &tmp);
}

void Downlink::ModuleCB(const std_msgs::msg::Int32MultiArray::SharedPtr msg) {
    if (this->debug) {
        return;
    }
    ModuleControlBuffer tmp;
    tmp.type = msg->data[0];
    tmp.content = msg->data[1];
    uplink_->Send(0xA4, &tmp);
}

void Downlink::ShootStatusCB(const std_msgs::msg::Int32MultiArray::SharedPtr msg) {
    if (this->debug) {
        return;
    }
    ShootStatusBuffer tmp;
    tmp.left_real_heat = msg->data[0];
    tmp.right_real_heat = msg->data[1];
    tmp.left_bullet_speed = msg->data[2];
    tmp.right_bullet_speed = msg->data[3];
    tmp.game_progress = msg->data[4];
    uplink_->Send(0xA5, &tmp);
}