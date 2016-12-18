#pragma once
#include "opencv2/opencv.hpp"

class mMinWindow
{
private:
	int iRefreshCounter = 0;
    constexpr static int MAX_REFRESH_COUNTER = 30;
    cv::Rect rRect;
    cv::Mat  mColorFrame, mPrevGrayFrame;

    std::vector<uchar> status;
    std::vector<float> err;
    std::vector<cv::Point2f> corners[2];
    std::vector<std::pair<cv::Point2f, cv::Point2f>> resultVector;
    cv::Size winSize /*tu się ładuje wynik optical flowa*/, subPixWinSize; //cornerSubPix
    cv::TermCriteria termcrit;

public:
	mMinWindow();
    mMinWindow(cv::Rect, cv::Size, cv::Size, cv::TermCriteria);
	~mMinWindow();

    int getWidth() const;
    int getHeigth() const;
    std::vector<std::pair<cv::Point2f, cv::Point2f>> getResultVector() const;

private:
	inline double square(int);

public:
    cv::Mat drawVectors(cv::Mat);

};

