//
//  filter.cpp
//  Created by Hongchao Fang on 10/30/23.
//

//the helper method for the main, ablt to find coner create the pyramid for display.


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/calib3d.hpp>

#include <numeric>
#include <map>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <filter.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>  
using namespace cv;

//create and display the corner on the img
std::vector<cv::Point2f> findCorner(bool drawCorner, cv::Mat& src, cv::Size& pattern_size) {
    std::vector<cv::Point2f> corner_set;
    bool pattern_found = cv::findChessboardCorners(src, pattern_size, corner_set);
    if (pattern_found) {
        Mat grayscale;
        cvtColor(src, grayscale, COLOR_BGR2GRAY);
        Size subPixWinSize(10, 10);
        TermCriteria termCrit(TermCriteria::COUNT | TermCriteria::EPS, 1, 0.1);
        cornerSubPix(grayscale, corner_set, subPixWinSize, Size(-1, -1), termCrit);
    }
    if (pattern_found && drawCorner) {
        cv::drawChessboardCorners(src, pattern_size, cv::Mat(corner_set), pattern_found);
    }

    return corner_set;
}

//create the point on the img
std::vector<cv::Vec3f> createPoint(cv::Size& pattern_size) {
    std::vector<cv::Vec3f> points;
    for (int i = 0; i < pattern_size.height; i++) {
        for (int j = 0; j < pattern_size.width; j++) {
            Vec3f coordinates = Vec3f(j, -i, 0);
            points.push_back(coordinates);
        }
    }
    return points;
}

/// <summary>
/// print string m
/// </summary>
/// <param name="m"> int put matrix m</param>
/// <returns>the m in string from each col to row</returns>
std::string toString_Mat(Mat m) {
    std::string result = "";

    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            result += std::to_string(m.at<double>(i, j)) + " ";
        }
    }
    return result;
}

/// <summary>
/// print array to string
/// </summary>
/// <param name="m">int put vector m</param>
/// <returns>m in string for each element in sequence</returns>
std::string toString_Array (std::vector<float> m) {
    std::string result = "";
    for (int i = 0; i < m.size(); i++) {
        result += std::to_string(m[i]) + " ";
    }
    return result;
}

/// <summary>
/// //store the feature into name .txt file
/// </summary>
/// <param name="name">text file name</param>
/// <param name="features"> target string</param>
/// <returns> 0 </returns>
int storeFeatures(std::string name, std::string features) {
    std::ofstream fout;
    fout.open(name + ".txt", std::ios_base::app);
    if (fout.is_open()) {
        fout << features << std::endl;
        fout.close();

    }
    return 0;
}

/// <summary>
/// construct pytramid in vec3f
/// </summary>
/// <returns>the pyramid in points</returns>
std::vector<Vec3f> constructPyramid() {
    std::vector<Vec3f> result;
    result.push_back(Vec3f(1, -1, 1));
    result.push_back(Vec3f(1, -8, 1));
    result.push_back(Vec3f(8, -8, 1));
    result.push_back(Vec3f(8, -1, 1));


    result.push_back(Vec3f(3, -3, 3));
    result.push_back(Vec3f(3, -6, 3));
    result.push_back(Vec3f(6, -6, 3));
    result.push_back(Vec3f(6, -3, 3));

    result.push_back(Vec3f(4, -4, 5));
    result.push_back(Vec3f(4, -5, 5));
    result.push_back(Vec3f(5, -5, 5));
    result.push_back(Vec3f(5, -4, 5));
    return result;
}

/// <summary>
/// connect points to form pyramid
/// </summary>
/// <param name="src"> img to draw</param>
/// <param name="p">those pointsS</param>
void drawPyramid(Mat& src, std::vector<Point2f> p) {
    line(src, p[0], p[1], Scalar(100, 30, 200), 3);
    line(src, p[1], p[2], Scalar(100, 30, 200), 3);
    line(src, p[2], p[3], Scalar(100, 30, 200), 3);
    line(src, p[0], p[3], Scalar(100, 30, 200), 3);

    line(src, p[4], p[5], Scalar(100, 30, 200), 3);
    line(src, p[5], p[6], Scalar(100, 30, 200), 3);
    line(src, p[6], p[7], Scalar(100, 30, 200), 3);
    line(src, p[7], p[4], Scalar(100, 30, 200), 3);

    line(src, p[8], p[9], Scalar(100, 30, 200), 3);
    line(src, p[9], p[10], Scalar(100, 30, 200), 3);
    line(src, p[11], p[10], Scalar(100, 30, 200), 3);
    line(src, p[11], p[8], Scalar(100, 30, 200), 3);


    line(src, p[0], p[4], Scalar(100, 30, 200), 3);
    line(src, p[1], p[5], Scalar(100, 30, 200), 3);
    line(src, p[2], p[6], Scalar(100, 30, 200), 3);
    line(src, p[3], p[7], Scalar(100, 30, 200), 3);

    line(src, p[8], p[4], Scalar(100, 30, 200), 3);
    line(src, p[9], p[5], Scalar(100, 30, 200), 3);
    line(src, p[10], p[6], Scalar(100, 30, 200), 3);
    line(src, p[11], p[7], Scalar(100, 30, 200), 3);
}