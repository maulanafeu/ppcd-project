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

int main( int argc, char** argv )
{
    using std::cout;
    using std::endl;
    if(argc != 3)
    {
        cout <<"Usage <"<< argv[0]<<"> <path to directory containing samples> <test image>" << endl;
    }

int height, width, x, y, i, minX, minY, maxX, maxY;
char imgFileName[100];


IplImage *origImage = cvLoadImage("kornet01.jpg", -1);
height = origImage->height;
width = origImage->width;

IplImage *grayImage = cvCreateImage(cvSize(width, height), 8, 1);
IplImage *binImage = cvCreateImage(cvSize(width, height), 8, 1);


//Pre-processing phase


cvCvtColor(origImage, grayImage, CV_BGR2GRAY);
cvShowImage("cv_color", grayImage);
/*cvDilate(grayImage, grayImage, NULL, 1);
cvShowImage("cv_dilate", grayImage);
cvSmooth(grayImage, grayImage, CV_GAUSSIAN, 21, 21, 0, 0);
cvShowImage("cv_smooth", grayImage);
*/
cvThreshold(grayImage, binImage, 75, 255, CV_THRESH_BINARY);

cvShowImage("cv_threshold", binImage);

cvNormalize(binImage,binImage,0,1,CV_MINMAX);

cvShowImage("cv_normalize", binImage);


minX = width;
minY = height;
maxX = 0;
maxY = 0;


CvScalar s;


for (x=228; x<1228; x++){
for(y=245; y<562; y++){
    s = cvGet2D(binImage, y, x);
    //printf("%f\n", s.val[0]);
    if (s.val[0] == 1){
        //printf("HELLO");
        minX = min(minX, x);
        minY = min(minY, y);
        maxX = max(maxX, x);
        maxY = max(maxY, y);

    }
}
}

cvSetImageROI(binImage, cvRect(minX, minY, maxX-minX, maxY-minY));

IplImage *cropImage = cvCreateImage(cvGetSize(binImage), 8, 1);

cvCopy(binImage, cropImage, NULL);

cvNormalize(cropImage, cropImage, 0, 255, CV_MINMAX);

cvErode(cropImage, cropImage, NULL, 1);
cvSaveImage("cropImage10.jpg", cropImage);
cvResetImageROI(binImage);

cvReleaseImage(&origImage);
cvReleaseImage(&binImage);

cvReleaseImage(&grayImage);
cvReleaseImage(&cropImage);

//cvWaitKey();

    IplImage* imagev = cvLoadImage("bestCase.pbm", 0);
	//////////////////
	//My OCR
	//////////////////
	int* size = new int[1];
	OCR ocr(argv[1], 62, 3);
	ocr.classify(imagev, 1, size);
    return 0;
}
