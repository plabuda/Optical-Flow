#include "stdafx.h"
#include "Vehicle.h"
#include <iostream>


Vehicle::Vehicle(cv::Rect dimensions)
{
	id = counter;
	this->dimensions = dimensions;
	counter++;
	length = 0;
	height = 0;
	width = 0;
	speed = 0;
	measured = false;
}

Vehicle::Vehicle(cv::Rect dimensions, cv::Point3d &dims)
{
	id = counter;
	this->dimensions = dimensions;
	counter++;
	length = dims.x;
	height = dims.z;
	width = dims.y;
	speed = 0;
	measured = false;
}

Vehicle::Vehicle(cv::Rect dimensions, int id)
{
	this->dimensions = dimensions;
	this->id = id;
}


Vehicle::Vehicle(cv::Rect dimensions, Vehicle const& v1)
{
	this->dimensions = dimensions;
	this->id = v1.id;
	this->length = v1.length;
	this->speed = v1.speed;
	this->measured = v1.measured;
	this->width = v1.width;
	this->height = v1.height;
}


Vehicle::~Vehicle()
{
}


cv::Rect Vehicle::getDim() const
{
	return dimensions;
}


int Vehicle::getID() const
{
	return id;
}


void Vehicle::measure()
{
	frames++;
}


double Vehicle::getLength() const
{
	return length;
}

double Vehicle::getHeight() const
{
	return height;
}


double Vehicle::getWidth() const
{
	return width;
}

double Vehicle::getFrames() const
{
	return frames;
}


bool Vehicle::isMeasured() const
{
	return measured;
}


void Vehicle::setSpeed(int speed)
{
    // niedokładne strasznie i jaka była idea?
	if (this->speed < speed)
	{
		this->speed = speed;
	}
}


void Vehicle::countLength(cv::Point3d &dims)
{
	if (speed == 0 || measured)
    {
		return;
    }

	length = dims.x;
//  wykomentowałem, bo nie wiedziałem, skąd wzięły się te wartoci
//    double x = 0.020625;
//	if (length < 400)
//	{
//		length = length + 180;
//		length /= 2;
//	}
//	length *= x;
    width = dims.y; // * 0.016087;
    height = dims.z;
    std::cout << "ID: " << id << ", WIDTH (DIM): " << width << " (" << dimensions.width << ")" << std::endl;
	measured = true;
}

int Vehicle::counter = 0;


