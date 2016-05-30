#include "stdafx.h"
#include "Vehicle.h"


Vehicle::Vehicle(cv::Rect dimensions)
{
	id = counter;
	this->dimensions = dimensions;
	counter++;
}


Vehicle::Vehicle(cv::Rect dimensions, int id)
{
	this->dimensions = dimensions;
	this->id = id;
}


Vehicle::Vehicle(cv::Rect dimensions, Vehicle prevVehicle)
{
	this->dimensions = dimensions;
	this->id = prevVehicle.getID();
}


Vehicle::~Vehicle()
{
}


cv::Rect Vehicle::getDim()
{
	return dimensions;
}


int Vehicle::getID()
{
	return id;
}


void Vehicle::addDim()
{
	vrDimensions.push_back(dimensions);
}


int Vehicle::counter = 0;