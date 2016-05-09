
#include "BGS.h"
#include "Config.h"
#include <cv.h>
#include "opencv2/opencv.hpp"
#include <highgui.h>
#include "package_bgs\jmo\MultiLayerBGS.h"

BGS::BGS()
{
}

BGS::BGS(int xv, int yv , int wv, int hv)
{
	x = xv;
	y = yv;
	w = wv;
	h = hv;
	rRect = cv::Rect(xv, yv, wv, hv);

	ibgsTest = new MultiLayerBGS;
}


BGS::~BGS()
{
	delete ibgsTest;
}

cv::Mat BGS::execute(cv::Mat frame)
{
	mColorFrame = frame(rRect);
	ibgsTest->process(mColorFrame, img_mask, img_bkgmodel);
	return img_mask;
}
