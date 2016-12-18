#pragma once
#include "opencv2/opencv.hpp"

class Vehicle
{
private:
	int id;
    cv::Rect dimensions; //prostokąt obejmujący pojazd
	double length;
	double width;
	int speed;
	bool measured;

public:
	static int counter;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
    Vehicle(cv::Rect dimensions, Vehicle const&);
	~Vehicle();

    cv::Rect getDim() const;
	void setSpeed(int speed);
    int getID() const;
	void measure();
    void countLength(); //and width
    double getLength() const;
    double getWidth() const;
    bool isMeasured() const;
};

