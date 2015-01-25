/*
 *  basicOCR.c
 *
 *
 *  Created by damiles on 18/11/08.
 *  Modified by Vikram renamed to OCR.cpp
 *
 */

#include  <vector>
#include "OCR.h"
#include<stdio.h>
#include <time.h>
#include<stdlib.h>

typedef struct tanggal{
int date;
int month;
int year;
};
int bulan[12]={31,28,31,30,31,30,31,31,30,31,30,31};
int bulan_kabisat[12]={31,29,31,30,31,30,31,31,30,31,30,31};
tanggal selisih[2];
/// <summary>
///     Reads the sample images and associated charaters into trainClasses and trainData respectively.
/// </summary>
/// <returns> Nothing. </returns>

const std::string currentTahun() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y", &tstruct);
    return buf;


}
const std::string currentBulan() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%m", &tstruct);
    return buf;


}
const std::string currentTanggal() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%d", &tstruct);
    return buf;


}


int hasil_tahunsama(int a,int b,int c){
int hasil=0;
for(int i=a-1;i<=b-1;i++){
if(c%4==0){
hasil=hasil+bulan_kabisat[i];
}else{
hasil=hasil+bulan[i];
}

}

int batas_bawah=bulan[b+1]-selisih[1].date;
int hasilnya=hasil-(selisih[0].date+batas_bawah);
int hasil_akhir=abs(hasilnya);
return hasil_akhir;
}

//RUMUS 2________________________________________________
int hasil_tahunbeda(int tahun1,int tahun2,int bln1,int bln2,int tgl1,int tgl2){

int hasil_thn=0,hasil_bln=0;

for(int i=tahun1;i<=tahun2;i++){
if(i%4==0){
hasil_thn=hasil_thn+366;
}else{
hasil_thn=hasil_thn+365;
}
}
for(int j=0;j<bln1-1;j++){
if(tahun1%4==0){
hasil_bln=hasil_bln+bulan_kabisat[j];
}else{
hasil_bln=hasil_bln+bulan[j];
}

}
for(int k=bln2-1;k<12;k++){
if(tahun2%4==0){
hasil_bln=hasil_bln+bulan_kabisat[k];
}else{
hasil_bln=hasil_bln+bulan[k];
}
}
int hasilnya=((hasil_thn-hasil_bln)-tgl1)+tgl2;

}

void OCR::getData()
{
	IplImage* src_image;
	IplImage prs_image;
	CvMat row,data;
	char file[255];
	char dataFile[255];
	std::ifstream labelStream;
	std::ostringstream outStringStream;
	char ch;
	int i,j;

	for(i = 0; i < classes; i++)
	{ //26
	    //Read the corresposampleUppercase.pbmnding character for current sample being processed into ch.
	    sprintf(dataFile,"%s%d/data.txt",file_path, i);
	    labelStream.open(dataFile);
	    labelStream >> ch;
	    //ir[i] = ch - '0';
	    labelStream.close();
		for( j = 0; j< train_samples; j++)
		{ //3
			//Load file
			//get the path of image for training into file.
			if(j<10)
				sprintf(file,"%s%d/%d0%d.pbm",file_path, i, i, j);
			else
				sprintf(file,"%s%d/%d%d.pbm",file_path, i, i, j);

			src_image = cvLoadImage(file,0);
			if(!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				//exit(-1);
			}

			//process file
			prs_image = preprocessing(src_image, size, size);
			//Set class label
			cvGetRow(trainClasses, &row, i*train_samples + j);
			cvSet(&row, cvRealScalar(ch));
			//Set data
			cvGetRow(trainData, &row, i*train_samples + j);
			IplImage* img = cvCreateImage( cvSize( size, size ), IPL_DEPTH_32F, 1 );
			//convert 8 bits image to 32 float image
			cvConvertScale(&prs_image, img, 0.0039215, 0);
			cvGetSubRect(img, &data, cvRect(0,0, size,size));

			CvMat row_header, *row1;
			//convert data matrix sizexsize to vecor
			row1 = cvReshape( &data, &row_header, 0, 1 );
			cvCopy(row1, &row, NULL);
		}
	}
}

