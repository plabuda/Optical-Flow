#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Vehicle
{
private:
	int id;
    cv::Rect dimensions; //prostokąt obejmujący pojazd
	double length;
	double width;
	double height;
	int speed;
	int frames;
	bool measured;
	bool sorted;

public:
	void addSnapshot(cv::Mat snapshot, cv::Point3d dimensions);
	void Save();
	void Sort();
	int Compare(cv::Point3d dim1, cv::Point3d dim2);
	static int counter;
	std::vector<cv::Mat> snapshots;
	std::vector<cv::Point3d> snapshots_dims;
	Vehicle(cv::Rect);
	Vehicle(cv::Rect, int);
    Vehicle(cv::Rect dimensions, Vehicle const&);
    Vehicle(cv::Rect, cv::Point3d &dims);
	~Vehicle();

    cv::Rect getDim() const;
	void setSpeed(int speed);
    int getID() const;
	void measure();
    void countLength(cv::Point3d &dims); //and width
    double getLength() const;
    double getFrames() const;
    double getWidth() const;
    double getHeight() const;
    bool isMeasured() const;
};

