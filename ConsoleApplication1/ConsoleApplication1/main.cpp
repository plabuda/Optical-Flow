

#include "stdafx.h"
#include <math.h>
#include "mMinWindow.h"



using namespace std;
using namespace cv;

int main(void)
{
	Size winSize(15, 15), subPixWinSize(15, 15);
	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.3);
	VideoCapture cap;

	cap.open("Highway.avi");

	if (!cap.isOpened()) {
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	namedWindow("Optical Flow", CV_WINDOW_AUTOSIZE);
	mMinWindow mMinFrame = mMinWindow(400, 500, 100, 400, winSize, subPixWinSize, termcrit);
	mMinWindow mMinFrame1 = mMinWindow(1400, 500, 100, 400, winSize, subPixWinSize, termcrit);
	
	cv::Mat mFrame_Wrapper(cv::Size(250, 400), CV_8UC3);

	while (true)
	{
		static Mat frame;

		cap >> frame;

		if (frame.empty()) {
			fprintf(stderr, "End of video");
			return -1;
		}

		mMinFrame.drawVectors(frame).copyTo(mFrame_Wrapper(cv::Rect(0, 0, 100, 400)));
		mMinFrame1.drawVectors(frame).copyTo(mFrame_Wrapper(cv::Rect(150, 0, 100, 400)));

		imshow("Optical Flow", mFrame_Wrapper);
		int key_pressed = waitKey(1);
		if (key_pressed == 'q') {
			break;
		}	
	}

	return 0;
}