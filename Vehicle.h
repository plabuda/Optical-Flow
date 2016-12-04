#pragma once
#include "opencv2/opencv.hpp"

class Vehicle
{
private:
	int id;
	cv::Rect dimensions;
	double length;
	double width;
	int speed;
	bool measured;

public:
	static int counter;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
	Vehicle(cv::Rect dimensions, Vehicle&);
	~Vehicle();

    cv::Rect getDim() const;
	void setSpeed(int speed);
    int getID() const;
	void measure();
    void countLength();
    double getLength() const;
    double getWidth() const;
    bool getMeasured() const;
};

