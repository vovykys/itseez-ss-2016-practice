#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;
  if (name == "median_flow") {
	  return std::make_shared<MedianFlowTracker>();
  }
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{

}