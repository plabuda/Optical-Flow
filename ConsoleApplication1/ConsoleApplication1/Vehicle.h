#pragma once
class Vehicle
{
private:
	int id;
	cv::Rect dimensions;
	int length;

public:
	static int counter;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
	Vehicle(cv::Rect dimensions, Vehicle&);
	~Vehicle();

	cv::Rect getDim();
	int getID();
	void measure();
	int getLength();
};

