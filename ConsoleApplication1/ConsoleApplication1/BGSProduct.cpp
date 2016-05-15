#include "stdafx.h"
#include "BGSProduct.h"


BGSProduct::BGSProduct()
{
}

BGSProduct::BGSProduct(cv::Mat mArg, cv::Rect rArg)
{
	mProduct = mArg;
	rRectProduct = rArg;
}

cv::Mat BGSProduct::getMatP()
{
	return mProduct;
}

cv::Rect BGSProduct::getRectP()
{
	return rRectProduct;
}


BGSProduct::~BGSProduct()
{
}
