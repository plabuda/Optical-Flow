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
    sorted = false;
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
    sorted = false;
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
    this->sorted = v1.sorted;
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

void Vehicle::addSnapshot(cv::Mat snapshot, cv::Point3d dimensions)
{
    sorted = false;
    snapshots.push_back(snapshot);
    snapshots_dims.push_back(dimensions);
}

int Vehicle::Compare(cv::Point3d dim1, cv::Point3d dim2)
{
    double a = dim1.x * dim1.y * dim1.z;
    double b = dim2.x * dim2.y * dim2.z;
    if(a == b) return 0;
    if(a > b) return -1;
    if(b < a) return 1;
}

void Vehicle::Sort()
{
    for(int i = 1; i < snapshots.size(); i++)
    {
        for(int j = 1; j < snapshots.size() - i + 1; j++)
        {
            if(Compare(snapshots_dims[j-1],snapshots_dims[j]) < 0)
            {
                cv::Mat tempmat;
                cv::Point3d tempdim;
                tempmat = snapshots[j-1];
                snapshots[j-1] = snapshots[j];
                snapshots[j] = tempmat;
                tempdim = snapshots_dims[j-1];
                snapshots_dims[j-1] = snapshots_dims[j];
                snapshots_dims[j] = tempdim;


            }




        }
    }

sorted = true;


}

void Vehicle::Save()
{
    if(!sorted || snapshots.size() == 0) return;
    cv::Point3d dims = snapshots_dims[snapshots_dims.size()/2];
    length = dims.x;
    width = dims.y;
    height = dims.z;

    cv::Scalar redColor(0, 0, 255);
    cv::Mat tempmat1;
    snapshots[snapshots.size()/2].copyTo(tempmat1);

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    //putText(tempmat1, std::to_string(getLength()).substr(0, 5), cv::Point(0, tempmat1.rows - 3), cv::FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
    //putText(tempmat1, std::to_string(getWidth()).substr(0, 5), cv::Point(tempmat1.cols / 2, tempmat1.rows - 3), cv::FONT_HERSHEY_SIMPLEX, 0.5, redColor, 1, CV_AA, false);
    imwrite("/home/piotr/Pulpit/Samochody-Github/Optical-Flow/images/img" + std::to_string(getID()) + ".png", tempmat1, compression_params);



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


