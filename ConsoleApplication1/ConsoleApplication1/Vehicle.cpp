#include "stdafx.h"
#include "Vehicle.h"


Vehicle::Vehicle(cv::Rect dimensions)
{
	id = counter;
	this->dimensions = dimensions;
	counter++;
	length = 0;
	width = 0;
	speed = 0;
	measured = false;
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
	this->speed = v1.speed;
	this->measured = v1.measured;
	this->width = v1.width;
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


double Vehicle::getLength()
{
	return length;
}


double Vehicle::getWidth()
{
	return width;
}


bool Vehicle::getMeasured()
{
	return measured;
}


void Vehicle::setSpeed(int speed)
{
	if (this->speed < speed)
	{
		this->speed = speed;
		return;
	}
	/*this->speed += speed;
	this->speed /= 2;*/
}


void Vehicle::countLength()
{
	if (speed == 0 || measured)
		return;
	
	length = length * speed;

	double x = 0.020625;
	/*if (length > 280)
		x = 0.0154;
	*/
	if (length < 400)
	{
		length = length + 180;
		length /= 2;
	}
	length *= x;
	width = dimensions.width * 0.016087;
	measured = true;
}

int Vehicle::counter = 0;

