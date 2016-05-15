#pragma once
class BGSProduct
{
private:
	cv::Mat mProduct;
	cv::Rect rRectProduct;

public:
	BGSProduct();
	BGSProduct(cv::Mat, cv::Rect);
	cv::Mat getMatP();
	cv::Rect getRectP();
	~BGSProduct();
};

