#include "stdafx.h"
#include "mMinWindow.h"

using namespace cv;
using namespace std;

mMinWindow::mMinWindow()
{
}

mMinWindow::mMinWindow(Rect rSizeArg, Size winSizev, Size subPixWinSizev, TermCriteria termcritv) {
	rRect = Rect(rSizeArg);
	winSize = winSizev;
	subPixWinSize = subPixWinSizev;
	termcrit = termcritv;
}


mMinWindow::~mMinWindow()
{
}

int mMinWindow::getWidth() const
{
	return rRect.width;
}

int mMinWindow::getHeigth() const
{
	return rRect.height;
}

vector<pair<cv::Point2f, cv::Point2f>> mMinWindow::getResultVector() const
{
	return resultVector;
}

inline double  mMinWindow::square(int a)
{
	return a * a;
}


cv::Mat mMinWindow::drawVectors(Mat frame)
{
	resultVector.clear();
	if (iRefreshCounter == 15) {
		corners[1].clear();
		corners[0].clear();
		iRefreshCounter = 0;
	}
	else {
		iRefreshCounter++;
	}

	frame(rRect).copyTo(mColorFrame);
	cvtColor(mColorFrame, mGrayFrame, cv::COLOR_BGR2GRAY);
	goodFeaturesToTrack(mGrayFrame, corners[1], NOF, QL, MD, Mat(), EBS, UH, EV);
	cornerSubPix(mGrayFrame, corners[1], subPixWinSize, Size(-1, -1), termcrit);

	if (mPrevGrayFrame.empty())
		mGrayFrame.copyTo(mPrevGrayFrame);

	if (!mPrevGrayFrame.empty() && !mGrayFrame.empty() && !corners[0].empty())
		calcOpticalFlowPyrLK(mPrevGrayFrame, mGrayFrame, corners[0], corners[1], status, err, winSize, 3, termcrit, 0, 0.0000001);

	if (!corners[0].empty())
		for (int i = 0; i < corners[1].size(); i++)
		{
			if (status[i] == 0) continue;
            int line_thickness = 2;

			CvScalar line_color; line_color = CV_RGB(255, 0, 0);
			CvPoint p, q;

			p.x = (int)corners[0][i].x;
			p.y = (int)corners[0][i].y;
			q.x = (int)corners[1][i].x;
			q.y = (int)corners[1][i].y;

			double angle = atan2((double)p.y - q.y, (double)p.x - q.x);
			double hypotenuse = sqrt(square(p.y - q.y) + square(p.x - q.x));
			circle(mColorFrame, corners[0][i], 5, Scalar(255, 0, 255), -1, 8, 0);

			if (hypotenuse > 3 && hypotenuse < 15) {
				resultVector.push_back(pair<Point2f, Point2f>(p, q));
				q.x = (int)(p.x - 3 * hypotenuse * cos(angle));
				q.y = (int)(p.y - 3 * hypotenuse * sin(angle));
                std::pair<cv::Point, cv::Point> p_pLine;
                //te czerwone vectorki na optical flow2
                arrowedLine(mColorFrame, p, q, line_color, line_thickness, CV_AA, 0, 0.3);
			}
		}

	corners[1].swap(corners[0]);
	swap(mPrevGrayFrame, mGrayFrame);
	mGrayFrame.deallocate();
	return mColorFrame;
}
