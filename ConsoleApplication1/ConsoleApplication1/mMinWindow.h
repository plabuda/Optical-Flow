#pragma once
class mMinWindow
{
private:
	int x;
	int y;
	int w;
	int h;

	int iRefreshCounter = 0;
	cv::RNG rng;
	cv::Rect rRect;
	cv::Mat  mColorFrame, mPrevGrayFrame, mGrayFrame;

	std::vector<uchar> status;
	std::vector<float> err;
	std::vector<cv::Point2f> corners[2];

	cv::Size winSize, subPixWinSize;
	cv::TermCriteria termcrit;

public:
	mMinWindow();
	mMinWindow(int, int, int, int, cv::Size, cv::Size, cv::TermCriteria);
	~mMinWindow();

	int getWidth();
	int getHeigth();

	inline double square(int);
	cv::Mat drawVectors(cv::Mat);

};

