#pragma once

#include "targetver.h"


/*Optical FLow operands*/
#define NOF 100  //numbers of futures
#define QL 0.1   //quality level
#define MD 3	 //minimal distance
#define EBS 3    //eig_block_size
#define UH false	 //use harris
#define EV 0.004	 //use harris
static const double pi = 3.14159265358979323846;

#include <stdio.h>
#include <tchar.h>
#include <cv.h>
#include "opencv2/opencv.hpp"
#include <highgui.h>

#include "package_bgs\jmo\MultiLayerBGS.h"

