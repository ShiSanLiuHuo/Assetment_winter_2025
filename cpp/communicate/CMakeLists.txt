cmake_minimum_required(VERSION 3.22.1)
project(communication_project)

# 设置 C++ 标准
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找 ROS 2 依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# 添加头文件目录
include_directories(
  include
  ${rclcpp_INCLUDE_DIRS}
  ${std_msgs_INCLUDE_DIRS}
  ${geometry_msgs_INCLUDE_DIRS}
)

# 添加可执行文件
add_executable(communication_node
  src/uplink.cpp
  src/downlink.cpp
)

# 链接 ROS 2 库
ament_target_dependencies(communication_node
  rclcpp
  std_msgs
  geometry_msgs
)

# 安装可执行文件
install(TARGETS
  communication_node
  DESTINATION lib/${PROJECT_NAME}
)

# 导出依赖
ament_package()