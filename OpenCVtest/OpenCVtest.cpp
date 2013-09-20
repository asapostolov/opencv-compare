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
	float a;
	int i = 0;
	if(infile.is_open()){
		std::string line;
		while (std::getline(infile, line))
		{
			if(isStertRead==false && line.find("D:\\")!= std::string::npos){
				isStertRead = true;
				continue;
			}
			if (isStertRead == true && isEndRead == false){
				line = ReplaceComma(line);
				std::istringstream iss(line);
				if (!(iss >> a )) { 
					break; 
				} // error

				cv::Mat test2;
				cv::resize(test,test2,Size(1,i+1));
				test = test2;
				test.at<float>(i,0) = a;
				
				i++;
			}
		}
	}
	
	return test;
}

std::string string_format(const std::string fmt, ...) {
    int size = 100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
    return str;
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

void SaveFile(vector<FileInfo> files, string path, string folderName,  int method, string methodName){
	vector<vector<string>> table;

	vector<string> topRow;
	topRow.push_back("x\t");
	for(int i = 0; i < files.size();i++){
		FileInfo file = files[i];
		topRow.push_back(string_format("%d\t",file.FileNumber));
	}
	table.push_back(topRow);
	for(int i = 0; i < files.size(); i++){
		FileInfo filei = files[i];
		vector<string> rowi;
		rowi.push_back(string_format("%d\t",filei.FileNumber));
		for(int j = 0; j < files.size();j++){
			FileInfo filej = files[j];
			if( i > j )  {
				rowi.push_back("-\t");
				continue;
			}
			Mat mat1 = GetMatFromFile(filei.NewPath);
			Mat mat2 = GetMatFromFile(filej.NewPath);

			rowi.push_back(string_format("%f\t",compareHist( mat1, mat2, method )));
		}
		table.push_back(rowi);
	}
	std::ofstream fs(path+"\\.result."+methodName+".txt");
	fs<<folderName<<"\n";
	for(int i = 0; i < table.size();i++){
		vector<string> row = table[i];
		for(int j = 0; j < row.size();j++){
			string cell = row[j];
			
			fs<<cell;
		}
		fs<<"\n";
	}
}

/** @function main */
int main( int argc, char** argv )
{
	string path = "C:\\GitHub\\opencv-compare\\samples\\reflRatioSpectra";
	string innerFolderPath = "br_r";

	DIR *dir = opendir(path.c_str());

    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR){
           	if(!isSystemPath(entry->d_name)){
				string innerPath  = path +"\\"+ string(entry->d_name) + "\\" + innerFolderPath;
				
				DIR *innerDir = opendir(innerPath.c_str());
				struct dirent *innerEntry = readdir(innerDir);
				printf("Processing %s ....\n\n", entry->d_name);
				int i = 0;
				vector<FileInfo> files;
				
				while (innerEntry != NULL)
				{
					if(!isSystemPath(innerEntry->d_name)){
						

						FileInfo info = FileInfo();
						info.FileName = string(innerEntry->d_name);
						info.FileNumber = i;
						info.FilePath = innerPath + "\\" + info.FileName;
						info.NewPath = innerPath + "\\" + to_string(info.FileNumber) + ".txt";

						files.push_back(info);
						i++;
					}
					innerEntry = readdir(innerDir);
				}
				string folderName = string(entry->d_name);
				std::ofstream fs(innerPath+"\\.result..legend.txt");
				fs<<folderName<<"\n";
				for(int i = 0; i < files.size();i++){
					FileInfo file = files[i];
					rename(file.FilePath.c_str(), file.NewPath.c_str());
					fs<<file.FileNumber<<" -- "<<file.FileName<<"\n";
				}
				fs<<"\n";

				SaveFile(files,innerPath,folderName,0,"Correlation");
				SaveFile(files,innerPath,folderName,1,"Chi-square");
				SaveFile(files,innerPath,folderName,2,"Intersection");
				SaveFile(files,innerPath,folderName,3,"Bhattacharyya");
			}
		}

        entry = readdir(dir);
    }
	
  printf( "Done \n" );
  system("pause");
 }
