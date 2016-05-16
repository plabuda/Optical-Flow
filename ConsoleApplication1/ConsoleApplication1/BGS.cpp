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
	mColorFrameArg(rRect).copyTo(mColorFrame);
	pMOG2->apply(mColorFrame, mMask, 0.001);
	findContours(mMask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();

	while (itc != contours.end()) {
			std::vector<cv::Point> pts = *itc;
			cv::Mat pointsMatrix = cv::Mat(pts);
			cv::Scalar color(255, 255, 255);

			cv::Rect r0 = cv::boundingRect(pointsMatrix);
			if (r0.area() > 4000)
				cv::rectangle(mColorFrame, r0, color, 2);

			++itc;

	}
	Mat* ret = new Mat[2];
	ret[0] = mMask;
	ret[1] = mColorFrame;
	return ret;
}
