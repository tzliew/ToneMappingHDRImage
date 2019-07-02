#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

tuple<double, double> calculateMaxAvg(Mat image){
    int pixels;
    double maxlum, avlum, totallum = 0;
    pixels = image.rows * image.cols;
    maxlum = 0.299*image.at<Vec3b>(0,0)[2] + 0.587*image.at<Vec3b>(0,0)[1] + 0.114*image.at<Vec3b>(0,0)[0];
    totallum += maxlum;


    int b, g, r;
    double lum;
    for(int i = 0; i<image.rows; i++){
        for(int j = 1; j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            lum = 0.299*r + 0.587*g + 0.114*b;
            if (lum != 0){
                totallum += log(lum);
            }
            if (lum > maxlum){
                maxlum = lum;
            }
        }
    }

    avlum = exp(totallum/pixels);
    cout << maxlum << " " << avlum <<endl;
    return make_tuple(maxlum, avlum);
}

int main(){
    Mat image;
    int r, g, b;
    double lumd, lumw, maxlum, avlum, maxR = 0, maxG = 0, maxB = 0;
    image = imread("test.tiff",1);
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);
    vector<vector<vector<double> > > matrix;

  // Set up sizes. (HEIGHT x WIDTH)
    matrix.resize(image.rows);
    for (int i = 0; i < image.rows; ++i) {
        matrix[i].resize(image.cols);

        for (int j = 0; j < image.cols; ++j){
            matrix[i][j].resize(3);
        }
    }
    tie(maxlum, avlum) = calculateMaxAvg(image);
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            lumw = 0.299*r + 0.587*g + 0.114*b;
            //lumd = (log10(1 + lumw))/(log10(1 + avlum));       //Change maxlum to avlum for difference result
            lumd = 1-exp(-(lumw/maxlum));
            matrix[i][j][0] = (lumd)*pow(b/lumw, 0.8);
            matrix[i][j][1] = (lumd)*pow(g/lumw, 0.8);
            matrix[i][j][2] = (lumd)*pow(r/lumw, 0.8);

            if (matrix[i][j][0] > maxB){
                maxB = matrix[i][j][0];
            }
            if (matrix[i][j][1] > maxG){
                maxG = matrix[i][j][1];
            }
            if (matrix[i][j][2] > maxR){
                maxR = matrix[i][j][2];
            }
        }
    }

    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){

            image.at<Vec3b>(i,j)[0] = matrix[i][j][0] * 127/maxB;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] = matrix[i][j][1] * 127/maxG;
            image.at<Vec3b>(i,j)[2] = matrix[i][j][2] * 127/maxR;
            image.at<Vec3b>(i,j)[0] *= 255/127;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] *= 255/127;
            image.at<Vec3b>(i,j)[2] *= 255/127;
        }
    }
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);
}
