

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
	mMinWindow minFrame = mMinWindow(400, 500, 200, 400, winSize, subPixWinSize, termcrit);


	while (true)
	{
		static Mat frame;

		cap >> frame;

		if (frame.empty()) {
			fprintf(stderr, "Error: Hmm. The end came sooner than we thought.\n");
			return -1;
		}

		imshow("Optical Flow", minFrame.drawVectors(frame));
		int key_pressed = waitKey(1);
		if (key_pressed == 'q') {
			break;
		}	
	}
}