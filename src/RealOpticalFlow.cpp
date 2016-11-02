#include <iostream>
#include <string>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include <image_transport/image_transport.h>
#include "std_msgs/String.h"
#include "ros/ros.h"


using namespace std;
using namespace cv;

extern void trackFeatures(const cv::Mat &img_l, const cv::Mat &img_r, std::vector<cv::Point2f> &features_l, std::vector<cv::Point2f> &features_r, std::vector<int> &status, int stereo);


std::vector<cv::Mat> imgSeq;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    imgSeq.push_back(cv_bridge::toCvShare(msg, "bgr8")->image);
    cv::imshow("view", imgSeq.back());
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}



int main(int argc, char *argv[])
{
    ros::init(argc, argv, "optical_process");
    ros::NodeHandle nh;
    cv::namedWindow("view");
    cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("guidance/right_image", 1, imageCallback);
    ros::spin();
    cv::destroyWindow("view");
    return 0;
}
