//
//  vidDisplay.cpp
//  Created by Hongchao Fang on 10/30/23.
//

//the main function of the program display img from camera

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <tuple>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "dirent.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core/core.hpp"

#include "filter.h"

#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

//main method mainly to display img from camera
int main(int argc, char* argv[]) {
    Size PATTERN_SIZE(9, 6);
    bool DISPLAY_POINT = true; //display for q1
    bool DISPLAY_CAL = true; //display for q2
    bool DISPLAY_TVEC = true; //display for q3

    std::string F_NAME_M = "matrix";
    std::string F_NAME_D = "distCoeffs";
    cv::VideoCapture* capdev;
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);
    printf("Press s to save img for calibration\n");
    printf("Press c to calibrate base on saved img\n");
    cv::namedWindow("Video", 1); // identifies a window
 


    cv::Mat frame;
    std::vector<std::vector<cv::Vec3f>> point_list;
    std::vector<std::vector<cv::Point2f>> corner_list;

    cv::Mat matrix = Mat::zeros(3, 3, CV_64FC1);

    matrix.at<cv::Vec2f>(0, 0) = 1;
    matrix.at<cv::Vec2f>(0, 2) = frame.cols / 2;
    matrix.at<cv::Vec2f>(1, 1) = 1;
    matrix.at<cv::Vec2f>(1, 2) = frame.rows / 2;
    matrix.at<cv::Vec2f>(2, 2) = 1;
    std::vector<float> dis_float;

    bool cal_mod = true;
    for (;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
       
        std::vector<cv::Point2f> corner = findCorner(DISPLAY_POINT, frame, PATTERN_SIZE);
        std::vector<cv::Vec3f> point = createPoint(PATTERN_SIZE);
        // if can't find corner, just move to next frame
        if (corner.size() != 54) {
            cv::imshow("Video", frame);
            std::cout << "cannot find the corner" << std::endl;
            continue;
        }
        //if the mod is calibration or display
        if (cal_mod) {
            if (key == 's') {
                point_list.push_back(point);
                corner_list.push_back(corner);
                std::cout << "img is saved" << std::endl;
                std::cout << "point_size " << point.size() << "    corner_size " << corner.size() << std::endl;

            }
            if (key == 'c') {
                //calibrate given the corner size
                if (corner_list.size() >= 5) {
                    cv::Mat m;
                    std::vector<float> distCoeffs;
                    std::vector<cv::Mat> rvecs;
                    std::vector<cv::Mat> tvecs;
                    double erorr = cv::calibrateCamera(point_list, corner_list, Size(frame.rows, frame.cols), m, distCoeffs, rvecs, tvecs);
                    matrix = m.clone();
                    dis_float = distCoeffs;

                    std::string m_str = toString_Mat(m);
                    std::string d_str = toString_Array(distCoeffs);
                    if (DISPLAY_CAL) {
                        std::cout << "distCoeff:" << d_str << std::endl;
                        std::cout << "matrix:" << m_str << std::endl;
                        std::cout << "erorr:" << erorr << std::endl;

                    }
                    storeFeatures(F_NAME_M, m_str);
                    storeFeatures(F_NAME_D, d_str);
                   
                }
                else {
                    std::cout << "You need to save more calibration img" << std::endl;
                    std::cout << corner_list.size() << "/5" << std::endl;
                }
            }

        }
        else {
            cv::Mat rvec;
            cv::Mat tvec;
            //get camera position
            bool status = solvePnP(point, corner, matrix, dis_float, rvec, tvec);
            if (status && DISPLAY_TVEC) {
                std::cout << "RVEC: " << std::endl;
                std::cout << toString_Mat(rvec) << std::endl;
                std::cout << "TVEC: " << std::endl;
                std::cout << toString_Mat(tvec) << std::endl;
                //display img given camera position
                std::vector<cv::Point2f> p_corner;
                projectPoints(point, rvec, tvec, matrix, dis_float, p_corner);
                //req 5 print outside corner
                circle(frame, p_corner[0], 5, Scalar(100, 30, 200), 3);
                circle(frame, p_corner[8], 5, Scalar(100, 30, 200), 3);
                circle(frame, p_corner[45], 5, Scalar(100, 30, 200), 3);
                circle(frame, p_corner[54], 5, Scalar(100, 30, 200), 3);

                //req 6 print a 3D object
                std::vector<Vec3f> objectPoints = constructPyramid();
                std::vector<Point2f> p;
                projectPoints(objectPoints, rvec, tvec, matrix, dis_float, p);
                drawPyramid(frame, p);
            }
 

        }
        
        //switch between calibration and display
        if (key == 'd' && cal_mod) {
            std::vector<float> matrix_float;
            if (corner_list.size() < 5) {
                std::cout << "You need to save more calibration img" << std::endl;
                std::cout << corner_list.size() << "/5" << std::endl;
            }
            else if (matrix.empty()) {
                std::cout << "Press c to calibrate before display" << std::endl;
                std::cout << corner_list.size() << "/5" << std::endl;
            }
            else {
                cal_mod = !cal_mod;
            }
        }
        else if (key == 'd') {
            cal_mod = !cal_mod;
        }
        


        cv::imshow("Video", frame);
    }

    delete capdev;
    return(0);
}

