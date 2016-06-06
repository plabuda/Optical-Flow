#pragma once
class Vehicle
{
private:
	int id;
	cv::Rect dimensions;
	double length;
	int speed;
	bool measured;

public:
	static int counter;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
	Vehicle(cv::Rect dimensions, Vehicle&);
	~Vehicle();

	cv::Rect getDim();
	void setSpeed(int speed);
	int getID();
	void measure();
	void countLength();
	double getLength();
};

