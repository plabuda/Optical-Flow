#include "stdafx.h"
#include <string>
#include "mMinWindow.h"
#include "BGS.h"
#include <vector>
using namespace std;
using namespace cv;

#include <libgen.h>
#include <unistd.h>

int main ( int argc, char **argv )
{

    cv::Point3d coeffs;
    coeffs.x = coeffs.y = coeffs.z = 1;
    if(argc > 2) coeffs.x = stod(argv[2]);
    if(argc > 3) coeffs.y = stod(argv[3]);
    if(argc > 4) coeffs.z = stod(argv[4]);

    std::cout << coeffs.x << " " << coeffs.y << " " << coeffs.z << std::endl;

	Size winSize(15, 15), subPixWinSize(15, 15);
    //uzywane w mMinWindow
    //im mniejszy 3ci paramert tym wolniejsze działanie, a zmiana nie widoczna
    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 30, 0.1);
    Mat mFrame0, mFrame1, mFrame2, frame;
	Mat* result1;
	VideoCapture cap;
    cv::Rect win1;

    char buf[1024] = {0};
    ssize_t size = readlink("/proc/self/exe", buf, sizeof(buf));
    if (size == 0 || size == sizeof(buf))
    {
        return 2;
    }
    std::string path(dirname(buf));
    std::cout << path << std::endl;

   // cap.open("file://" + path + "/banan_ffmpeg.avi");
if(argc > 1) cap.open("file://" + path + "/" + argv[1]);
else cap.open("file://" + path + "/Braniewo1.mp4");

	if (!cap.isOpened()) {
		cout << "Can not open the video file" << endl;
		return -1;
	}

	cap >> frame;

    win1 = cv::Rect(0, 0, frame.cols, frame.rows); //x,y,width, height
	mMinWindow mMinFrame0 = mMinWindow(win1, winSize, subPixWinSize, termcrit);
    //Do zbadania: 2 i 3 argument (history, varThreshold)
    BGS bgsFrame0 = BGS(win1, 300, 60, 200, 260, 300);

	while (true)
	{
		cap >> frame;

		if (frame.empty()) {
            //na koncy wyświetlam informache ile pojazdów zostało zareestrowanych i jakie miały rozmiary
            bgsFrame0.printVehicleInfo(coeffs);
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
