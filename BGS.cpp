#include "stdafx.h"
#include "BGS.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace cv;
using namespace std;

Mat sobel(Mat gray){
    Mat edges;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    Mat edges_x, edges_y;
    Mat abs_edges_x, abs_edges_y;
    Sobel(gray, edges_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs( edges_x, abs_edges_x );
    Sobel(gray, edges_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(edges_y, abs_edges_y);
    addWeighted(abs_edges_x, 0.5, abs_edges_y, 0.5, 0, edges);
    threshold(edges, edges, 240, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    return edges;
}

BGS::BGS(Rect rRectArg, int history, float varThreshold, int iDetectLineX1, int iDetectLineX2, int iDetectLineY){
	rRect = Rect(rRectArg);
    bool detectShadows = false;
    pMOG2 = createBackgroundSubtractorMOG2(history, varThreshold, detectShadows);
	pMOG2->setNMixtures(5);
//    pMOG2->setShadowThreshold(127);
    pMOG2->setVarMin(200);
    pMOG2->setVarThresholdGen(10.1);
    se1 = getStructuringElement(MORPH_RECT, Point(10, 10));
    se2 = getStructuringElement(MORPH_RECT, Point(2, 2));
    mMaskG = getStructuringElement(MORPH_RECT, Point(5, 5));

    //wyznaczanie połozenia zielonej linii
	p_pLine.first = Point(iDetectLineX1, iDetectLineY);
	p_pLine.second = Point(iDetectLineX2, iDetectLineY);
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
    imshow("refactor img", mArg);
}

//rysowanie kwadratów wokół samochodów
cv::Mat* BGS::drawSquare(cv::Mat const& mColorFrameArg, std::vector<pair<cv::Point2f, cv::Point2f>> const& vp_p2fArgument)
{
    std::vector<int> vehicle_ids;
    cv::Mat tempmat;
    std::vector<Vehicle> vrVehicles;
    std::vector<std::vector<cv::Point>> vvpContours;
	mColorFrameArg(rRect).copyTo(mColorFrame);
	mColorFrameArg(rRect).copyTo(mColorFrame1);
    // 3 argument odpowiada za częstotliwość odświeżania tła
//    pMOG2->apply(mColorFrame, mMask, 0.001);
    //to nic nie robi bo maska i tak jest czarno biała
    //cv::inRange(mMask, 200, 255, mMask);
//    imshow("draw square img", mMask);
    //Sobel
    cv::Mat grayFrame;
    cvtColor(mColorFrame, grayFrame , cv::COLOR_BGR2GRAY);
    Mat detected_edges = sobel(grayFrame);
    //canny egde detection
//    pMOG2->apply(mColorFrame, mMask, 0.001);
//    Mat detected_edges;
//    Canny(mMask, detected_edges, 1000, 1400, 3);
//    erode(detected_edges, detected_edges, getStructuringElement(MORPH_RECT, Point(1, 1)));
//    dilate(detected_edges, detected_edges, getStructuringElement(MORPH_RECT, Point(3, 3)));
//    erode(detected_edges, detected_edges, getStructuringElement(MORPH_RECT, Point(1, 1)));
//    imshow("3. Edge Detector", detected_edges);
//    Refactor(mMask);

    //znajduje kontury samochodów widoczne na Optical Flow1 i zapisuje je do vvpContours jako wektory punktów
    //hierarchy to arraja wyjściowa ale nigdy nie używana
    // do przebadania: CV_RETR_XXX, CHAIN_APPROX_XXX. Ta funkcje też zmienia wartość ret[0]
    findContours(*(new Mat(detected_edges)) , vvpContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    ret[0] = detected_edges;
//    dilate(detected_edges, detected_edges, getStructuringElement(MORPH_RECT, Point(1, 1)));

    // ta zielona linia na opt flow
    line(mColorFrame, p_pLine.first, p_pLine.second, Scalar(0, 255, 0), 5, CV_AA, 0);
    //dla wszystkich kanturów pojazdów
    for (auto itc = vvpContours.begin(); itc != vvpContours.end(); ++itc) {
        cv::Scalar redColor(0, 0, 255);

        //prostokat z pojazdem
        cv::Rect r0 = cv::boundingRect(cv::Mat(*itc));
        //patrzy czy prostokąt obejmujący pojaz jest odpowiednio wielki, i patrzy na proporcje długość szerokość
        if (r0.area() > 4000 && r0.width < r0.height * 1.5) {
            //środek pojazdu
            Point2f temp = Point2f((r0.br() + r0.tl()) / 2);
            //pod koniec ten wektor jest zaminaiiany z wektorem pojazdów znalezionych w tej pętli stąd ta ifowa magia
            if (vrPrevVehicles.empty())
            {
                vrVehicles.emplace_back(Vehicle(r0));
            }
            else
            {
				bool flag = false;
                //sprawdź czy pojazd już zareejstrowano
                for (auto itcR = vrPrevVehicles.begin(); itcR != vrPrevVehicles.end(); ++itcR) {
					Vehicle tempr = *itcR;
					if (tempr.getDim().contains(temp)) {
						Point2f p2fCenter = (tempr.getDim().br() + tempr.getDim().tl()) / 2;

						if (r0.y + r0.height >= p_pLine.first.y && r0.y < p_pLine.first.y && r0.x > p_pLine.first.x && r0.x + r0.width < p_pLine.second.x) {
							cv::rectangle(mColorFrame, r0, cv::Scalar(255, 0, 0), 2);
                            //zakładamy że w każdej klatce, w jakiej pojawił się pojazd, przesunął się o jeden piksel? Trochę to słabe
							tempr.measure();
                            //wygląda na to, że pomiar jednego pojazdu może odbyć się dwuktrotnie, ale brany jest tylko pierwszy wynik
                            if ((std::find(vehicle_ids.begin(), vehicle_ids.end(), tempr.getID()) == vehicle_ids.end())) {
                                vehicle_ids.push_back(tempr.getID());
                                mColorFrame1(r0).copyTo(tempmat);
                                imshow("vechicles", tempmat);
                            }
						}
						else
						{
                            cv::rectangle(mColorFrame, r0, redColor, 2);
                            //jeśli był już raz zmierzony (długość nie zerowa), to wyrysuj na filmie aktualne wymiaru (to jest mało widoczne)
							if (tempr.getLength() != 0)
							{
								putText(mColorFrame, std::to_string(tempr.getWidth()), r0.tl(), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1, CV_AA, false);
								putText(mColorFrame, std::to_string(tempr.getLength()), (r0.tl() + Point(r0.width / 2, 0)), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1, CV_AA, false);
								tempr.countLength();
                                auto measuredVehicleIt = std::find_if(measuredVehiclesVehicles.begin(),
                                                                   measuredVehiclesVehicles.end(),
                                                                   [&tempr](Vehicle const& veh) -> bool {return tempr.getID() == veh.getID();}
                                                         );
                                if (measuredVehicleIt == measuredVehiclesVehicles.end())
                                {
                                    measuredVehiclesVehicles.push_back(tempr);
                                }
                                else
                                {
                                    *measuredVehicleIt = tempr;
                                }
							}
						}
                        //vectorki otrzymane z optical flowa służa do oblicznia prędkości pojazdów w danym momencie
                        //co jak prędkość się zmienia? Albo samochody bedą stły n. w korku?
                        for (auto itcPP = vp_p2fArgument.begin(); itcPP != vp_p2fArgument.end(); ++itcPP) {
							pair<cv::Point2f, cv::Point2f> tempPP = *itcPP;
							if (r0.contains(tempPP.first)) {
								double hypotenuse = sqrt(square(tempPP.first.y - tempPP.second.y) + square(tempPP.first.x - tempPP.second.x));
                                //znowu te magiczne widełki długość wektorka z optical flow
								if (hypotenuse > 3 && hypotenuse < 15) {
									tempr.setSpeed(hypotenuse);
									break;
								}
							}
						}

                        vrVehicles.emplace_back(Vehicle(r0, tempr));
                        //te proste linie łączące fioletowe czasem punkty prostymi na filmie. Skąd one się biorą?
                        line(mColorFrame, p2fCenter, temp, redColor, 5, CV_AA, 0);
                        // zmierzone pojazdy wykadroawuj i zapisuje do pliku z szerokościa i długością pojazdu
                        if (tempr.isMeasured() && !tempmat.empty())
						{
							vector<int> compression_params;
                            cv::Mat tempmat1;
							tempmat.copyTo(tempmat1);
							compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
							compression_params.push_back(9);
                            putText(tempmat1, "L :" + std::to_string(tempr.getLength()).substr(0, 5), Point(0, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
                            putText(tempmat1, "W: " + std::to_string(tempr.getWidth()).substr(0, 5), Point(tempmat.cols / 2, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
                            imwrite("/home/edek437/Coding/Studia/Optical-Flow/images/img" + std::to_string(tempr.getID()) + ".png", tempmat1, compression_params);
						}

                        flag = true;
                        break;
                    }
                }

                //pod koniec ten wektor jest zaminaiiany z wektorem pojazdów znalezionych w tej pętli stąd ta ifowa magia
                if (!flag)
                {
                    vrVehicles.emplace_back(Vehicle(r0));
                }
            }
			
            //filotetowa kropka na obrazie optical flow
            cv::circle(mColorFrame, temp , 5, cv::Scalar(255, 0, 255), -1, 8, 0);

		}		
	
	}
	vrPrevVehicles.swap(vrVehicles);
	ret[1] = mColorFrame;
	mMask.deallocate();
	mColorFrame.deallocate();
	return ret;
}

void BGS::printVehicleInfo() {
    std::cout << "VEHICLES FOUND:" << std::endl;
    for (auto it = measuredVehiclesVehicles.begin(); it != measuredVehiclesVehicles.end(); ++it)
    {
        // wyprintować wymiary pojazdów
        std::cout << "Vehicle Id:" << it->getID() << std::endl;
        std::cout << "Vehicle lenght:" << it->getLength() << std::endl;
        std::cout << "Vehicle width:" << it->getWidth() << std::endl;
    }
}