/// <summary>
///     Trains using trainData and trainClasses using k-nearest algorithm and result is saved in knn.
/// <summary>
/// <returns> Nothing. </returns>
void OCR::train()
{
	knn=new CvKNearest( trainData, trainClasses, 0, false, K );
}

/// <summary>
///     Classifies the given img and returns the result, if showResult is 1 then result is printed on std out before returning.
/// </summary>
/// <param name="img">
///     IplImage to be classified.
/// </param>
/// <param name="showResult">
///     If its 1 then the result is printed onto std out.
/// </param>
/// <returns> Result after classifying. </returns>
float* OCR::classify(IplImage* img, int showResult, int* resultSize)
{
	float *result;
	result = preprocessPara(img, size, size, showResult, resultSize);

	printf("\n");
	return result;
}

/// <summary>
///     Classifies the given prs_image by running k-nearest algorithm and prints the result.
/// </summary>
/// <param name="prs_image">
///     IplImage to be classified.
/// </param>
/// <param name="showResult">
///     If its 1, then prints result after classifying.
/// </param>
/// <returns> Result after classifying given image. </returns>
float OCR::print(IplImage prs_image, int showResult)
{
    float result;
    int ar[10];
    int m=0;
    int n=0;
    CvMat data;
    CvMat* nearest=cvCreateMat(1,K,CV_32FC1);
    //Set data
	IplImage* img32 = cvCreateImage( cvSize( size, size ), IPL_DEPTH_32F, 1 );
	cvConvertScale(&prs_image, img32, 0.0039215, 0);
	cvGetSubRect(img32, &data, cvRect(0,0, size,size));
	CvMat row_header, *row1;
	row1 = cvReshape( &data, &row_header, 0, 1 );

	result=knn->find_nearest(row1,K,0,0,nearest,0);

    if(showResult == 1)
        {
            char r = result;
            int ir[17];
            int accuracy=0;
            for(int i=0;i<K;i++)
            {
                if( nearest->data.fl[i] == result)
                    accuracy++;
            }
            //for(n=0;n<17;n++){
               printf("%c ",r);
                ir[n] = r - '0';
            //}
            result = ir[n];
            m++;





    //        float pre=100*((float)accuracy/(float)K);
    //        printf("|\t%c\t| \t%.2f%%  \t| \t%d of %d \t",r,pre,accuracy,K);
    //        printf(" \n---------------------------------------------------------------\n");
            }
            //printf("%d ", ar[0]);
                //int tglx = ir[13];
	/*
    for(i = 0; i < 17; i++){
        printf("\n%d ", ir[i]);
    }
    */
	return result;
}

/// <summary>
///     Creates new instance of the OCR class. Trains the k-nearest algorithm with given data.
/// </summary>
/// <params name="path">
///     Relative or absolute path of the directory under which training samples are located.
/// </params>
/// <params name="classes">
///     Number of possible classes into which data can be classified into.
/// </params>
/// <params name="samples">
///     Total number of samples for each class type.
/// </params>
OCR::OCR(char* path, int classe, int samples)
{
	sprintf(file_path, "%s", path);
	//file_path = path;
	train_samples = samples;
	classes = classe;
	size = 80;
	trainData = cvCreateMat(train_samples*classes, size*size, CV_32FC1);
	trainClasses = cvCreateMat(train_samples*classes, 1, CV_32FC1);

	//Get data (get images and process it)
	getData();

	//train
	train();

	printf(" ---------------------------------------------------------------\n");
	printf("Deteksi Tgl Kadaluarsa | Metode KNN | PPCD\n");
	printf(" ---------------------------------------------------------------\n");
}

/*****************************************************************
*
* Find the min box. The min box respect original aspect ratio image
* The image is a binary data and background is white.
*
*******************************************************************/

