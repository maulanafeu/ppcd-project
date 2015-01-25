#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include "OCR.h"
#endif

using namespace std;
using namespace cv;

int main()
{
    Mat im_rgb = imread("popmie.jpg");
    Mat im_gray;
    cvtColor(im_rgb, im_gray, CV_RGB2GRAY);

    Mat img_bw = im_gray > 128;

    imwrite("popmiebw.jpg", img_bw);

    cvWaitKey(0);
    return 0;

    vWaitKey(0);
    return 0;
}
