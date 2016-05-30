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

	struct sVector;

	int iRefreshCounter = 0;
	RNG rng;
	Rect rRect;
	Mat  mColorFrame, mPrevGrayFrame, mGrayFrame;

	vector<uchar> status;
	vector<float> err;
	vector<cv::Point2f> corners[2];
	vector<pair<cv::Point2f, cv::Point2f>> resultVector;
	Size winSize, subPixWinSize;
	TermCriteria termcrit;
	vector<pair<cv::Point2f, cv::Point2f>> *V2P, *tmpV2P = new vector<pair<cv::Point2f, cv::Point2f>>;

public:
	mMinWindow();
	mMinWindow(int, int, int, int, Size, Size, TermCriteria, vector<pair<cv::Point2f, cv::Point2f>> *V2P);
	~mMinWindow();

	int getWidth();
	int getHeigth();
	vector<pair<cv::Point2f, cv::Point2f>> getResultVector();

private:
	inline double square(int);

public:
	Mat drawVectors(cv::Mat);

};

