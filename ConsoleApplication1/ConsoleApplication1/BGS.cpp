#include "stdafx.h"
#include "BGS.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>>
using namespace cv;
using namespace std;

BGS::BGS()
{
}


BGS::BGS(Rect rRectArg, int history, float varThreshold, int iDetectLineX1, int iDetectLineX2, int iDetectLineY){
	frame = 0;
	rRect = Rect(rRectArg);
	pMOG2 = createBackgroundSubtractorMOG2(history, varThreshold);
	pMOG2->setDetectShadows(true);
	pMOG2->
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
	imshow("adsa", mArg);
}

cv::Mat* BGS::drawSquare(cv::Mat mColorFrameArg, vector<pair<cv::Point2f, cv::Point2f>> vp_p2fArgument)
{
	
	cv::Mat mFrame_Wrapper(
		cv::Size(rRect.width * 3 + 100,
			rRect.height),
		CV_8UC1);
	vrVehicles.clear();
	mColorFrameArg(rRect).copyTo(mColorFrame);
	mColorFrameArg(rRect).copyTo(mColorFrame1);
	pMOG2->apply(mColorFrame, mMask, 0.001);
	imshow("ad", mMask);
	Refactor(mMask);
	//test1(Mat(mMask), mFrame_Wrapper, rRect);
	ret[0] = mMask;

	findContours(*(new Mat(mMask)) , vvpContours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
	std::vector<std::vector<cv::Point>>::iterator itc = vvpContours.begin();
	line(mColorFrame, p_pLine.first, p_pLine.second, Scalar(0, 255, 0), 5, CV_AA, 0);
	while (itc != vvpContours.end()) {
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
				Vehicle::counter = 0;
			if (!vrPrevVehicles.empty()) {
				std::vector<Vehicle>::iterator itcR = vrPrevVehicles.begin();
				bool flag = false;
				while (itcR != vrPrevVehicles.end()) {
					Vehicle tempr = *itcR;
					if (tempr.getDim().contains(temp)) {
						Point2f p2fCenter = (tempr.getDim().br() + tempr.getDim().tl()) / 2;

						
						vrVehicles.push_back(Vehicle(r0, tempr.getID()));
						line(mColorFrame, p2fCenter, temp, color, 5, CV_AA, 0);
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
					vrVehicles.push_back(Vehicle(r0));
			}

			int *largestRect = new int[vrVehicles.size()];
			vector<Vehicle> tmpVehicle;

			/*std::sort(vrVehicles.begin(), vrVehicles.end(), cmp());
			tmpVehicle.push_back(vrVehicles[0]);
			for (int i = 1; i < vrVehicles.size() - 1; i++)
			{
				if (vrVehicles[i].getID() != vrVehicles[i + 1].getID())
					tmpVehicle.push_back(vrVehicles[i + 1]);
			}
			if (vrVehicles.size() > 1)
			{
				if (vrVehicles[vrVehicles.size() - 2].getID() != vrVehicles[vrVehicles.size() - 1].getID())
				{
					tmpVehicle.push_back(vrVehicles[vrVehicles.size() - 1]);
				}
				vrVehicles = tmpVehicle;
			}*/

			/*for (int i = 0; i < vrVehicles.size(); i++)
			{
				if (vrVehicles[i].getDim().width * 2 < vrVehicles[i].getDim().height)
					tmpVehicle.push_back(vrVehicles[i]);
			}
			vrVehicles = tmpVehicle;*/

			cv::circle(mColorFrame, temp , 5, cv::Scalar(255, 0, 255), -1, 8, 0);

			if (r0.y + r0.height >= p_pLine.first.y && r0.y < p_pLine.first.y && r0.x > p_pLine.first.x && r0.x + r0.width < p_pLine.second.x) {
				cv::rectangle(mColorFrame, r0, cv::Scalar(255, 0, 0), 2);
			}	
			else
				cv::rectangle(mColorFrame, r0, color, 2);
		}		
	
		++itc;
	}
	//std::ofstream f("frames/somefile" + to_string(frame) + ".txt");
	//for (vector<Vehicle>::iterator i = vrVehicles.begin(); i != vrVehicles.end(); ++i) {
//		f << i->getID() << " " << i->getDim().size() << '\n';
//	}
	frame++;
	vrPrevVehicles.swap(vrVehicles);
	ret[1] = mColorFrame;
	mMask.deallocate();
	mColorFrame.deallocate();
	return ret;
}

//struct BGS::cmp {
//	inline bool operator()(Vehicle &v1, Vehicle &v2)
//	{
//		if (v1.getID() == v2.getID())
//			return v1.getDim().size().area() > v2.getDim().size().area();
//		return v1.getID() < v2.getID();
//	}
//};
