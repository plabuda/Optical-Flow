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
#include <cmath>
using namespace cv;
using namespace std;

BGS::BGS(Rect rRectArg, int history, float varThreshold, int iDetectLineX1, int iDetectLineX2, int iDetectLineY)
{
    rRect = Rect(rRectArg);
    bool detectShadows = false;
    pMOG2 = createBackgroundSubtractorMOG2(history, varThreshold, detectShadows);
    pMOG2->setNMixtures(5);
//    pMOG2->setShadowThreshold(127);
    pMOG2->setVarMin(200);
    pMOG2->setVarThresholdGen(10.1);
    se1 = getStructuringElement(MORPH_RECT, Point(5, 5));
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
    pMOG2->apply(mColorFrame, mMask, 0.001);
    cv::inRange(mMask, 200, 255, mMask);
    imshow("draw square img", mMask);
    Refactor(mMask);
    ret[0] = mMask;

    //znajduje kontury samochodów widoczne na Optical Flow1 i zapisuje je do vvpContours jako wektory punktów
    //hierarchy to arraja wyjściowa ale nigdy nie używana
    // do przebadania: CV_RETR_XXX, CHAIN_APPROX_XXX
    findContours(*(new Mat(mMask)) , vvpContours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
    // ta zielona linia na opt flow
    line(mColorFrame, p_pLine.first, p_pLine.second, Scalar(0, 255, 0), 5, CV_AA, 0);
    //dla wszystkich kanturów pojazdów
    for (auto itc = vvpContours.begin(); itc != vvpContours.end(); ++itc)
    {
        cv::Scalar redColor(0, 0, 255);

        //prostokat z pojazdem
        cv::Rect r0 = cv::boundingRect(cv::Mat(*itc));


        //patrzy czy prostokąt obejmujący pojaz jest odpowiednio wielki, i patrzy na proporcje długość szerokość
        if (r0.area() > 3600 && r0.width < r0.height * 1.5)
        {
            //środek pojazdu
            Point2f temp = Point2f((r0.br() + r0.tl()) / 2);


            cv::Point A, B, C, D, E, F;
            Point3d dimensions;
            //konstrukcja pudełka
            //wzór prostej nieprostopadłej - y = ax + b
            //obliczamy a
            double a = -0.5;
            //dla danego punktu x, y znajdujemy b rownaniem b = y - ax
            // znajdujemy b srodka
            double h0 = temp.y - a * temp.x;
            //proste górna i dolna zadane są równaniamy y = ax + h0 + hg oraz y = ax + h0 + hd
            //wyznaczamy początkowe, maksymalnie złe wartości
            double hg = 0.75 * abs(a) * r0.height;
            double hd = -0.75 * abs(a) * r0.height;
            //magiczny numerek 0.75 to półtora połowy rozmiaru pudełka
            //w sumie nie można by wystartować od zera? Do sprawdzenia

            //iteruj po konturach

            if(true)//if (r0.y + r0.height >= p_pLine.first.y && r0.y < p_pLine.first.y && r0.x > p_pLine.first.x && r0.x + r0.width < p_pLine.second.x)
            {
                for (auto itc2 = itc->begin(); itc2 != itc->end(); ++itc2)
                {

                    // jeśli punkt konturu należy do r0 policz wysokość linii przez niego przechodzącej
                    if( r0.contains(*itc2))
                    {
                        double h_new = itc2->y - a * itc2->x;
                        h_new = h0 - h_new;
                        // poprawianie hg i hd
                        if( h_new < hg) hg = h_new;
                        if( h_new > hd) hd = h_new;
                    }


                }
                //Obliczanie punktów
                // przeciecie y = ax + b z osia y = n  -> x = (n - b)/a
                //przeciecie z y = ax + b z osia x = n -> y = an + b
                if(a < 0 )
                {
                A = cv::Point((r0.y - h0 - hg)/a, r0.y);
                B = cv::Point(r0.x + r0.width, r0.y);
                C = cv::Point(r0.x + r0.width, a * (r0.x + r0.width) + h0 + hd);
                D = cv::Point((r0.y + r0.height - h0 - hd)/a, r0.y + r0.height);
                E = cv::Point(r0.x, r0.y + r0.height);
                F = cv::Point(r0.x, a * (r0.x) + h0 + hg);

                cv::line(mColorFrame, A, B,  cv::Scalar(255,0,0), 2);
                cv::line(mColorFrame, B, C,  cv::Scalar(0,255,0), 2);
                cv::line(mColorFrame, C, D,  cv::Scalar(0,0,255), 2);
                cv::line(mColorFrame, D, E,  cv::Scalar(255,0,0), 2);
                cv::line(mColorFrame, E, F,  cv::Scalar(0,255,0), 2);
                cv::line(mColorFrame, F, A,  cv::Scalar(0,0,255), 2);
                dimensions.x = (cv::norm(F-A) + cv::norm(C-D))/2; //oś x - wyznaczona ze współczynnika a - długość auta
                dimensions.y = (cv::norm(E-D) + cv::norm(A-B))/2; //oś y - nie oś x i nie oś pionowa - szerokość auta
                dimensions.z = (cv::norm(F-E) + cv::norm(C-B))/2; //oś z - oś pionowa - wysokość auta


                }else
                {
                A = cv::Point(r0.x, r0.y);
                B = cv::Point((r0.y - h0 - hg)/a, r0.y);
                C = cv::Point(r0.x + r0.width, a * (r0.x + r0.width) + h0 + hg);
                D = cv::Point(r0.x + r0.width, r0.y + r0.height);
                E = cv::Point((r0.y + r0.height - h0 - hd)/a, r0.y + r0.height);
                F = cv::Point(r0.x, a * (r0.x) + h0 + hd);

                cv::line(mColorFrame, A, B,  cv::Scalar(255,0,0), 2);
                cv::line(mColorFrame, B, C,  cv::Scalar(0,0,255), 2);
                cv::line(mColorFrame, C, D,  cv::Scalar(0,255,0), 2);
                cv::line(mColorFrame, D, E,  cv::Scalar(255,0,0), 2);
                cv::line(mColorFrame, E, F,  cv::Scalar(0,0,255), 2);
                cv::line(mColorFrame, F, A,  cv::Scalar(0,255,0), 2);


                dimensions.x = (cv::norm(F-E) + cv::norm(C-B))/2;
                dimensions.y = (cv::norm(E-D) + cv::norm(A-B))/2;
                dimensions.z = (cv::norm(F-A) + cv::norm(C-D))/2;
                }
            }





//pod koniec ten wektor jest zaminaiiany z wektorem pojazdów znalezionych w tej pętli stąd ta ifowa magia


            if (vrPrevVehicles.empty())
            {
                vrVehicles.emplace_back(Vehicle(r0,dimensions));
            }
            else
            {
                bool flag = false;
                //sprawdź czy pojazd już zareejstrowano
                for (auto itcR = vrPrevVehicles.begin(); itcR != vrPrevVehicles.end(); ++itcR)
                {
                    Vehicle tempr = *itcR;
                    if (tempr.getDim().contains(temp)) //czy środek aktualnego pojazdu należu do prostokątu innego pojazdu
                    {
                        Point2f p2fCenter = (tempr.getDim().br() + tempr.getDim().tl()) / 2;

                        if (r0.y + r0.height >= p_pLine.first.y && r0.y < p_pLine.first.y && r0.x > p_pLine.first.x && r0.x + r0.width < p_pLine.second.x) // jeśli zahaczamy o linię to
                        {
                            //cv::rectangle(mColorFrame, r0, cv::Scalar(255, 0, 0), 2);


                            //zakładamy że w każdej klatce, w jakiej pojawił się pojazd, przesunął się o jeden piksel? Trochę to słabe
                            tempr.measure();
                            //wygląda na to, że pomiar jednego pojazdu może odbyć się dwuktrotnie, ale brany jest tylko pierwszy wynik
                            if ((std::find(vehicle_ids.begin(), vehicle_ids.end(), tempr.getID()) == vehicle_ids.end())) // jeśli nie znaleźliśmy pojazdu o tym id
                            {
                                vehicle_ids.push_back(tempr.getID());// dodaj pojazd
                                mColorFrame1(r0).copyTo(tempmat);
                                imshow("vechicles", tempmat);
                            }
                        }
                        else
                        {
                           // cv::rectangle(mColorFrame, r0, redColor, 2);
                            // cv::line(mColorFrame, cv::Point(temp.x - 10, temp.y - a * 10), cv::Point(temp.x + 10, temp.y + a * 10), cv::Scalar(255, 255, 0), 10);
                            //jeśli był już raz zmierzony (długość nie zerowa), to wyrysuj na filmie aktualne wymiaru (to jest mało widoczne)
                            if (tempr.getFrames() != 0)
                            {

                                putText(mColorFrame, std::to_string(tempr.getWidth()), r0.tl(), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1, CV_AA, false);
                                putText(mColorFrame, std::to_string(tempr.getLength()), (r0.tl() + Point(r0.width / 2, 0)), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1, CV_AA, false);
                                tempr.countLength(dimensions);
                                //tempr.box(vvpContours, 45);
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
                        for (auto itcPP = vp_p2fArgument.begin(); itcPP != vp_p2fArgument.end(); ++itcPP)
                        {
                            pair<cv::Point2f, cv::Point2f> tempPP = *itcPP;
                            if (r0.contains(tempPP.first))
                            {
                                double hypotenuse = sqrt(square(tempPP.first.y - tempPP.second.y) + square(tempPP.first.x - tempPP.second.x));
                                //znowu te magiczne widełki długość wektorka z optical flow
                                if (hypotenuse > 3 && hypotenuse < 15)
                                {
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
                            putText(tempmat1, std::to_string(tempr.getLength()).substr(0, 5), Point(0, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
                            putText(tempmat1, std::to_string(tempr.getWidth()).substr(0, 5), Point(tempmat.cols / 2, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
                            imwrite("/home/piotr/Pulpit/Samochody-Github/Optical-Flow/images/img" + std::to_string(tempr.getID()) + ".png", tempmat1, compression_params);
                        }

                        flag = true;
                        break;
                    }
                }

                //pod koniec ten wektor jest zaminaiiany z wektorem pojazdów znalezionych w tej pętli stąd ta ifowa magia
                if (!flag)
                {
                    vrVehicles.emplace_back(Vehicle(r0,dimensions));
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

void BGS::printVehicleInfo()
{
    for (auto it = measuredVehiclesVehicles.begin(); it != measuredVehiclesVehicles.end(); ++it)
    {
        // wyprintować wymiary pojazdów
        std::cout << "Vehicle Id:" << it->getID() << std::endl;
        std::cout << "Vehicle lenght:" << it->getLength() << std::endl;
        std::cout << "Vehicle width:" << it->getWidth() << std::endl;
        std::cout << "Vehicle height:" << it->getHeight() << std::endl;
    }
}

