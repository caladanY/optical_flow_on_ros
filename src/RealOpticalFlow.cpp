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



void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}



int main(int argc, char *argv[])
{
    Mat imgSeq[10];
    ros::init(argc, argv, "optical_process");
    ros::NodeHandle nh;
    cv::namedWindow("view");
    cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("guidance/right_image", 1, imageCallback);
    ros::spin();
    cv::destroyWindow("view");


    vector<Point2f> features;
    vector<int> status;
    status.resize(100);
    for(int i=0; i<100;i++){
        status[i]=2;
    }
    Mat img_r;
    vector<Point2f> features_r;
    vector<Point2f> pre_features;
    Mat imgResult;

    imgSeq[0].copyTo(imgResult);

    for(int i = 0; i < 8; i++){
        trackFeatures(imgSeq[i],img_r,features,features_r,status,0);
        if(i==1){
            for(int j=0; j<100; j++){
                arrowedLine(imgResult,features[j],pre_features[j],CV_RGB(255,0,0),2,8,0);
            }
        }
        pre_features=features;
    }
    imshow("figure 1",imgResult);
    return 0;
}
