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
	frame_= frame;
	position_=roi;
}

Rect MedianFlowTracker::Track(const cv::Mat &frame)
{

	std::vector<Point2f> prevPts;
	std::vector<Point2f> nextPts;
	std::vector<bool> status;
	std::vector<bool> err;
	cv::goodFeaturesToTrack(frame_, prevPts, 100,0.01,5);
	int k = 0;
	cv::calcOpticalFlowPyrLK(frame_, frame, prevPts, nextPts, status, err);
	for (int i = 0; i <status.size(); i++)
	{
		if (status[i] == false)
		{
			nextPts.erase(nextPts.begin() + (i - k));
			prevPts.erase(nextPts.begin() + (i - k));
			k++;
		}
	}
	std::vector<float> dx;
	std::vector<float> dy;

	int x = 0, y = 0;
	for (int i = 0; i <prevPts.size(); i++)
	{
		x = abs(prevPts[i].x - nextPts[i].x);
		y = abs(prevPts[i].y - nextPts[i].y);
		dx.push_back(x);
		dy.push_back(y);
	}
	
	template<class T>
	void quickSortR(T* a, long N) {
		// На входе - массив a[], a[N] - его последний элемент.

		long i = 0, j = N - 1; 		// поставить указатели на исходные места
		T temp, p;

		p = a[N >> 1];		// центральный элемент

							// процедура разделения
		do {
			while (a[i] < p) i++;
			while (a[j] > p) j--;

			if (i <= j) {
				temp = a[i]; a[i] = a[j]; a[j] = temp;
				i++; j--;
			}
		} while (i <= j);

		quickSortR(dx,dx.size());
		quickSortR(dy, dy.size());

		int medianX = dx[dx.size() / 2];
		int medianY = dy[dy.size() / 2];
		

}