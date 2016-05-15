#pragma once

using namespace cv;
using namespace std;

class mMinWindow
{
private:
	int x;
	int y;
	int w;
	int h;

	int iRefreshCounter = 0;
	RNG rng;
	Rect rRect;
	Mat  mColorFrame, mPrevGrayFrame, mGrayFrame;

	vector<uchar> status;
	vector<float> err;
	vector<cv::Point2f> corners[2];

	Size winSize, subPixWinSize;
	TermCriteria termcrit;

public:
	mMinWindow();
	mMinWindow(int, int, int, int, Size, Size, TermCriteria);
	~mMinWindow();

	int getWidth();
	int getHeigth();

private:
	inline double square(int);

public:
	Mat drawVectors(cv::Mat);

};

