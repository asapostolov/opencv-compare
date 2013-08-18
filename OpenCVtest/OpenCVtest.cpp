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

Mat GetMatFromFile(string filePath){
	cv::Mat test(1,2,DataType<float>::type);
	std::ifstream infile;

	infile.open(filePath);

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
	
	return test;
}

/** @function main */
int main( int argc, char** argv )
{
 	Mat mat1 = GetMatFromFile("..\\samples\\b1.txt");
	Mat mat2 = GetMatFromFile("..\\samples\\b2.txt");

  /// Apply the histogram comparison methods
  for( int i = 0; i < 4; i++ )
     { int compare_method = i;
		double self = compareHist( mat1, mat1, compare_method );
		double comp = compareHist( mat1, mat2, compare_method );

       printf( " Method [%d] Compare : %f, %f \n", i, self, comp );
     }

  printf( "Done \n" );
  system ("pause");
 }
