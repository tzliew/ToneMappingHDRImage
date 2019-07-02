#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

double calculateMaxBrightness(Mat image){
    int r, g, b;
    double currlum, x, k, maxQ, q;

    currlum = 0.299*image.at<Vec3b>(0,0)[2] + 0.587*image.at<Vec3b>(0,0)[1] + 0.114*image.at<Vec3b>(0,0)[0];
    x = 0.338 * pow(currlum, 0.034);
    k = -1.5 * log10(currlum) + 6.1;
    maxQ = k * pow(currlum, x);

    for(int i = 0; i<image.rows; i++){
        for(int j = 1; j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            currlum = 0.299*r + 0.587*g + 0.114*b;
            x = 0.338 * pow(currlum, 0.034);
            k = -1.5 * log10(currlum) + 6.1;
            q = k * pow(currlum, x);

            if (q > maxQ){
                maxQ = q;
            }
        }
    }
    return maxQ;
}

int main(){
    Mat image;
    int r, g, b;
    double currlum, maxQ, x, k, q, displayQ, displayMax=30;
    image = imread("test.tiff",1);
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);

    maxQ = calculateMaxBrightness(image);
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            currlum = 0.299*r + 0.587*g + 0.114*b;
            x = 0.338 * pow(currlum, 0.034);
            k = -1.5 * log10(currlum) + 6.1;
            q = k * pow(currlum, x);

            displayQ = q/maxQ*displayMax;
            cout << displayQ << endl;
            //image.at<Vec3b>(i,j)[0] = (lum/(1+lum))*pow(b/ lumw,1.3)*255;
            //image.at<Vec3b>(i,j)[1] = (lum/(1+lum))*pow(g/ lumw,1.3)*255;
            //image.at<Vec3b>(i,j)[2] = (lum/(1+lum))*pow(r/ lumw,1.3)*255;
            //cout << (lum/(1+lum))*b/ lumw <<endl;
        }
    }
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);
}

