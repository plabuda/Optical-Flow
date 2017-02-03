#pragma once
#include "opencv2/opencv.hpp"

class Vehicle
{
private:
	int id;
    cv::Rect dimensions; //prostokąt obejmujący pojazd
	double length;
	double width;
	double height;
	int speed;
	int frames;
	bool measured;

public:
	static int counter;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
    Vehicle(cv::Rect dimensions, Vehicle const&);
    Vehicle(cv::Rect, cv::Point3d &dims);
	~Vehicle();

    cv::Rect getDim() const;
	void setSpeed(int speed);
    int getID() const;
	void measure();
    void countLength(cv::Point3d &dims); //and width
    double getLength() const;
    double getFrames() const;
    double getWidth() const;
    double getHeight() const;
    bool isMeasured() const;
};

