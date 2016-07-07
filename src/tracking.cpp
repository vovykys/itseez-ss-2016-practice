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
	return 0;
}
template<class T>
void quickSortR( std::vector<T> a, long N)
{
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
}

#include <iostream>
using std::cout;
using std::endl;

Rect MedianFlowTracker::Track(const cv::Mat &frame)
{

	std::vector<Point2f> prevPts;
	std::vector<Point2f> nextPts;
	std::vector<uchar> status;
	std::vector<float> err;
	Mat pos = frame_(position_);
	cout << "0000" << endl;
	cv::goodFeaturesToTrack(pos, prevPts, 100,0.01,5);
	cout << prevPts.size() << endl;
	int k = 0;
	cout << "1111" << endl;
	cv::calcOpticalFlowPyrLK(frame_, frame, prevPts, nextPts, status, err);
	cout << "2222" << endl;
	for (int i = 0; i <status.size(); i++)
	{
		if (!status[i])
		{
			nextPts.erase(nextPts.begin() + (i - k));
			prevPts.erase(nextPts.begin() + (i - k));
			k++;
		}
	}
	std::vector<double> dx;
	std::vector<double> dy;

	int x = 0, y = 0;
	for (int i = 0; i <prevPts.size(); i++)
	{
		x = abs(prevPts[i].x - nextPts[i].x);
		y = abs(prevPts[i].y - nextPts[i].y);
		dx.push_back(x);
		dy.push_back(y);
	}
	
	
		quickSortR(dx,dx.size());
		quickSortR(dy, dy.size());

		int medianX = dx[dx.size() / 2];
		int medianY = dy[dy.size() / 2];

		position_.x += medianX;
		position_.y += medianY;

		std::vector<double> rPrev;
		std::vector<double> rNext;
		std::vector<double> dr;

		for(int i = 0; i <prevPts.size(); i++)
			for (int j = 0; j <prevPts.size(); j++)
			{
				rPrev[i] = sqrt(pow(prevPts[i].x - prevPts[j].x, 2) + pow(prevPts[i].y - prevPts[j].y, 2));
				rNext[i] = sqrt(pow(nextPts[i].x - nextPts[j].x, 2) + pow(nextPts[i].y - nextPts[j].y, 2));
				dr[i] = rNext[i] / rPrev[i];
			}

		quickSortR(dr, dr.size());
		int medianSize = dr[dr.size() / 2];

		position_.height *= medianSize;
		position_.width *= medianSize;

		return position_;
}