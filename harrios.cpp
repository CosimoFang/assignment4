///
// harris.cpp
//  Created by Hongchao Fang on 10/30/23.
//

//the main function of the program display feature by using cornerHarris

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

//main method to display img from camera

int main(int argc, char* argv[]) {
    cv::VideoCapture* capdev;
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);
    Mat frame;

    cv::namedWindow("Video", 1); // identifies a window

    int blockSize = 2;
    int kSize = 3;
    double k = 0.04;

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

        //cr https://docs.opencv.org/3.4/d4/d7d/tutorial_harris_detector.html
        Mat input;
        Mat dst;
        cvtColor(frame, input, COLOR_BGR2GRAY);
        cornerHarris(input, dst, blockSize, kSize, k);
        double m = 0;
        for (int i = 0; i < dst.rows; i++) {
            for (int j = 0; j < dst.cols; j++) {
                m = max<float>(m, dst.at<float>(i, j));
            }
        }
        m = 0.1 * m;
        for (int i = 0; i < dst.rows; i++) {
            for (int j = 0; j < dst.cols; j++) {
                if ((int) dst.at<float>(i, j) > m) {
                    circle(frame, Point(j, i), 1, Scalar(100, 30, 200), 3);
                }
            }
        }


        cv::imshow("Video", frame);
    }

    delete capdev;
    return(0);
}