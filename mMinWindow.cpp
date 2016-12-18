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


//wszystko co się tu dzieje jest odwzorowywane na obrazie Optical Flow2
cv::Mat mMinWindow::drawVectors(Mat frame)
{
    cv::Mat mGrayFrame;
	resultVector.clear();

    //bez tego optical flow vektorki sie nie rysuja
    if (iRefreshCounter == MAX_REFRESH_COUNTER) {
		corners[1].clear();
		corners[0].clear();
		iRefreshCounter = 0;
	}
    else
    {
		iRefreshCounter++;
	}

	frame(rRect).copyTo(mColorFrame);
    cvtColor(mColorFrame, mGrayFrame, cv::COLOR_BGR2GRAY);
    //pomanipulować tymi definami z stdafx.h
    //generuje vectory opticl flow na obrazie OpticalFlow2
    goodFeaturesToTrack(mGrayFrame, corners[1], NOF, QL, MD, Mat(), EBS, UH, EV);
    //usuwa anomalie pojawiania się
    cornerSubPix(mGrayFrame, corners[1], subPixWinSize, Size(-1, -1), termcrit);

    //zeby się nie zepsuło na swapowaniu dla pierwszego wywołania
	if (mPrevGrayFrame.empty())
    {
		mGrayFrame.copyTo(mPrevGrayFrame);
    }

	if (!mPrevGrayFrame.empty() && !mGrayFrame.empty() && !corners[0].empty())
    {
        //dwoma ostatnimi parametrami się pobawić
        calcOpticalFlowPyrLK(mPrevGrayFrame, mGrayFrame, corners[0], corners[1], status, err, winSize, 3, termcrit, 0, 0.0000001);
    }

	if (!corners[0].empty())
		for (int i = 0; i < corners[1].size(); i++)
		{
            //sprawdza czy flow zostal wykryty (0 oznacza, że nie został)
            if (status[i] == 0)
            {
                continue;
            }
			CvPoint p, q;

			p.x = (int)corners[0][i].x;
			p.y = (int)corners[0][i].y;
			q.x = (int)corners[1][i].x;
			q.y = (int)corners[1][i].y;

            double hypotenuse = sqrt(square(p.y - q.y) + square(p.x - q.x));
            // te filotetowe kropki na optical flow 2
            circle(mColorFrame, corners[0][i], 5, Scalar(255, 0, 255), -1, 8, 0);

            //usunięcie tego ifa nic nie zmienia?? Jakie jest jego zadanie?
            if (hypotenuse > 3 && hypotenuse < 15) {
                resultVector.emplace_back(pair<Point2f, Point2f>(p, q));
                //wyznaczenie końcó czerownych wektorków
                double angle = atan2((double)p.y - q.y, (double)p.x - q.x);
				q.x = (int)(p.x - 3 * hypotenuse * cos(angle));
				q.y = (int)(p.y - 3 * hypotenuse * sin(angle));
                //te czerwone vectorki na optical flow2
                CvScalar line_color = CV_RGB(255, 0, 0);
                int line_thickness = 2;
                arrowedLine(mColorFrame, p, q, line_color, line_thickness, CV_AA, 0, 0.3);
            }
		}

	corners[1].swap(corners[0]);
    std::swap(mPrevGrayFrame, mGrayFrame);
	return mColorFrame;
}
