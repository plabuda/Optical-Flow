#include "stdafx.h"
#include "mMinWindow.h"
#include "BGS.h"

using namespace std;
using namespace cv;

int main(void)
{
	Size winSize(15, 15), subPixWinSize(15, 15);
    //uzywane w mMinWindow
    //im mniejszy 3ci paramert tym wolniejsze działanie, a zmiana nie widoczna
    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 30, 0.3);
    Mat mFrame0, mFrame1, mFrame2, frame;
	Mat* result1;
	VideoCapture cap;
    cv::Rect win1;
    cap.open("file:///home/edek437/Coding/Studia/Optical-Flow/banan_ffmpeg.avi");

	if (!cap.isOpened()) {
		cout << "Cannot open the video file" << endl;
		return -1;
	}
	
	cap >> frame;

    win1 = cv::Rect(400, 200, 300, frame.rows - 200); //x,y,width, height
	mMinWindow mMinFrame0 = mMinWindow(win1, winSize, subPixWinSize, termcrit);
    //Do zbadania: 2 i 3 argument (history, varThreshold)
    BGS bgsFrame0 = BGS(win1, 300, 20, 0, win1.width, 300);

	while (true)
	{
		cap >> frame;

		if (frame.empty()) {
            //na koncy wyświetlam informache ile pojazdów zostało zareestrowanych i jakie miały rozmiary
            bgsFrame0.printVehicleInfo();
			fprintf(stderr, "End of video");
			return -1;
		}
		
		mFrame0 = mMinFrame0.drawVectors(frame);
		result1 = bgsFrame0.drawSquare(frame, mMinFrame0.getResultVector());
        mFrame1 = result1[0];
        mFrame2 = result1[1];

        //prostokąty
        imshow("Optical Flow", mFrame2);
        //kontury
        imshow("Optical Flow1", mFrame1);
        //vectory
        imshow("Optical Flow2", mFrame0);

        // sleep 1ms co klatkę
        // bez tego nic się nie wyswietla
        int key_pressed = waitKey(1);
        if (key_pressed == 'q') break;
			
	}

	return 0;
}
