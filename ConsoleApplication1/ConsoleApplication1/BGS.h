#pragma once
#include "BGSProduct.h"
#include <vector>

class BGS
{
private:
	int x;
	int y;
	int w;
	int h;

	cv::RNG rng;
	cv::Rect rRect, rCntRect;
	cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
	cv::Mat  mColorFrame, mMask, mThresholdOutput, mMaskG, ret[2];

	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>> vvpContours;
	std::vector<cv::Rect> vrRects, vrPrevRects;
	
public:
	BGS();
	BGS::BGS(int xv, int yv, int wv, int hv, int history, float varThreshold, bool bShadowDetection);
	~BGS();

private:

public:
	cv::Mat* drawSquare(cv::Mat mColorFrameArg);
};

