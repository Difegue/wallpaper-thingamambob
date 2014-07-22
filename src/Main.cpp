#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include "ImageFetching.h"
#include "CImg.h"
using namespace cimg_library;

#undef min
#undef max

struct ScreenResolution
{
	int width;
	int height;
};

char* string2CA(std::string& s) //Converts std::string to array of char.
{
	char *a=new char[s.size()+1];
	a[s.size()]=0;
	memcpy(a,s.c_str(),s.size());
	return a;
}

bool compFloat (float A, float B, float EPSILON) 
{
   float diff = A - B;
   return (diff < EPSILON) && (-diff < EPSILON);
}

int main(){

	vector<wstring> filenames = FetchDirectories("Folders.txt");

	//read time and resolutions from Timer.txt
	//Fetch two filenames, stitch them with cimg, use winAPI to set as wallpaper
	//sleep(int x) to wait until next wp change. x in seconds.
	std::ifstream infile("Timer.txt");
	std::string output;
	vector<ScreenResolution> Screens;

	while (std::getline(infile, output)) //Store each line into directoryPath and operate on it
	{
		do
		{ //get width
			if (output!="endmonitor")
			{
				ScreenResolution hue;
				hue.width=std::stoi(output);
				std::getline(infile, output); //get height
				hue.height=std::stoi(output);

				Screens.push_back(hue);
			}
		}
		while (output!="endmonitor");

		std::getline(infile, output);
		std::string time = output;

	}

	//BEGIN LOOP HERE
	//Selecting wallpapers

	vector<CImg<unsigned char>> wps;

	for(int i=0; i<=sizeof(Screens); i++ )
	{
	int numb = rand() % sizeof(filenames); //reroll
	wps.push_back(CImg<unsigned char>(string2CA(ws2s(filenames[numb])))); //add wallpaper
	}

	//resizing each wallpaper to the corresponding screen.
	for(int i=0; i<=sizeof(Screens); i++ )
	{
		float ratioMonitor = Screens[i].width/Screens[i].height;
		float ratioWall = wps[i].width()/(wps[i]).height();

		if (compFloat(ratioWall, ratioMonitor, 0.01)) //comparer
		{
			wps[i].resize(Screens[i].width,Screens[i].height);
		}
		else
			if (ratioWall < ratioMonitor)
			{

			}
			else
			{


			}

	}

	//create new image which is a combination of all wallpapers.

	//call winAPI, set as wallpaper!

	//sleep, and redo.
	//END LOOP HERE

	return 0;
}






//Get main monitor resolution from winAPI
//x = GetSystemMetrics(0);
//y = GetSystemMetrics(1);
/*
Get resolution for multiple monitors from winAPI:
MONITORINFO monitor;
monitor.cbSize=sizeof ( MONITORINFO ); //Create monitorinfo instance

GetMonitorInfo(,*monitor);

*/
