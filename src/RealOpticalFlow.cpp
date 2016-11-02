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
std::vector<cv::Point2f> features;
std::vector<int> status;
std::vector<cv::Point2f> features_r;
std::vector<cv::Point2f> pre_features;
cv::Mat imgResult;
cv::Mat img_r;



void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    imgSeq.push_back(cv_bridge::toCvShare(msg, "bgr8")->image);
    cout<<"track features"<<endl;
    trackFeatures(imgSeq.back(),img_r,features,features_r,status,0);
    cout<<"pre_features size: "<<pre_features.size()<<"  features size: "<<features.size()<<endl;
    imgResult = imgSeq.back();
    for(int j = 0; j < 100; j++){
      if (pre_features.size() == 0)break;
      arrowedLine(imgResult,features[j],pre_features[j],CV_RGB(255,0,0),2,8,0);
      if(features[j] == cv::Point2f(-100, -100))break;
      cout<<features[j]<<endl;
    }
    cv::imshow("view", imgResult);
    cv::waitKey(30);
    pre_features.resize(features.size());
    pre_features = features;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}



int main(int argc, char *argv[])
{
    cout<<"start"<<endl;
    status.resize(100);
    for(int i=0; i<100;i++){
      status[i]=2;
    }

    ros::init(argc, argv, "optical_process");
    ros::NodeHandle nh;
    cv::namedWindow("view");
    cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("guidance/left_image", 1, imageCallback);
    ros::spin();
    cv::destroyWindow("view");
    return 0;
}
