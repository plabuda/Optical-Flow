#pragma once
class Vehicle
{
private:
	int id;
	cv::Rect dimensions;
	int length;
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
	int getLength();
};

