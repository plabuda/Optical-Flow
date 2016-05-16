#include "stdafx.h"
#include "BGS.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

BGS::BGS()
{
}


BGS::BGS(int xv, int yv, int wv, int hv, int history, float varThreshold, bool bShadowDetection){
	x = xv;
	y = yv;
	w = wv;
	h = hv;
	rRect = Rect(xv, yv, wv, hv);
	pMOG2 = createBackgroundSubtractorMOG2(history, varThreshold, bShadowDetection);
	pMOG2->setNMixtures(1);
}


BGS::~BGS()
{
}

inline double  BGS::square(int a)
{
	return a * a;
}

cv::Mat* BGS::drawSquare(cv::Mat mColorFrameArg, vector<pair<cv::Point2f, cv::Point2f>> vp_p2fArgument)
{
	vrVehicles.clear();
	mColorFrameArg(rRect).copyTo(mColorFrame);
	pMOG2->apply(mColorFrame, mMask, 0.001);
	ret[0] = mMask;
	findContours(*(new Mat(mMask)) , vvpContours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	std::vector<std::vector<cv::Point>>::iterator itc = vvpContours.begin();

	while (itc != vvpContours.end()) {
			std::vector<cv::Point> pts = *itc;
			cv::Mat pointsMatrix = cv::Mat(pts);
			cv::Scalar color(255, 255, 255);

			cv::Rect r0 = cv::boundingRect(pointsMatrix);
						
			if (r0.area() > 4000) {
				Point2f temp = Point2f((r0.br() + r0.tl()) / 2);
				if (vrPrevVehicles.empty())
				{
					vrVehicles.push_back(Vehicle(r0));
				}
				if (Vehicle::counter >= 1000)
					Vehicle::counter = 0;
				if (!vrPrevVehicles.empty()) {
					std::vector<Vehicle>::iterator itcR = vrPrevVehicles.begin();
					bool flag = false;
					while (itcR != vrPrevVehicles.end()) {
						Vehicle tempr = *itcR;
						if (tempr.getDim().contains(temp)) {
							vrVehicles.push_back(Vehicle(r0, tempr.getID()));
							line(mColorFrame, Point2f((tempr.getDim().br() + tempr.getDim().tl()) / 2) , temp, color, 5, CV_AA, 0);
							putText(mColorFrame, std::to_string(tempr.getID()),r0.tl(), FONT_HERSHEY_SIMPLEX, 0.5, color, 1, CV_AA, false);
							std::vector<pair<cv::Point2f, cv::Point2f>>::iterator itcPP = vp_p2fArgument.begin();
							while (itcPP != vp_p2fArgument.end()) {
								pair<cv::Point2f, cv::Point2f> tempPP = *itcPP;
								if (r0.contains(tempPP.first)) {
									double hypotenuse = sqrt(square(tempPP.first.y - tempPP.second.y) + square(tempPP.first.x - tempPP.second.x));
									if (hypotenuse > 3 && hypotenuse < 15) {
										putText(mColorFrame, std::to_string(hypotenuse), (r0.tl() + Point(r0.width/2, 0)), FONT_HERSHEY_SIMPLEX, 0.5, color, 1, CV_AA, false);
										break;
									}
								}
								itcPP++;
							}
							flag = true;
							break;
						}
						itcR++;
					}

					if (!flag)
					{
						vrVehicles.push_back(Vehicle(r0));
					}
				}
				circle(mColorFrame, temp , 5, Scalar(255, 0, 255), -1, 8, 0);
				cv::rectangle(mColorFrame, r0, color, 2);
			}		

			++itc;
	}
	vrPrevVehicles.swap(vrVehicles);
	ret[1] = mColorFrame;
	mMask.deallocate();
	mColorFrame.deallocate();
	return ret;
}
