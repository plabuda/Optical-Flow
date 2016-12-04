#pragma once

using namespace cv;
using namespace std;

class mMinWindow
{
private:
	int iRefreshCounter = 0;
	Rect rRect;
	Mat  mColorFrame, mPrevGrayFrame, mGrayFrame;

	vector<uchar> status;
	vector<float> err;
	vector<cv::Point2f> corners[2];
	vector<pair<cv::Point2f, cv::Point2f>> resultVector;
	Size winSize, subPixWinSize;
	TermCriteria termcrit;

public:
	mMinWindow();
	mMinWindow(Rect, Size, Size, TermCriteria);
	~mMinWindow();

    int getWidth() const;
    int getHeigth() const;
    vector<pair<cv::Point2f, cv::Point2f>> getResultVector() const;

private:
	inline double square(int);

public:
	Mat drawVectors(cv::Mat);

};

