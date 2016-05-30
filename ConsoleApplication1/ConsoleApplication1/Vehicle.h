#pragma once
#include <vector>
class Vehicle
{
private:
	int id;
	cv::Rect dimensions;
	std::vector<cv::Rect> vrDimensions;

public:
	static int counter;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
	Vehicle(cv::Rect, Vehicle);
	~Vehicle();

	cv::Rect getDim();
	int getID();
	void addDim();
	
};

