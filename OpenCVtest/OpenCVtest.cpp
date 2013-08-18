

#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;
using namespace cv;


string ReplaceComma(string line){
	string search_string =",";
	int spot = line.find(search_string);
	if(spot >= 0)
	{
		string tmpstring = line.substr(0,spot);
		tmpstring += ".";
		tmpstring += line.substr(spot+search_string.length(), line.length());
		line = tmpstring;
	}
	spot = line.find(search_string);
	if(spot >= 0)
	{
		return ReplaceComma(line);
	}
	return line;
 }

/** @function main */
int main( int argc, char** argv )
{
 // cvLoadImage("D:\\funny.jpg");

  argv[1] = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Desert.jpg";
 // argv[2] = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Hydrangeas.jpg";
 // argv[3] = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Koala.jpg";

 //  Mat src_base, hsv_base;
 // Mat src_test1, hsv_test1;
 // Mat src_test2, hsv_test2;
 // Mat hsv_half_down;

 // /// Load three images with different environment settings
 // /*if( argc < 4 )
 //   { printf("** Error. Usage: ./compareHist_Demo <image_settings0> <image_setting1> <image_settings2>\n");
 //     return -1;
 //   }*/

 // src_base = imread( argv[1], 1 );
 // src_test1 = imread( argv[2], 1 );
 // src_test2 = imread( argv[3], 1 );

 // /// Convert to HSV
 // cvtColor( src_base, hsv_base, CV_BGR2HSV );
 // cvtColor( src_test1, hsv_test1, CV_BGR2HSV );
 // cvtColor( src_test2, hsv_test2, CV_BGR2HSV );

 // hsv_half_down = hsv_base( Range( hsv_base.rows/2, hsv_base.rows - 1 ), Range( 0, hsv_base.cols - 1 ) );

 // /// Using 30 bins for hue and 32 for saturation
 // int h_bins = 150; int s_bins = 2;
 // int histSize[] = { h_bins, s_bins };

 // // hue varies from 0 to 256, saturation from 0 to 180
 // float h_ranges[] = { 0, 256 };
 // float s_ranges[] = { 0, 180 };

 // const float* ranges[] = { h_ranges, s_ranges };

 // // Use the o-th and 1-st channels
 // int channels[] = { 0, 1 };

 // /// Histograms
 // MatND hist_base;
 // MatND hist_half_down;
 // MatND hist_test1;
 // MatND hist_test2;

 // /// Calculate the histograms for the HSV images
 // calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
 // normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

 // calcHist( &hsv_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false );
 // normalize( hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat() );

 // calcHist( &hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false );
 // normalize( hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat() );

 // calcHist( &hsv_test2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false );
 // normalize( hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat() );
 // 
 // 
 // cv::Mat my_histogram;
 // cv::FileStorage fs("C:\\GitHub\\opencv-compare\\samples\\my_histogram_file.yml", cv::FileStorage::WRITE);
	//if (!fs.isOpened()) 
	//{
	//	std::cout << "unable to open file storage!" << std::endl; 
	//	//return;
	//}
	//fs << "my_histogram" << hist_base;
	//fs.release();
 // 
	//cv::FileStorage fs2("test.yml", FileStorage::READ);


	///// Apply the histogram comparison methods
 // for( int i = 0; i < 4; i++ )
 //    { int compare_method = i;
 //      double base_base = compareHist( hist_base, hist_base, compare_method );
 //      double base_half = compareHist( hist_base, hist_half_down, compare_method );
 //      double base_test1 = compareHist( hist_base, hist_test1, compare_method );
 //      double base_test2 = compareHist( hist_base, hist_test2, compare_method );

 //      printf( " Method [%d] Perfect, Base-Half, Base-Test(1), Base-Test(2) : %f, %f, %f, %f \n", i, base_base, base_half , base_test1, base_test2 );
 //    }

 // printf( "Done \n" );
 // 
 // system ("pause");
 //      
 // return 0;

  
   Mat src, hsv;
    if(!(src=imread(argv[1], 1)).data )
        return -1;

    cvtColor(src, hsv, CV_BGR2HSV);

    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 150, sbins = 2;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    calcHist( &hsv, 1, channels, Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
    double maxVal=0;
	normalize( hsv, hsv, 0, 1, NORM_MINMAX, -1, Mat() );

	
    //minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 5;
    Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle( histImg, Point(h*scale, s*scale),
                        Point( (h+1)*scale - 1, (s+1)*scale - 1),
                        Scalar::all(intensity),
                        CV_FILLED );
        }

    namedWindow( "Source", 1 );
    imshow( "Source", src );

    namedWindow( "H-S Histogram", 1 );
    imshow( "H-S Histogram", histImg );

	cv::Mat test(1,2,DataType<float>::type);

	/*test.at<float>(10,0) = 42.33;
	
	float as = test.at<float>(10, 1);
*/

	std::ifstream infile;

	infile.open("C:\\GitHub\\opencv-compare\\samples\\b1.txt");

	bool isStertRead = false, isEndRead = false;
	float a, b;
	int i = 0;
	if(infile.is_open()){
		std::string line;
		while (std::getline(infile, line))
		{
			if(isStertRead==false && line.find(">>>>>Begin Processed Spectral Data<<<<<")!= std::string::npos){
				isStertRead = true;
				continue;
			}
			if (isStertRead == true && isEndRead == false){
				if(isEndRead==false && line.find(">>>>>End Processed Spectral Data<<<<<")!= std::string::npos){
					isEndRead = true;
					continue;
				}
				line = ReplaceComma(line);
				std::istringstream iss(line);
				//int a, b;
				if (!(iss >> a >> b)) { 
					break; 
				} // error

				cv::Mat test2;
				cv::resize(test,test2,Size(2,i+1));
				test = test2;
					//create(i+1,2,DataType<float>::type);
				test.at<float>(i,0) = a;
				test.at<float>(i,1) = b;
				i++;
				// process pair (a,b)
			}
		}
	}

	cv::Mat my_histogram;
    cv::FileStorage fs("C:\\GitHub\\opencv-compare\\samples\\my_histogram_file.yml", cv::FileStorage::WRITE);
	if (!fs.isOpened()) 
	{
		std::cout << "unable to open file storage!" << std::endl; 
		return 0;
	}
	fs << "my_histogram" << test;
	fs.release();
    //waitKey();
 }
