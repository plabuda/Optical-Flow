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


cv::Point BGS::get_lowpoint(cv::Rect r0, double angle, std::vector<cv::Point_<int> > ict,  bool draw)
{
    cv::Point result;
    Point2f temp = Point2f((r0.br() + r0.tl()) / 2);
    //konstrukcja pudełka
    //wzór prostej nieprostopadłej - y = ax + b
    //obliczamy a
    double a = angle;
    //dla danego punktu x, y znajdujemy b rownaniem b = y - ax
    // znajdujemy b srodka
    double h0 = temp.y - a * temp.x;
    //proste górna i dolna zadane są równaniamy y = ax + h0 + hg oraz y = ax + h0 + hd
    //wyznaczamy początkowe, maksymalnie złe wartości
    double hd = -0.75 * abs(a) * r0.height;
    //magiczny numerek 0.75 to półtora połowy rozmiaru pudełka
    //w sumie nie można by wystartować od zera? Do sprawdzenia

    //iteruj po konturach

    for (auto itc2 = ict.begin(); itc2 != ict.end(); ++itc2)
    {

        // jeśli punkt konturu należy do r0 policz wysokość linii przez niego przechodzącej
        if( r0.contains(*itc2))
        {
            double h_new = itc2->y - a * itc2->x;
            h_new = h0 - h_new;
            // poprawianie hg i hd
            if( h_new > hd) hd = h_new;
        }


    }
    //Obliczanie punktów
    // przeciecie y = ax + b z osia y = n  -> x = (n - b)/a
    //przeciecie z y = ax + b z osia x = n -> y = an + b
    if(a < 0 )
    {

        return cv::Point((r0.y + r0.height - h0 - hd)/a, r0.y + r0.height);
    }
    else
    {

        return cv::Point((r0.y + r0.height - h0 - hd)/a, r0.y + r0.height);
    }

}


