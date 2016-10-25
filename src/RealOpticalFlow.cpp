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
    stringstream SSfileName;
    string fileName;
    for(int i = 0; i < 8; i++){
//        SSfileName<<"/home/lf/workspace/OpticalFlow/Army/frame"<<i+7<<".png";

        SSfileName<<"/home/lf/workspace/OpticalFlow/images/"<<i+1<<".jpg";
        SSfileName>>fileName;
        cout<<fileName<<endl;
        imgSeq[i] = imread(fileName,1);
        if(imgSeq[i].data==NULL){
            cout<<"File is not found"<<endl;
            return 1;
        }
        SSfileName.clear();
    }

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
    waitKey(0);
    return 0;
}
