#pragma once
#include "Vehicle.h"
#include <vector>
#include <opencv2/bgsegm.hpp>

class BGS
{
private:
	int frame;
	std::pair<cv::Point, cv::Point> p_pLine;
	cv::RNG rng;
	cv::Rect rRect, rCntRect;
	cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
	cv::Mat  mColorFrame, mColorFrame1, mMask, mThresholdOutput, mMaskG, ret[2];
	cv::Mat se1, se2;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>> vvpContours;
	//std::vector<cv::Rect> vrRects, vrPrevRects;
	std::vector<Vehicle> vrVehicles, vrPrevVehicles;
	
public:
	BGS();
	BGS::BGS(cv::Rect, int history, float varThreshold, int iDetectLineX1, int iDetectLineX2, int iDetectLineY1);
	~BGS();

	double square(int a);

	

	

private:
	void Refactor(cv::Mat &mArg, cv::Mat mWrapper, cv::Rect rRect);
	void test1(cv::Mat mArg, cv::Mat mWrapper, cv::Rect rRect);
public:
	cv::Mat* drawSquare(cv::Mat mColorFrameArg, std::vector<std::pair<cv::Point2f, cv::Point2f>>);
};

