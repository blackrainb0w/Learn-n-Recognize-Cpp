//
//  LBPRecognizer.hpp
//  learn-n-recognize
//
//  Created by Samuel Prevost on 25/12/2016.
//  Copyright © 2016 Samuel Prevost. All rights reserved.
//

#ifndef LBPRecognizer_hpp
#define LBPRecognizer_hpp

// Font size of displayed name (0.2 to 1 in good)
#define FONT_SIZE 0.5

#include <iostream>
#include <cstdlib>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

#include "Message.hpp"
#include "Miscellaneous.hpp"

using std::string;
using std::vector;

using cv::Scalar;
using cv::Mat;
using cv::Rect;
using cv::Size;
using cv::Point;
using cv::Ptr;
using cv::cvtColor;
using cv::COLOR_BGR2GRAY;
using cv::FONT_HERSHEY_COMPLEX;
using cv::face::FaceRecognizer;
using cv::face::createLBPHFaceRecognizer;

class LBPRecognizer {
private:
    bool isFaceRecognizerEmpty = true, hasBeenImproved = false;
    Ptr<FaceRecognizer> model;
    Mat toGrey(Mat src);
    vector<Mat> toGrey(vector<Mat> src);
public:
    LBPRecognizer(string path);
    LBPRecognizer();
    bool load(string path);
    void save(string path);
    void recognize(Mat frame, int* id, double* confidence);
    void drawNameAndConf(Mat* frame, Rect face, string name, string conf);
    
    inline bool isEmpty(){
        return this->isFaceRecognizerEmpty;
    };
    
    void train(vector<Mat> src, vector<int> labels);
    void update(vector<Mat> src, vector<int> labels);
};


#endif /* LBPRecognizer_hpp */
