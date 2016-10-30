#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <boost/foreach.hpp>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <image_transport/image_transport.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <iostream>
// A struct to hold the synchronized camera data 
// Struct to store stereo data
using namespace rosbag;

std::vector<StereoData> stereo_dataset;

class StereoData
{
public:
  sensor_msgs::Image::ConstPtr image_l, image_r;
  
  StereoData(const sensor_msgs::Image::ConstPtr &l_img, 
             const sensor_msgs::Image::ConstPtr &r_img) :
    image_l(l_img),
    image_r(r_img)
  {}
};

/**
 * Inherits from message_filters::SimpleFilter<M>
 * to use protected signalMessage function 
 */
template <class M>
class BagSubscriber : public message_filters::SimpleFilter<M>
{
public:
  void newMessage(const boost::shared_ptr<M const> &msg)
  {
    this->signalMessage(msg);
  }
};

// Callback for synchronized messages
void callback(const sensor_msgs::Image::ConstPtr &l_img, 
              const sensor_msgs::Image::ConstPtr &r_img)
{
  StereoData sd(l_img, r_img);
  // Stereo dataset is class variable to store data
  stereo_dataset.push_back(sd);
}

// Load bag
void loadBag(const std::string &filename)
{
  rosbag::Bag bag;
  bag.open(filename, rosbag::bagmode::Read);
  
  std::string image_ns_ = "Test";

  std::string l_cam = image_ns_ + "/left";
  std::string r_cam = image_ns_ + "/right";
  std::string l_cam_image = l_cam + "/image_raw";
  std::string r_cam_image = r_cam + "/image_raw";
  // Image topics to load
  std::vector<std::string> topics;
  topics.push_back(l_cam_image);
  topics.push_back(r_cam_image);
  
  rosbag::View view(bag, rosbag::TopicQuery(topics));
  
  // Set up fake subscribers to capture images
  BagSubscriber<sensor_msgs::Image> l_img_sub, r_img_sub;
  
  // Use time synchronizer to make sure we get properly synchronized images
  message_filters::TimeSynchronizer<sensor_msgs::Image, sensor_msgs::Image, sensor_msgs::CameraInfo, sensor_msgs::CameraInfo> sync(l_img_sub, r_img_sub, 25);
  sync.registerCallback(boost::bind(&callback, _1, _2, _3, _4));

  // Load all messages into our stereo dataset
  BOOST_FOREACH(rosbag::MessageInstance const m, view)
  {
    if (m.getTopic() == l_cam_image || ("/" + m.getTopic() == l_cam_image))
    {
      sensor_msgs::Image::ConstPtr l_img = m.instantiate<sensor_msgs::Image>();
      if (l_img != NULL)
        l_img_sub.newMessage(l_img);
    }
    
    if (m.getTopic() == r_cam_image || ("/" + m.getTopic() == r_cam_image))
    {
      sensor_msgs::Image::ConstPtr r_img = m.instantiate<sensor_msgs::Image>();
      if (r_img != NULL)
        r_img_sub.newMessage(r_img);
    }
    

  }
  bag.close();
}

