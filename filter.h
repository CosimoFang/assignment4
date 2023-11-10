//
//  filter.h
//  Created by Hongchao Fang on 10/30/23.
//

// the distance calulation, dialation, erosion, cluster to support vidDisplayCPP.project 3 header file



#ifndef __filter_h
#define __filter_h

#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
std::vector<cv::Point2f> findCorner(bool drawCorner, cv::Mat& src, cv:: Size& pattern_size);
std::vector<cv::Vec3f> createPoint(cv::Size& pattern_size);
int storeFeatures(std::string name, std::string features);
std::string toString_Mat(Mat m);
std::string toString_Array(std::vector<float> m);
std::vector<cv::Vec3f> constructPyramid();
void drawPyramid(Mat& src, std::vector<Point2f> p);
#endif#pragma once
