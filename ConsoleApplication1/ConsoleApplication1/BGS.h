#pragma once
class BGS
{
private:
	int x;
	int y;
	int w;
	int h;

	cv::Rect rRect;
	cv::Mat  mColorFrame, img_mask, img_bkgmodel;

	BGS::IBGS *ibgsTest;

public:
	BGS();
	BGS(int, int, int, int);
	~BGS();

	cv::Mat execute(cv::Mat);
};

