#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

double calculateAvg(Mat image){
    int pixels;
    double avlum, totallum = 0;
    pixels = image.rows * image.cols;


    int b, g, r;
    double lum;
    for(int i = 0; i<image.rows; i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];
            lum = 0.299*r + 0.587*g + 0.114*b;
            if (lum != 0){
                totallum += log(lum);
            }
        }
    }
    avlum = exp(totallum/pixels);
    return avlum;
}

int main(){
    Mat image;
    double lum, lumw, avlum, lumda = 30, gammada, gammawa, maxR = 0, maxG = 0, maxB = 0;
    int r, g, b;
    image = imread("test.tiff",1);
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);

    avlum = calculateAvg(image);

    vector<vector<vector<double> > > matrix;

  // Set up sizes. (HEIGHT x WIDTH)
    matrix.resize(image.rows);
    for (int i = 0; i < image.rows; ++i) {
        matrix[i].resize(image.cols);

        for (int j = 0; j < image.cols; ++j){
            matrix[i][j].resize(3);
        }
    }
    /**
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];
            matrix[i][j][0] = b*0.00392157;
            matrix[i][j][1] = g*0.00392157;
            matrix[i][j][2] = r*0.00392157;

        }
    }
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){

            image.at<Vec3b>(i,j)[0] = matrix[i][j][0] * 7;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] = matrix[i][j][1] * 7;
            image.at<Vec3b>(i,j)[2] = matrix[i][j][2] * 7;
            image.at<Vec3b>(i,j)[0] *= 255/7;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] *= 255/7;
            image.at<Vec3b>(i,j)[2] *= 255/7;
            image.at<Vec3b>(i,j)[0] += 0;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] += 0;
            image.at<Vec3b>(i,j)[2] += 0;
        }
    }
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);
    */

    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            lumw = 0.299*r + 0.587*g + 0.114*b;
            if (lumda > 100){
                gammada = 2.655;
            }else{
                gammada = 1.855 + 0.4 * log10(lumda + 0.000023);
            }
            if (avlum > 100){
                gammawa = 2.655;
            }else{
                gammawa = 1.855 + 0.4 * log10(avlum + 0.000023);
            }
            lum = 1*lumda*pow((lumw/avlum),(gammawa/gammada));

            matrix[i][j][0] = ((lum)*pow(b/lumw,0.1));
            matrix[i][j][1] = ((lum)*pow(g/lumw,0.1));
            matrix[i][j][2] = ((lum)*pow(r/lumw,0.1));

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

            image.at<Vec3b>(i,j)[0] = matrix[i][j][0] * 3/maxB;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] = matrix[i][j][1] * 3/maxG;
            image.at<Vec3b>(i,j)[2] = matrix[i][j][2] * 3/maxR;
            image.at<Vec3b>(i,j)[0] *= 255/3;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] *= 255/3;
            image.at<Vec3b>(i,j)[2] *= 255/3;
        }
    }
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);


}

