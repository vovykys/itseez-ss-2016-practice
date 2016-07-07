#include <iostream>
#include <string>
#include "detection.hpp"
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
"{ i image        | <none> | image to process                         }"
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ m model        | <none> |                                          }"
"{ h ? help usage |        | print help message                       }";


int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
	  parser.printMessage();
	  return 0;
  }

  CascadeDetector det;
  det.Init(parser.get<string>("model"));
  if (parser.has("image")) {
	 
	  // Read image.
	  Mat src = imread(parser.get<string>("image"));
	  if (src.empty()) {
		  cout << "Failed to open image file '" + parser.get<string>("image") + "'."
			  << endl;
		  return 0;
	  }
	  std::vector<cv::Rect> objects;
	  std::vector<double> scores;
	  det.Detect(src, objects, scores);
	  for (size_t i = 0; i < objects.size(); i++)
	  {
		  cv::rectangle(src, objects[i], CV_RGB(255, 0, 0));
	  }

	  // Show source image.
	  const string kSrcWindowName = "Detected image";
	  //const int kWaitKeyDelay = 1;
	  namedWindow(kSrcWindowName);
	  imshow(kSrcWindowName, src);
	  waitKey(0);
  }
  
  if (parser.has("video")) {

	   // Read video.
	  VideoCapture video;
	  video.open(parser.get<string>("video"));
	  if (!video.isOpened()) {
		  return 0;
	  }
	  const string kSrcWindowName = "Detected video";
	  const int kWaitKeyDelay = 10;
	  namedWindow(kSrcWindowName);

	  while (true) {
		  Mat src;
		  video >> src;
		  if (src.empty()) {
			  break;
		  }
		  std::vector<cv::Rect> objects;
		  std::vector<double> scores;
		  det.Detect(src, objects, scores);
		  for (size_t i = 0; i < objects.size(); i++)
		  {
			  cv::rectangle(src, objects[i], CV_RGB(255, 0, 0));
		  }

		  // Show source image.
		  imshow(kSrcWindowName, src);
		  if (waitKey(kWaitKeyDelay) >= 0) break;
		 
	  }
	  
  }

  if (parser.has("camera")) {

	  // Read video.
	  VideoCapture video;
	  video.open(0);
	  if (!video.isOpened()) {
		  return 0;
	  }
	  const string kSrcWindowName = "Detected video";
	  const int kWaitKeyDelay = 10;
	  namedWindow(kSrcWindowName);

	  while (true) {
		  Mat src;
		  video >> src;
		  if (src.empty()) {
			  break;
		  }
		  std::vector<cv::Rect> objects;
		  std::vector<double> scores;
		  det.Detect(src, objects, scores);
		  for (size_t i = 0; i < objects.size(); i++)
		  {
			  cv::rectangle(src, objects[i], CV_RGB(255, 0, 0));
		  }

		  // Show source image.
		  imshow(kSrcWindowName, src);
		  if (waitKey(kWaitKeyDelay) >= 0) break;

	  }

  }
  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
