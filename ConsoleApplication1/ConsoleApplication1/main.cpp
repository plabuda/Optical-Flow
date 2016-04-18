

#include "stdafx.h"
#include <cv.h>
#include "opencv2/opencv.hpp"
#include <highgui.h>
#include <math.h>

#define NOF 500 //numbers of futures
#define QL 0.1   //quality level
#define MD 10    //minimal distance
#define EBS 3    //eig_block_size
#define UH 0	 //use harris
using namespace std;
using namespace cv;

static const double pi = 3.14159265358979323846;
inline static double square(int a)
{
	return a * a;
}


int main(void)
{

	//Obliczamy ilosc klatek w filmie
	long number_of_frames = 0, current_frame = 0;
	vector<Point2f> corners[2];
	bool needToInit = true;
	Mat framet;
	Size winSize(31, 31);
	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
	VideoCapture cap;

	cap.open("Highway.avi");

	while (true) {
		cap >> framet;
		if (framet.empty()) {
			break;
		}
		number_of_frames++;
	}

	cap.open("Highway.avi");

	if (!cap.isOpened()) {
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	namedWindow("Optical Flow", CV_WINDOW_AUTOSIZE);


	while (true)
	{
		static Mat frame, prevgrayframe, grayframe, frame1;

		cap >> frame;

		if (frame.empty()) {
			fprintf(stderr, "Error: Hmm. The end came sooner than we thought.\n");
			return -1;
		}


		frame.copyTo(frame1);

		int number_of_features = NOF;
		cvtColor(frame, grayframe, CV_BGR2GRAY);
		goodFeaturesToTrack(grayframe, corners[1], NOF, QL, MD, cv::noArray(), EBS, UH);
		if (prevgrayframe.empty())
			grayframe.copyTo(prevgrayframe);
		vector<uchar> status;
		vector<float> err;
		if (!prevgrayframe.empty() && !grayframe.empty() && !corners[0].empty())
			calcOpticalFlowPyrLK(prevgrayframe, grayframe, corners[0], corners[1], status, err, winSize, 3, termcrit, 0, 0.001);

		if (!corners[0].empty())
			/* For fun (and debugging :)), let's draw the flow field. */
			for (int i = 0; i < number_of_features; i++)
			{
				/* If Pyramidal Lucas Kanade didn't really find the feature, skip it. */
				int line_thickness; line_thickness = 2;
				/* CV_RGB(red, green, blue) is the red, green, and blue components
				* of the color you want, each out of 255.
				*/
				CvScalar line_color; line_color = CV_RGB(255, 0, 0);
				/* Let's make the flow field look nice with arrows. */
				/* The arrows will be a bit too short for a nice visualization because of the
				high framerate
				* (ie: there's not much motion between the frames). So let's lengthen them
				by a factor of 3.
				*/
				CvPoint p, q;
				p.x = (int)corners[0][i].x;
				p.y = (int)corners[0][i].y;
				q.x = (int)corners[1][i].x;
				q.y = (int)corners[1][i].y;
				double angle; angle = atan2((double)p.y - q.y, (double)p.x - q.x);
				double hypotenuse; hypotenuse = sqrt(square(p.y - q.y) + square(p.x - q.x));
				if (hypotenuse > 3 && hypotenuse < 15) {
					/* Here we lengthen the arrow by a factor of three. */
					q.x = (int)(p.x - 3 * hypotenuse * cos(angle));
					q.y = (int)(p.y - 3 * hypotenuse * sin(angle));
					/* Now we draw the main line of the arrow. */
					/* "frame1" is the frame to draw on.
					* "p" is the point where the line begins.
					* "q" is the point where the line stops.
					* "CV_AA" means antialiased drawing.
					* "0" means no fractional bits in the center cooridinate or radius.
					*/
					line(frame1, p, q, line_color, line_thickness, CV_AA, 0);
					/* Now draw the tips of the arrow. I do some scaling so that the
					* tips look proportional to the main line of the arrow.
					*/
					p.x = (int)(q.x + 9 * cos(angle + pi / 4));
					p.y = (int)(q.y + 9 * sin(angle + pi / 4));
					line(frame1, p, q, line_color, line_thickness, CV_AA, 0);
					p.x = (int)(q.x + 9 * cos(angle - pi / 4));
					p.y = (int)(q.y + 9 * sin(angle - pi / 4));
					line(frame1, p, q, line_color, line_thickness, CV_AA, 0);
				}
			}
		/* Now display the image we drew on. Recall that "Optical Flow" is the name of
		* the window we created above.
		*/
		imshow("Optical Flow", frame1);
		/* And wait for the user to press a key (so the user has time to look at the
		image).
		* If the argument is 0 then it waits forever otherwise it waits that number of
		milliseconds.
		* The return value is the key the user pressed.
		*/
		int key_pressed;
		key_pressed = waitKey(1);
		if (key_pressed == 'q') {
			break;
		}
		/* If the users pushes "b" or "B" go back one frame.
		* Otherwise go forward one frame.
		*/
		current_frame++;
		/* Don't run past the front/end of the AVI. */
		if (current_frame < 0) current_frame = 0;
		if (current_frame >= number_of_frames - 1) current_frame = 0;
		swap(corners[1], corners[0]);
	}
}