/// <summary>
///     Finds min and max X of the data present in given image.
/// </summary>
/// <params name="imsSrc">
///     Source image for which min and max X has to be found.
/// </params>
/// <params name="min">
///     Int pointer where the min X has to saved.
/// </params>
/// <params name="max">
///     Int pointer where the max X has to saved.
/// </params>
/// <returns> Nothing. </returns>
void OCR::findX(IplImage* imgSrc,int* min, int* max)
{
	int i;
	int minFound=0;
	CvMat data;
	CvScalar maxVal=cvRealScalar(imgSrc->height * 255);
	CvScalar val=cvRealScalar(0);
	//For each col sum, if sum < width*255 then we find the min
	//then continue to end to search the max, if sum< width*255 then is new max
	for (i=0; i< imgSrc->width; i++)
	{
	    val = cvRealScalar(0);
		cvGetCol(imgSrc, &data, i);
		val= cvSum(&data);
		if(val.val[0] < maxVal.val[0])
		{
			*max= i;
			if(!minFound)
			{
				*min= i;
				minFound= 1;
			}

		}
	}
}

/// <summary>
///     Finds min and max Y of the data present in given image.
/// </summary>
/// <params name="imsSrc">
///     Source image for which min and max Y has to be found.
/// </params>
/// <params name="min">
///     Int pointer where the min Y has to saved.
/// </params>
/// <params name="max">
///     Int pointer where the max Y has to saved.
/// </params>
/// <returns> Nothing. </returns>
void OCR::findY(IplImage* imgSrc,int* min, int* max)
{
	int i;
	int minFound=0;
	CvMat data;
	CvScalar maxVal=cvRealScalar(imgSrc->width * 255);
	CvScalar val=cvRealScalar(0);
	//For each col sum, if sum < width*255 then we find the min
	//then continue to end to search the max, if sum< width*255 then is new max
	for (i=0; i< imgSrc->height; i++)
	{
	    val = cvRealScalar(0);
		cvGetRow(imgSrc, &data, i);
		val= cvSum(&data);
		if(val.val[0] < maxVal.val[0])
		{
			*max=i;
			if(!minFound)
			{
				*min= i;
				minFound= 1;
			}
		}
	}
}

/// <summary>
///     Finds bounding-box of the data present in given image.
/// </summary>
/// <params name="imsSrc">
///     Source image for which bouding box has to be found.
/// </params>
/// <returns> Bounding box as CvRect. </returns>
CvRect OCR::findBB(IplImage* imgSrc)
{
	CvRect aux;
	int xmin, xmax, ymin, ymax;
	xmin=xmax=ymin=ymax=0;

	findX(imgSrc, &xmin, &xmax);
	findY(imgSrc, &ymin, &ymax);
	aux=cvRect(xmin, ymin, xmax-xmin, ymax-ymin);

	return aux;
}

/// <summary>
///     Given image, finds bounding box, resizes it to new_width and new_height, and if printResult is non-zero, prints result.
/// </summary>
/// <params name="imsSrc">
///     Source image which has to be processed.
/// </params>
/// <params name="new_width">
///     Width of the image to be returned.
/// </params>
/// <params name="new_height">
///     Height of the image to be returned.
/// </params>
/// <params name="printResult">
///     Indicates whether result has be printed, if its non-zero result are printed after running k-neares algorithm.
/// </params>
/// <returns> Returns the cropped image from original image measuring bounding box size, resized to new_width and new_height.</returns>
IplImage OCR::preprocessing(IplImage* imgSrc,int new_width, int new_height, int printResult)
{
	IplImage* result;
	IplImage* scaledResult;

	CvMat data;
	CvMat dataA;
	CvRect bb;//bounding box

	//Find bounding box
	bb=findBB(imgSrc);

	//Get bounding box data and no with aspect ratio, the x and y can be corrupted
	cvGetSubRect(imgSrc, &data, cvRect(bb.x, bb.y, bb.width, bb.height));

	//Create image with this data with width and height with aspect ratio 1
	//then we get highest size betwen width and height of our bounding box
	int size=(bb.width>bb.height)?bb.width:bb.height;
	result=cvCreateImage( cvSize( size, size ), 8, 1 );
	cvSet(result,CV_RGB(255,255,255),NULL);
	//Copy de data in center of image
	int x=(int)floor((float)(size-bb.width)/2.0f);
	int y=(int)floor((float)(size-bb.height)/2.0f);

	cvGetSubRect(result, &dataA, cvRect(x,y,bb.width, bb.height));
	cvCopy(&data, &dataA, NULL);

	//Scale result
	scaledResult=cvCreateImage( cvSize( new_width, new_height ), 8, 1 );
	cvResize(result, scaledResult, CV_INTER_NN);

	//Return processed data
	if(printResult == 1)
	{
	   print(*scaledResult, printResult);
	}

	return *scaledResult;
}

