#include "stdafx.h"
#include "Vehicle.h"


Vehicle::Vehicle(cv::Rect dimensions)
{
	id = counter;
	this->dimensions = dimensions;
	counter++;
	length = 0;
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

void Vehicle::setSpeed(int speed)
{
	if (this->speed == 0)
	{
		this->speed = speed;
		return;
	}
	this->speed += speed;
	this->speed /= 2;
}


void Vehicle::countLength()
{
	if (speed == 0 || measured)
		return;
	double x = 0.1575445;
	length = length * x * speed;
	measured = true;
}

int Vehicle::counter = 0;

