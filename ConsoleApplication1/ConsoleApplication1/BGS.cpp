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

cv::Mat* BGS::drawSquare(cv::Mat mColorFrameArg)
{
	vrRects.clear();
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
				vrRects.push_back(r0);
				if (!vrPrevRects.empty()) {
					std::vector<cv::Rect>::iterator itcR = vrPrevRects.begin();
					while (itcR != vrPrevRects.end()) {
						cv::Rect tempr = *itcR;
						if (tempr.contains(temp)) {
							line(mColorFrame, Point2f((tempr.br() + tempr.tl()) / 2) , temp, color, 5, CV_AA, 0);
						}
						itcR++;
					}
				}
				circle(mColorFrame, temp , 5, Scalar(255, 0, 255), -1, 8, 0);
				cv::rectangle(mColorFrame, r0, color, 2);
			}		

			++itc;
	}
	vrPrevRects.swap(vrRects);
	ret[1] = mColorFrame;
	mMask.deallocate();
	mColorFrame.deallocate();
	return ret;
}
