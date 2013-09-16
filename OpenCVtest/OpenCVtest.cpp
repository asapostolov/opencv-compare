#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>


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

bool isSystemPath(char* text){
	return string(text)== "." || string(text)=="..";
}

class FileInfo{
public: string FileName;
public: int FileNumber;
public: string FilePath;
public: string NewPath;
};

/** @function main */
int main( int argc, char** argv )
{
	string path = "C:\\GitHub\\opencv-compare\\samples\\FluoRatioSpectra";
	string innerFolderPath = "b2_r";

	DIR *dir = opendir(path.c_str());

    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR){
            printf("%s\n", entry->d_name);
			if(!isSystemPath(entry->d_name)){
				string innerPath  = path +"\\"+ string(entry->d_name) + "\\" + innerFolderPath;
				DIR *innerDir = opendir(innerPath.c_str());

				struct dirent *innerEntry = readdir(innerDir);
				//tFileInfo files[5];
				int i = 0;
				vector<FileInfo> files;
				while (innerEntry != NULL)
				{
					if(!isSystemPath(innerEntry->d_name)){
						printf("\t%s\n", innerEntry->d_name);

						FileInfo info = FileInfo();
						info.FileName = string(innerEntry->d_name);
						info.FileNumber = i;
						info.FilePath = innerPath + "\\" + info.FileName;
						info.NewPath = innerPath + "\\" + to_string(info.FileNumber) + ".txt";

						//files.resize(i+1);
						files.push_back(info);
						i++;
					}

					innerEntry = readdir(innerDir);
				}

				for(int i = 0; i < files.size();i++){
					FileInfo file = files[i];

					rename(file.FilePath.c_str(), file.NewPath.c_str());
				}
			}
		}

        entry = readdir(dir);
    }

    closedir(dir);

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