/// <summary>
///     Given image with paragraph of characters,
///     finds bounding box, resizes it to new_width and new_height, and if printResult is 1, prints result for each character.
/// </summary>
/// <params name="imsSrc">
///     Source image which has to be processed.
/// </params>
/// <params name="new_width">
///     Width of the image to be used for processing.
/// </params>
/// <params name="new_height">
///     Height of the image to be used for processing.
/// </params>
/// <params name="printResult">
///     Indicates whether result has be printed, if its 1, result are printed after running k-neares algorithm.
/// </params>
/// <params name="resultSize">
///     Number of resulting characters identified, size of the array to which result will be pointing to.
/// </params>
/// <returns> Pointer to array of result. </returns>
float* OCR::preprocessPara(IplImage* imgSrc, int new_width, int new_height, int printResult, int* resultSize)
{
	int minY, maxY;
    int i;
    int m=0;
    float ar[17];
	int minYFound=0;
	float result;
	vector<float> resultVector;
	float* resultPointer;
	CvMat data;
	CvScalar maxVal=cvRealScalar(imgSrc->width * 255);
	CvScalar val=cvRealScalar(0);
	//For each col sum, if sum < width*255 then we find the min
	//then continue to end to search the max, if sum< width*255 then is new max.
    for (i=0; i< imgSrc->height; i++)
    {
        cvGetRow(imgSrc, &data, i);
        val= cvSum(&data);
        if(val.val[0] < maxVal.val[0])
        { // some data is found!
            maxY = i;
            if(!minYFound)
            {
                minY = i;
                minYFound = 1;
            }
        }
        else if(minYFound == 1)
        {
            //some data was found previously, but current row 'i' doesn't have any data.
            //So process from row 'minY' till row maxY
            int j;
            int minX, maxX;
            int minXFound=0;
            //CvMat data;
            CvScalar maxValx=cvRealScalar((maxY - minY) * 255);
            CvScalar valx=cvRealScalar(0);
            //For each col sum, if sum < width*255 then we find the min
            //then continue to end to search the max, if sum< width*255 then is new max
            for (j=0; j< imgSrc->width - 1; j++)
            {
                valx=cvRealScalar(0);
                //instead of taking sum of entire column get sum of sub part of it.
                cvGetSubRect(imgSrc,&data, cvRect(j,minY,1,maxY-minY));
                //cvGetCol(imgSrc, &data, i);
                valx= cvSum(&data);
                if(valx.val[0] < maxValx.val[0])
                { //Some data found
                    maxX= j;
                    if(!minXFound)
                    {
                        minX= j;
                        minXFound= 1;
                    }
                }
                else if(minXFound == 1)
                {
                    int maxYp;
                    int minYp;
                    int minYpFound = 0;
                    CvScalar maxValyS = cvRealScalar((maxX-minX)*255);
                    CvScalar valyS = cvRealScalar(0);
                    // from minx to maxx and miny to maxy
                    for(int k = minY; k <= maxY; k++)
                    {
                        cvGetSubRect(imgSrc, &data, cvRect(minX, k, maxX-minX,1));
                        valyS = cvSum(&data);
                        if(valyS.val[0] - maxValyS.val[0])
                        {
                            maxYp = k;
                            if(minYpFound!=1)
                            {
                                minYp = k;
                                minYpFound = 1;
                            }
                        }
                    }
//                    for(int k=maxY-1; k >= minY; k--)
//                    {
//                        cvGetSubRect(imgSrc, &data, cvRect(minX, k, maxX-minX,1));
//                        valyS = cvSum(&data);
//                        if(valyS.val[0] < maxValyS.val[0])
//                        {
//                            maxYp = k+1;
//                            break;
//                        }
//                    }
                    //Some data was found previosly but current column 'j' doesn't have any data.
                    // so from minY to maxY and minX to maxX is the bounding box of character!
                    result = process(imgSrc, new_width, new_height, printResult, cvRect(minX, minYp, maxX-minX, maxYp-minYp));

                    //printf("\n");
                    ar[m] = result;
                        if(m>11)
                    {

                        if(m==17)
                        {

                         int tahunini=atoi(currentTahun().c_str());
                            int bulanini=atoi(currentBulan().c_str());
                            int tanggalini=atoi(currentTanggal().c_str());

                         int tanggal=(((int)ar[12])*10)+(int)ar[13];

                             printf("\n\ntanggal exp = %d\n", tanggal);

                         int bulan=(((int)ar[14])*10)+(int)ar[15];

                             printf("bulan exp = %d\n", bulan);

                         int tahun=2000+(((int)ar[m-1])*10)+(int)ar[m];

                             printf("tahun  exp= %d\n", tahun);
                        int hasil_tahunsama(int,int,int);
                        int hasil_tahunbeda(int,int,int,int,int,int);

                        int j=1,bulan_atas,bulan_bawah,bln_atas,bln_bawah,tahun_atas,tahun_bawah,tgl_atas,tgl_bawah,hsl;




                        //menempatkan nilai tertinggi apakah nilai 1 atau yang ke  2

                        selisih[0].date=tanggalini; //tanggal ke 1
                        selisih[1].date=tanggal; //tanggal ke 2
                        selisih[0].month=bulanini; // bulan ke 1
                        selisih[1].month=bulan; // bulan ke 2
                        selisih[0].year=tahunini; // tahun ke 1
                        selisih[1].year=tahun; // tahun ke 2

                        if(selisih[0].month>selisih[1].month){
                        bulan_atas=selisih[0].month;
                        bulan_bawah=selisih[1].month;
                        }else{
                        bulan_atas=selisih[1].month;
                        bulan_bawah=selisih[0].month;
                        }
                        if(selisih[0].year>selisih[1].year){
                        tahun_atas=selisih[0].year;
                        tgl_atas=selisih[0].date;
                        bln_atas=selisih[0].month;
                        tahun_bawah=selisih[1].year;
                        tgl_bawah=selisih[1].date;
                        bln_bawah=selisih[1].month;
                        }else{
                        tahun_atas=selisih[1].year;
                        tgl_atas=selisih[1].date;
                        bln_atas=selisih[1].month;
                        tahun_bawah=selisih[0].year;
                        tgl_bawah=selisih[0].date;
                        bln_bawah=selisih[0].month;
                        }

                        //memanggil fungsi rumus
                        if(selisih[0].year==selisih[1].year){

                        int hasilaja = hasil_tahunsama(bulan_bawah,bulan_atas,tahun_atas);

                        if(selisih[0].month<selisih[1].month)
                        {
                            hasilaja= hasilaja * (-1);
                        }
                        if((selisih[0].month<selisih[1].month) && (selisih[0].date<selisih[1].date))
                        {
                            hasilaja= hasilaja * (-1);
                        }

                        printf("Hasil selisihnya %d hari\n",hasilaja);

                        }else{
                        int hsl=hasil_tahunbeda(tahun_bawah,tahun_atas,bln_bawah,bln_atas,tgl_atas,tgl_bawah);
                        if(selisih[0].month<selisih[1].month)
                        {
                            hsl= hsl * (-1);
                        }
                        if((selisih[0].month<selisih[1].month) && (selisih[0].date<selisih[1].date))
                        {
                            hsl= hsl * (-1);
                        }
                        printf("\nSisa waktu sampai produk kadaluarsa %d hari\n",hsl);
                        }
                        }





                    }

                    m++;

                    resultVector.push_back(result); // after finding each result push the result to the vector.

//                    	CvPoint pt1,pt2;
//                    	pt1.x = minX;
//                    	pt1.y = minYp;
//                    	pt2.x = minX;
//                    	pt2.y = maxYp;
//                    	cvLine(imgSrc, pt1, pt2, CV_RGB(0, 0, 0));
//
//                    	pt1.x = maxX;
//                    	pt2.x = maxX;
//
//                        cvLine(imgSrc, pt1, pt2, CV_RGB(0, 0, 0));
//
//                        pt1.x = minX;
//                        pt1.y = minYp;
//                        pt2.x = maxX;
//                        pt2.y = minYp;
//
//                        cvLine(imgSrc, pt1, pt2, CV_RGB(0, 0, 0));
//
//                        pt1.y = maxYp;
//                        pt2.y = maxYp;
//                        cvLine(imgSrc, pt1, pt2, CV_RGB(0, 0, 0));
//
//                    	cvNamedWindow("scaled result", CV_WINDOW_AUTOSIZE);
//                        cvShowImage("scaled result",imgSrc);
//
//                        cvWaitKey(0);

                    minXFound = 0;
                }
            }

            minYFound = 0;
        }
    }// akhir for
	//If exit from loop was because max height was reached, but minFound has been set, then process from minFound till height.
	//This will not happen in the ideal examples I take :)
	*resultSize = resultVector.size();
	resultPointer = new float[*resultSize];
	int k;
	for(k = 0; k < *resultSize; k++)
	{
	    *(resultPointer+k) = resultVector[k];
	}

	return ar;
}

