cmake_minimum_required(VERSION 2.8.3)
project(optical_process)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES optical_flow
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(optical_process src/RealOpticalFlow.cpp)
target_link_libraries(optical_process ${catkin_LIBRARIES})
find_package(OpenCV REQUIRED)
target_link_libraries(optical_process ${OpenCV_LIBS})

find_package(klt_feature_tracker)
include_directories(${klt_feature_tracker_INCLUDE_DIRS})
target_link_libraries(optical_process ${klt_feature_tracker_LIBRARIES})
