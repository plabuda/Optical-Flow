#pragma once
#include "Vehicle.h"
#include <vector>

class BGS
{
private:
    std::pair<cv::Point, cv::Point> p_pLine;
    cv::Rect rRect;
	cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
    cv::Mat mColorFrame, mColorFrame1, mMask, mMaskG, ret[2];
    cv::Mat se1, se2;
	std::vector<cv::Vec4i> hierarchy;
    std::vector<Vehicle> vrPrevVehicles;
    std::vector<Vehicle> measuredVehiclesVehicles;

	
public:
    BGS() = default;
    BGS(cv::Rect, int history, float varThreshold, int iDetectLineX1, int iDetectLineX2, int iDetectLineY1);
    ~BGS() = default;

	double square(int a);
	
private:
	void Refactor(cv::Mat &mArg);
public:
    cv::Mat* drawSquare(cv::Mat const& mColorFrameArg, std::vector<std::pair<cv::Point2f, cv::Point2f>> const&);
    void printVehicleInfo();
};

