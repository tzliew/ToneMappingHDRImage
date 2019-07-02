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
        minlum = 0.27*image.at<Vec3b>(0,0)[2] + 0.67*image.at<Vec3b>(0,0)[1] + 0.06*image.at<Vec3b>(0,0)[0];
        maxlum = minlum;
        totallum += log(minlum);
        start = 1;
    }else{
        minlum = 0.27*image.at<Vec3b>(0,0)[2] + 0.67*image.at<Vec3b>(0,0)[1] + 0.06*image.at<Vec3b>(0,0)[0];
        maxlum = 0.27*image.at<Vec3b>(0,1)[2] + 0.67*image.at<Vec3b>(0,1)[1] + 0.06*image.at<Vec3b>(0,1)[0];
        totallum = log(minlum) + log(maxlum);
        start = 2;
    }
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

            lum = 0.27*r + 0.67*g + 0.06*b;
            lum1 = 0.27*r1 + 0.67*g1 + 0.06*b1;
            totallum += log(lum) + log(lum1);
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
    return make_tuple(minlum, maxlum, avlum);
}

int main(){
    Mat image;
    double maxi = 0, mini = 500;
    int r, g, b;
    double lum, lumw, maxlum, minlum, avlum, exponentf, alpha, maxR = 0, maxG = 0, maxB = 0;
    image = imread("test.tiff ",1);
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
    exponentf = (2*log2(avlum)-log2(minlum)-log2(maxlum))/(log2(maxlum)-log2(minlum));
    alpha = 0.18 * pow(4, exponentf);
    cout << alpha <<endl;
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];

            lumw = 0.27*r + 0.67*g + 0.06*b;
            lum = alpha*lumw/avlum;
            matrix[i][j][0] = ((lum/(1+lum))*pow(b/lumw, 1));
            matrix[i][j][1] = ((lum/(1+lum))*pow(g/lumw, 1));
            matrix[i][j][2] = ((lum/(1+lum))*pow(r/lumw, 1));

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
    double l;
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){

            image.at<Vec3b>(i,j)[0] = matrix[i][j][0] * 255;
            image.at<Vec3b>(i,j)[1] = matrix[i][j][1] * 255;
            image.at<Vec3b>(i,j)[2] = matrix[i][j][2] * 255;

            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];
            l = (0.27*r + 0.67*g + 0.06*b)/255;
            image.at<Vec3b>(i,j)[0] = (l/(1-l))/l*b;
            image.at<Vec3b>(i,j)[1] = (l/(1-l))/l*g;
            image.at<Vec3b>(i,j)[2] = (l/(1-l))/l*r;
        }
    }
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);
    /**
    double lumd, lumwp;
    for(int i = 0; i<image.rows;i++){
        for(int j = 0;j<image.cols; j++){
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];
            lumd = (0.299*r + 0.587*g + 0.114*b)/255;    //the number 25 is a variable and the result will change obviously if the number changes
            lumwp = lumd/(1-lumd);
            image.at<Vec3b>(i,j)[0] = lumwp/lumd*b/63;
            image.at<Vec3b>(i,j)[1] = lumwp/lumd*g/63;
            image.at<Vec3b>(i,j)[2] = lumwp/lumd*r/63;
            b = image.at<Vec3b>(i,j)[0];
            g = image.at<Vec3b>(i,j)[1];
            r = image.at<Vec3b>(i,j)[2];
            if (b > maxi){
                maxi = b;
            }else if(g > maxi){
                maxi = g;
            }else if(r > maxi){
                maxi = r;
            }
            if (b < mini){
                mini = b;
            }else if(g < mini){
                mini = g;
            }else if(r < mini){
     *= 255/63;           mini = r;
            }
        }
    }
        cout << maxi << " " << mini << endl;
    namedWindow("Picture",CV_WINDOW_FREERATIO);
    imshow("Picture",image);
    waitKey(0);
    */
}

