#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

tuple<double, double, double> calculateMinMax(Mat image){
    int pixels, start;
    double minlum, maxlum, avlum, totallum = 0;
    pixels = image.rows * image.cols;
    if (pixels % 2 == 1){
        minlum = 0.299*image.at<Vec3b>(0,0)[2] + 0.587*image.at<Vec3b>(0,0)[1] + 0.114*image.at<Vec3b>(0,0)[0];
        maxlum = minlum;
        totallum += log(minlum);
        start = 1;
    }else{
        minlum = 0.299*image.at<Vec3b>(0,0)[2] + 0.587*image.at<Vec3b>(0,0)[1] + 0.114*image.at<Vec3b>(0,0)[0];
        maxlum = 0.299*image.at<Vec3b>(0,1)[2] + 0.587*image.at<Vec3b>(0,1)[1] + 0.114*image.at<Vec3b>(0,1)[0];
        totallum = log(minlum) + log(maxlum);
        start = 2;
    }
    cout << pixels << endl;
    int j, b, g, r, b1, g1, r1;
    double lum, lum1;
    bool over = false;
    for(int i = 0; i<image.rows; i++){
        if (i == 0){
            j = start;
        }else if (over){
            j = 1;
            over = false;
        }else{
            j = 0;
        }
        while(j<image.cols){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];
            if (j+1 < image.cols){
                b1 = image.at<Vec3b>(i,j+1)[0];
                g1 = image.at<Vec3b>(i,j+1)[1];
                r1 = image.at<Vec3b>(i,j+1)[2];
            }else{
                b1 = image.at<Vec3b>(i+1,0)[0];
                g1 = image.at<Vec3b>(i+1,0)[1];
                r1 = image.at<Vec3b>(i+1,0)[2];
                over = true;
            }

            lum = 0.299*r + 0.587*g + 0.114*b;
            lum1 = 0.299*r1 + 0.587*g1 + 0.114*b1;
            if (lum != 0){
                totallum += log(lum);
            }
            if (lum1 != 0){
                totallum += log(lum1);
            }
            if (lum < lum1){
                if (minlum > lum){
                    minlum = lum;
                }
                if (maxlum < lum1){
                    maxlum = lum1;
                }
            }else{
                if (minlum > lum1){
                    minlum = lum1;
                }
                if (maxlum < lum){
                    maxlum = lum;
                }
            }
        j+=2;
        }
    }

    avlum = exp(totallum/pixels);
    //cout << loop*2 << " "<< pixels << endl;
    //cout << totallum/pixels;
    cout << minlum << " " << maxlum << " " << avlum <<endl;
    return make_tuple(minlum, maxlum, avlum);
}

int main(){
    Mat image;
    double maxi = 0, mini = 500;
    int r, g, b;
    double lumd, lumdmax = 100, lumw, maxlum, minlum, avlum, maxR = 0, maxG = 0, maxB = 0, p = 0.7;
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
    tie(minlum, maxlum, avlum) = calculateMinMax(image);
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            lumw = 0.299*r + 0.587*g + 0.114*b;
            lumd = (lumdmax)/100/log10(1+maxlum)*log10(1+lumw)/log10(2+8*(pow((lumw/maxlum),(log10(p)/log10(0.5)))));       //Change maxlum to avlum for difference result

            matrix[i][j][0] = (lumd)*pow(b/lumw, 1);
            matrix[i][j][1] = (lumd)*pow(g/lumw, 1);
            matrix[i][j][2] = (lumd)*pow(r/lumw, 1);

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
