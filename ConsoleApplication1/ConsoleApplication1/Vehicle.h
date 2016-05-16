#pragma once
class Vehicle
{
private:
	int id;
	cv::Rect dimensions;

public:
	static int counter;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
	~Vehicle();

	cv::Rect getDim();
	int getID();
};

