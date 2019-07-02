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
            lum = 0.299*r + 0.587*g + 0.114*b + pow(10, -8);
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
    double lumd, lumw, avlum, lumda = 30, scale, maxR = 0, maxG = 0, maxB = 0;
    int r, g, b;
    image = imread("test.tiff",1);
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);

    avlum = calculateAvg(image);
    cout << avlum << endl;
    vector<vector<vector<double> > > matrix;

  // Set up sizes. (HEIGHT x WIDTH)
    matrix.resize(image.rows);
    for (int i = 0; i < image.rows; ++i) {
        matrix[i].resize(image.cols);

        for (int j = 0; j < image.cols; ++j){
            matrix[i][j].resize(3);
        }
    }

    scale = (1/lumda)*pow(((1.219 + pow((lumda/2),0.4))/(1.219 + pow(avlum,0.4))),2.5);
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            lumw = 0.299*r + 0.587*g + 0.114*b;
            lumd = scale*lumw;
            matrix[i][j][0] = ((lumd)*pow(b/lumw,0.1));
            matrix[i][j][1] = ((lumd)*pow(g/lumw,0.1));
            matrix[i][j][2] = ((lumd)*pow(r/lumw,0.1));

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

            image.at<Vec3b>(i,j)[0] = matrix[i][j][0] * 7/maxB;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] = matrix[i][j][1] * 7/maxG;
            image.at<Vec3b>(i,j)[2] = matrix[i][j][2] * 7/maxR;
            image.at<Vec3b>(i,j)[0] *= 255/7;  //it will be darker when the 63 decreases
            image.at<Vec3b>(i,j)[1] *= 255/7;
            image.at<Vec3b>(i,j)[2] *= 255/7;
        }
    }

    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);


}

