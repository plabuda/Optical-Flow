#include "stdafx.h"
#include "BGS.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace cv;
using namespace std;

BGS::BGS()
{
}


BGS::BGS(Rect rRectArg, int history, float varThreshold, int iDetectLineX1, int iDetectLineX2, int iDetectLineY){
	frame = 0;
	rRect = Rect(rRectArg);
    bool detectShadows = true;
    pMOG2 = createBackgroundSubtractorMOG2(history, varThreshold, detectShadows);
	pMOG2->setDetectShadows(true);
	pMOG2->setNMixtures(5);
    //pMOG2->setShadowThreshold(127);
	//pMOG2->setVarMin(200);
	pMOG2->setVarThresholdGen(10.1);
    se1 = getStructuringElement(MORPH_RECT, Point(5, 5));
    se2 = getStructuringElement(MORPH_RECT, Point(2, 2));
	mMaskG = getStructuringElement(MORPH_RECT, Point(5, 5));

	p_pLine.first = Point(iDetectLineX1, iDetectLineY);
	p_pLine.second = Point(iDetectLineX2, iDetectLineY);
}


BGS::~BGS()
{
}

inline double  BGS::square(int a)
{
	return a * a;
}

void BGS::Refactor(Mat &mArg)
{
    morphologyEx(mArg, mArg, MORPH_CLOSE, se1);
    morphologyEx(mArg, mArg, MORPH_OPEN, se2);
	erode(mArg, mArg, mMaskG);
	dilate(mArg, mArg, mMaskG);
	erode(mArg, mArg, mMaskG);
    imshow("refactor img", mArg);
}

cv::Mat* BGS::drawSquare(cv::Mat mColorFrameArg, vector<pair<cv::Point2f, cv::Point2f>> vp_p2fArgument)
{
	vector<int> ind;
	Mat tempmat, tempmat1;
	cv::Mat mFrame_Wrapper(
		cv::Size(rRect.width * 3 + 100,
			rRect.height),
		CV_8UC1);
	vrVehicles.clear();
	mColorFrameArg(rRect).copyTo(mColorFrame);
	mColorFrameArg(rRect).copyTo(mColorFrame1);
	pMOG2->apply(mColorFrame, mMask, 0.001);
    imshow("ad", mMask);
	cv::inRange(mMask, 200, 255, mMask);
    imshow("draw square img", mMask);
	Refactor(mMask);
	ret[0] = mMask;

	findContours(*(new Mat(mMask)) , vvpContours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
	line(mColorFrame, p_pLine.first, p_pLine.second, Scalar(0, 255, 0), 5, CV_AA, 0);
    for (auto itc = vvpContours.begin(); itc != vvpContours.end(); ++itc) {
		std::vector<cv::Point> pts = *itc;
		cv::Mat pointsMatrix = cv::Mat(pts);
		cv::Scalar color(255, 255, 255);

		cv::Rect r0 = cv::boundingRect(pointsMatrix);
					
		if (r0.area() > 4000 && r0.width < r0.height * 1.5) {
			Point2f temp = Point2f((r0.br() + r0.tl()) / 2);
			if (vrPrevVehicles.empty())
			{
				vrVehicles.push_back(Vehicle(r0));
			}
			if (Vehicle::counter >= 1000)
            {
				Vehicle::counter = 0;
            }
			if (!vrPrevVehicles.empty()) {
				bool flag = false;
                for (auto itcR = vrPrevVehicles.begin(); itcR != vrPrevVehicles.end(); ++itcR) {
					Vehicle tempr = *itcR;
					if (tempr.getDim().contains(temp)) {
						Point2f p2fCenter = (tempr.getDim().br() + tempr.getDim().tl()) / 2;

						

						if (r0.y + r0.height >= p_pLine.first.y && r0.y < p_pLine.first.y && r0.x > p_pLine.first.x && r0.x + r0.width < p_pLine.second.x) {
							cv::rectangle(mColorFrame, r0, cv::Scalar(255, 0, 0), 2);
							tempr.measure();
                            if ((std::find(ind.begin(), ind.end(), tempr.getID()) == ind.end())) {

                                ind.push_back(tempr.getID());
                                mColorFrame1(r0).copyTo(tempmat);
                                imshow("vechicles", tempmat);
                                // wyprintować wymiary pojazdu
                                std::cout << "Vehicle Id:" << tempr.getID() << std::endl;
                                std::cout << "Vehicle lenght:" << tempr.getLength() << std::endl;
                                std::cout << "Vehicle width:" << tempr.getWidth() << std::endl;
                            }
						}
						else
						{
							cv::rectangle(mColorFrame, r0, color, 2);
							if (tempr.getLength() != 0)
							{
								putText(mColorFrame, std::to_string(tempr.getWidth()), r0.tl(), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1, CV_AA, false);
								putText(mColorFrame, std::to_string(tempr.getLength()), (r0.tl() + Point(r0.width / 2, 0)), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1, CV_AA, false);
								tempr.countLength();
							}
						}
						std::vector<pair<cv::Point2f, cv::Point2f>>::iterator itcPP = vp_p2fArgument.begin();
						while (itcPP != vp_p2fArgument.end()) {
							pair<cv::Point2f, cv::Point2f> tempPP = *itcPP;
							if (r0.contains(tempPP.first)) {
								double hypotenuse = sqrt(square(tempPP.first.y - tempPP.second.y) + square(tempPP.first.x - tempPP.second.x));
								if (hypotenuse > 3 && hypotenuse < 15) {
									//putText(mColorFrame, std::to_string(hypotenuse), (r0.tl() + Point(r0.width / 2, 0)), FONT_HERSHEY_SIMPLEX, 0.5, color, 1, CV_AA, false);
									tempr.setSpeed(hypotenuse);
									break;
								}
							}
							itcPP++;
						}

						
						//vrVehicles.push_back(Vehicle(r0, tempr.getID()));
						vrVehicles.push_back(Vehicle(r0, tempr));
						line(mColorFrame, p2fCenter, temp, color, 5, CV_AA, 0);
						//putText(mColorFrame, std::to_string(tempr.getID()),r0.tl(), FONT_HERSHEY_SIMPLEX, 0.5, color, 1, CV_AA, false);
						

						if (tempr.getMeasured() && !tempmat.empty())
						{
							vector<int> compression_params;
							tempmat.copyTo(tempmat1);
							compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
							compression_params.push_back(9);
							putText(tempmat1, std::to_string(tempr.getLength()).substr(0, 5), Point(0, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, color, 1, CV_AA, false);
							putText(tempmat1, std::to_string(tempr.getWidth()).substr(0, 5), Point(tempmat.cols / 2, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, color, 1, CV_AA, false);
							imwrite("images/img" + std::to_string(tempr.getID()) + ".png", tempmat1, compression_params);
						}

					flag = true;
					break;
				}
			}

			

				if (!flag)			
					vrVehicles.push_back(Vehicle(r0));
			}
			
            //filotetowa kropka na obrazie optical flow
            cv::circle(mColorFrame, temp , 5, cv::Scalar(255, 0, 255), -1, 8, 0);

		}		
	
	}
	frame++;
	vrPrevVehicles.swap(vrVehicles);
	ret[1] = mColorFrame;
	mMask.deallocate();
	mColorFrame.deallocate();
	return ret;
}
