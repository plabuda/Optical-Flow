#include "stdafx.h"
#include "Vehicle.h"


Vehicle::Vehicle(cv::Rect dimensions)
{
	id = counter;
	this->dimensions = dimensions;
	counter++;
	length = 0;
}

Vehicle::Vehicle(cv::Rect dimensions, int id)
{
	this->dimensions = dimensions;
	this->id = id;
}


Vehicle::Vehicle(cv::Rect dimensions, Vehicle &v1)
{
	this->dimensions = dimensions;
	this->id = v1.id;
	this->length = v1.length;
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


void Vehicle::measure()
{
	length++;
}


int Vehicle::getLength()
{
	return length;
}


int Vehicle::counter = 0;

