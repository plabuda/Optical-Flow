#pragma once
#include "Vehicle.h"
#include <vector>

class BGS
{
private:
	struct cmp {
		inline bool operator()(Vehicle &v1, Vehicle &v2)
		{
			if (v1.getID() == v2.getID())
				return v1.getDim().size().area() > v2.getDim().size().area();
			return v1.getID() < v2.getID();
		}
	};

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
	void Refactor(cv::Mat &mArg);
public:
	cv::Mat* drawSquare(cv::Mat mColorFrameArg, std::vector<std::pair<cv::Point2f, cv::Point2f>>);
};

