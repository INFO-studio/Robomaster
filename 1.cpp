#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main()
{
    Mat img;
    img = imread("/home/parallels/Robomaster/learn/Resources/Untitled 5.png", IMREAD_COLOR);
    Mat hsvFrame;
    Mat gaussImg;
    Mat kernel;
    Mat REDimg1;
    Mat REDimg;
    Mat GREENimg;
    Mat BULEimg;
    vector<vector<Point>> REDcontours;
    vector<vector<Point>> GREENcontours;
    vector<vector<Point>> BLUEcontours;
    int colorThreshold[][6] =
        {
            // {hmin, hmax, smin, smax, vmin, vmax}
            {0, 5, 25, 256, 50, 256},    // RED1
            {150, 181, 25, 256, 50, 256}, // RED2
            {45, 90, 100, 256, 50, 256}, // GREEN
            {100, 120, 100, 256, 50, 256}, // BULE
        };

    cvtColor(img, hsvFrame, COLOR_BGR2HSV);

    GaussianBlur(hsvFrame, gaussImg, Size(5, 5), 0);

    inRange(gaussImg,
            Scalar(colorThreshold[0][0], colorThreshold[0][2], colorThreshold[0][4]),
            Scalar(colorThreshold[0][1], colorThreshold[0][3], colorThreshold[0][5]),
            REDimg1);
    inRange(gaussImg,
            Scalar(colorThreshold[1][0], colorThreshold[1][2], colorThreshold[1][4]),
            Scalar(colorThreshold[1][1], colorThreshold[1][3], colorThreshold[1][5]),
            REDimg);
    inRange(gaussImg,
            Scalar(colorThreshold[2][0], colorThreshold[2][2], colorThreshold[2][4]),
            Scalar(colorThreshold[2][1], colorThreshold[2][3], colorThreshold[2][5]),
            GREENimg);
    inRange(gaussImg,
            Scalar(colorThreshold[3][0], colorThreshold[3][2], colorThreshold[3][4]),
            Scalar(colorThreshold[3][1], colorThreshold[3][3], colorThreshold[3][5]),
            BULEimg);

    Mat REDimg0=REDimg+REDimg1;
    kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(REDimg0, REDimg0, MORPH_CLOSE, kernel);
    morphologyEx(REDimg0, REDimg0, MORPH_OPEN, kernel);
    morphologyEx(GREENimg, GREENimg, MORPH_CLOSE, kernel);
    morphologyEx(GREENimg, GREENimg, MORPH_OPEN, kernel);
    morphologyEx(BULEimg, BULEimg, MORPH_CLOSE, kernel);
    morphologyEx(BULEimg, BULEimg, MORPH_OPEN, kernel);

    Mat ele = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat outRed, outGreen, outBlue;
    erode(REDimg0, outRed, ele);
    erode(GREENimg, outGreen, ele);
    erode(BULEimg, outBlue, ele);

    Mat ele1 = getStructuringElement(MORPH_RECT, Size(15, 15));
    Mat outRed_, outGreen_, outBlue_;
    dilate(outRed, outRed_, ele1);
    erode(outRed_, outRed_, ele);
    dilate(outRed_, outRed_, getStructuringElement(MORPH_RECT, Size(60, 60)));
    erode(outRed_, outRed_, getStructuringElement(MORPH_RECT, Size(65, 65)));
    dilate(outGreen, outGreen_, ele);
    dilate(outBlue, outBlue_, ele);

    findContours(outRed_, REDcontours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    findContours(outGreen_, GREENcontours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    findContours(outBlue_, BLUEcontours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    drawContours(img, REDcontours, -1, Scalar(0, 0, 255), 2);
    drawContours(img, GREENcontours, -1, Scalar(0, 255, 0), 2);
    drawContours(img, BLUEcontours, -1, Scalar(255, 0, 0), 2);
    imshow("video", img);
    waitKey();

    return 0;
}