/// <summary>
///     Given image of single character and bounding box,
///     resizes it to new_width and new_height, and if printResult is 1, prints result after running k-nearest algorithm.
/// </summary>
/// <params name="imsSrc">
///     Source image which has to be processed.
/// </params>
/// <params name="new_width">
///     Width to which image has to be resized before running k-nearest algorithm in it.
/// </params>
/// <params name="new_height">
///     Height to which image has to be resized before running k-nearest algorithm in it.
/// </params>
/// <params name="printResult">
///     Indicates whether result has be printed, if its non-zero result are printed after running k-neares algorithm.
/// </params>
/// <returns> Result after classifying image. </returns>
float OCR::process(IplImage* imgSrc, int new_width, int new_height, int printResult, CvRect bb)
{
    int ar[17];
    int m;
    IplImage* result;
	IplImage* scaledResult;

	CvMat data;
	CvMat dataA;
	CvRect bba;//bounding box maintain aspect ratio.
	//Get bounding box data and no with aspect ratio, the x and y can be corrupted
	cvGetSubRect(imgSrc, &data, cvRect(bb.x, bb.y, bb.width, bb.height));
	//Create image with this data with width and height with aspect ratio 1
	//then we get highest size betwen width and height of our bounding box
	int size=(bb.width>bb.height)?bb.width:bb.height;
	result=cvCreateImage( cvSize( size, size ), 8, 1 );
	cvSet(result,CV_RGB(255,255,255),NULL);
	//Copy data to center of image
	int x=(int)floor((float)(size-bb.width)/2.0f);
	int y=(int)floor((float)(size-bb.height)/2.0f);

	//Get center of the result into dataA.
	cvGetSubRect(result, &dataA, cvRect(x,y,bb.width, bb.height));
	cvCopy(&data, &dataA, NULL);
	//Scale result
	scaledResult=cvCreateImage( cvSize( new_width, new_height ), 8, 1 );
	cvResize(result, scaledResult, CV_INTER_NN);
	//Return processed data
	ar[m] = print(*scaledResult, printResult);

	//printf("%d ", ar[m]);
	return ar[m++];
}
