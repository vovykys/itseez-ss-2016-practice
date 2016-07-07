#include <iostream>
#include <string>
#include "detection.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "tracking.hpp"
using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
"{ v video        | <none> | video to process                         }"
"{ h ? help usage |        | print help message                       }";

struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};

void OnMouse(int event, int x, int y, int flags, void* userdata) {
	MouseCallbackState* mouse = reinterpret_cast<MouseCallbackState*>(userdata);
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		mouse->is_selection_started = true;
		mouse->is_selection_finished = false;
		mouse->point_first.x = x;
		mouse->point_first.y = y;

		//GetCursorPos(&mouse.point_first);


	}
	if (event == cv::EVENT_LBUTTONUP)
	{
		mouse->is_selection_started = false;
		mouse->is_selection_finished = true;
		//GetCursorPos(&mouse.point_second);
		mouse->point_second.x = x;
		mouse->point_second.y = y;

	}
	if (event == cv::EVENT_MOUSEMOVE&&mouse->is_selection_finished == false)
	{
		//GetCursorPos(&mouse.point_second);
		mouse->point_second.x = x;
		mouse->point_second.y = y;

	}
}


int main(int argc, const char** argv) {
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	// If help option is given, print help message and exit.
	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	const string kSrcWindowName = "Detected video";
	VideoCapture video;
	video.open(parser.get<string>("video"));
	if (!video.isOpened()) {
		return 0;
	}
	const int kWaitKeyDelay = 10;
	namedWindow(kSrcWindowName);

  MouseCallbackState mouse_state;
  mouse_state.is_selection_started = false;
  mouse_state.is_selection_finished = false;
  setMouseCallback(kSrcWindowName, OnMouse, &mouse_state);

  Mat src;
  video >> src;
  while (!mouse_state.is_selection_finished) {
	  imshow(kSrcWindowName, src);
	  waitKey(100);
	  video >> src;
  }
  Rect roi(mouse_state.point_first, mouse_state.point_second);
  MedianFlowTracker track;
  Mat src_gray;
  cvtColor(src, src_gray, CV_BGR2GRAY);
  track.Init(src_gray, roi);
  
  while (true) {

	 
	  video >> src;
	  if (src.empty()) {
		  break;
	  }
	  rectangle(src, roi, Scalar(0, 255, 0), 2);
	  cvtColor(src, src_gray, CV_BGR2GRAY);
	  roi = track.Track(src_gray);

	  // Show source image.
	  imshow(kSrcWindowName, src);
	  if (waitKey(kWaitKeyDelay) >= 0) break;
  }

  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
