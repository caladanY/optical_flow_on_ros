#include "ros/ros.h"
#include "std_msgs/String.h"

extern void trackFeatures(const cv::Mat &img_l, const cv::Mat &img_r, std::vector<cv::Point2f> &features_l, std::vector<cv::Point2f> &features_r, std::vector<int> &status,
                          int stereo);


