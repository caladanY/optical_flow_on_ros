#include <iostream>
#include <string>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "ros/ros.h"
#include "std_msgs/String.h"


using namespace std;
using namespace cv;

extern void trackFeatures(const cv::Mat &img_l, const cv::Mat &img_r, std::vector<cv::Point2f> &features_l, std::vector<cv::Point2f> &features_r, std::vector<int> &status, int stereo);


int main(int argc, char *argv[])
{
    //import images
    Mat imgSeq[10];
    cout<<"Test Completed"<<endl;
    waitKey(0);
    return 0;
}