cv::Point3d BGS::construct_box(cv::Rect r0, double angle, std::vector<cv::Point_<int> > ict,  bool draw)
{
    Point3d result;
    Point2f temp = Point2f((r0.br() + r0.tl()) / 2);


    cv::Point A, B, C, D, E, F;
    Point3d dimensions;
    //konstrukcja pudełka
    //wzór prostej nieprostopadłej - y = ax + b
    //obliczamy a
    double a = angle;
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

    for (auto itc2 = ict.begin(); itc2 != ict.end(); ++itc2)
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


        if(draw)
        {
            cv::line(mColorFrame, A, B,  cv::Scalar(255,0,0), 2);
            cv::line(mColorFrame, B, C,  cv::Scalar(0,255,0), 2);
            cv::line(mColorFrame, C, D,  cv::Scalar(0,0,255), 2);
            cv::line(mColorFrame, D, E,  cv::Scalar(255,0,0), 2);
            cv::line(mColorFrame, E, F,  cv::Scalar(0,255,0), 2);
            cv::line(mColorFrame, F, A,  cv::Scalar(0,0,255), 2);

        }
        dimensions.x = cv::norm(C-D); //oś x - wyznaczona ze współczynnika a - długość auta
        dimensions.y = cv::norm(E-D); //oś y - nie oś x i nie oś pionowa - szerokość auta
        dimensions.z = cv::norm(F-E); //oś z - oś pionowa - wysokość auta


    }
    else
    {
        A = cv::Point(r0.x, r0.y);
        B = cv::Point((r0.y - h0 - hg)/a, r0.y);
        C = cv::Point(r0.x + r0.width, a * (r0.x + r0.width) + h0 + hg);
        D = cv::Point(r0.x + r0.width, r0.y + r0.height);
        E = cv::Point((r0.y + r0.height - h0 - hd)/a, r0.y + r0.height);
        F = cv::Point(r0.x, a * (r0.x) + h0 + hd);

        if(draw)
        {
            cv::line(mColorFrame, A, B,  cv::Scalar(255,0,0), 2);
            cv::line(mColorFrame, B, C,  cv::Scalar(0,0,255), 2);
            cv::line(mColorFrame, C, D,  cv::Scalar(0,255,0), 2);
            cv::line(mColorFrame, D, E,  cv::Scalar(255,0,0), 2);
            cv::line(mColorFrame, E, F,  cv::Scalar(0,0,255), 2);
            cv::line(mColorFrame, F, A,  cv::Scalar(0,255,0), 2);
        }


        dimensions.x = cv::norm(F-E);
        dimensions.y = cv::norm(E-D);
        dimensions.z = cv::norm(C-D);
    }


    result = dimensions;
    return result;

}



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
        Point3d dimensions =  construct_box(r0,1,*itc,false);
        cv::circle(mColorFrame, get_lowpoint(r0,1,*itc,false) , 5, cv::Scalar(0, 255, 255), -1, 8, 0);

        //patrzy czy prostokąt obejmujący pojaz jest odpowiednio wielki, i patrzy na proporcje długość szerokość
        if(r0.area() > 2600 && dimensions.x > dimensions.y * 1.2 && get_lowpoint(r0,1,*itc,false).x > p_pLine.first.x && get_lowpoint(r0,1,*itc,false).x < p_pLine.second.x && get_lowpoint(r0,1,*itc,false).y > p_pLine.first.y)
        {
            construct_box(r0,1,*itc,true);

           // konstrukcja pudełka
            Point2f temp = Point2f((r0.br() + r0.tl()) / 2);




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

                       // if (r0.y + r0.height >= p_pLine.first.y && r0.y < p_pLine.first.y && r0.x > p_pLine.first.x && r0.x + r0.width < p_pLine.second.x) // jeśli zahaczamy o linię to
                      if(true)//   if(temp.x > p_pLine.first.x && temp.x < p_pLine.second.x)

                            //warunek do poprawienia, zdecydowanie
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

                                //zapisz obraz pojazdu
                                vector<int> compression_params;

                                tempr.addSnapshot(tempmat,dimensions);

                                /*cv::Mat tempmat1;
                                tempmat.copyTo(tempmat1);
                                tempr.snapshots.push_back(tempmat1);
                                tempr.snapshots_dims.push_back(dimensions);
                                compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
                                compression_params.push_back(9);
                                putText(tempmat1, std::to_string(tempr.getLength()).substr(0, 5), Point(0, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
                                putText(tempmat1, std::to_string(tempr.getWidth()).substr(0, 5), Point(tempmat.cols / 2, tempmat.rows - 3), FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
                                long volume = 1000 * dimensions.x * dimensions.y * dimensions.z;
                                imwrite("/home/piotr/Pulpit/Samochody-Github/Optical-Flow/images/img" + std::to_string(tempr.getID()) + "_" +  std::to_string(volume) + ".png", tempmat1, compression_params);

                                */
                                // ????

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

                                    measuredVehicleIt->addSnapshot(tempmat,dimensions);
                                }


                            }
                        }
                        vrVehicles.emplace_back(Vehicle(r0, tempr));
                        //te proste linie łączące fioletowe czasem punkty prostymi na filmie. Skąd one się biorą?
                        line(mColorFrame, p2fCenter, temp, redColor, 5, CV_AA, 0);
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

void BGS::printVehicleInfo(cv::Point3d coeffs)
{
    for (auto it = measuredVehiclesVehicles.begin(); it != measuredVehiclesVehicles.end(); ++it)
    {

        // wyprintować wymiary pojazdów
        // posortować snapshoty po objętości, wybraź medianę?
        it->Sort();
        it->Save();
        std::cout << "Vehicle Id:" << it->getID() << std::endl;
        std::cout << "Vehicle lenght:" << it->getLength() <<"px " << it->getLength() * coeffs.x << "cm"<< std::endl;
        std::cout << "Vehicle width:" << it->getWidth() <<"px " << it->getWidth() * coeffs.y << "cm"<< std::endl;
        std::cout << "Vehicle height:" << it->getHeight() <<"px " << it->getHeight() * coeffs.z << "cm"<< std::endl;
        std::cout << "snapshots: " << it->snapshots.size() << std::endl;
    }
}

