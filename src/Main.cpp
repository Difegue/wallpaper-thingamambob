#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <conio.h> 
#include <time.h> 
#include <direct.h>
#include "ImageFetching.h"
#define cimg_use_png
#define cimg_use_jpeg
#include "CImg.h"

using namespace cimg_library;

struct ScreenResolution
{
	int width;
	int height;
};

int main(){

	srand (time(NULL));

	std::cout << "Fetching directories from Folders.txt..." << std::endl;
	std::vector<std::string> filenames = FetchDirectories("Folders.txt"); //Path of the .exe + Folders.txt, the file we want.

	//read time and resolutions from Timer.txt
	//Fetch two filenames, stitch them with cimg, use winAPI to set as wallpaper
	//sleep(int x) to wait until next wp change. x in seconds.
	std::fstream infile("Timer.txt");
	std::string output;
	std::vector<ScreenResolution> Screens;

	int taimu;
	int numb(0);
	bool norepeat; //Do we repeat previously displayed wallpapers, or not?

	//Get main monitor resolution from winAPI
//x = GetSystemMetrics(0);
//y = GetSystemMetrics(1);
/*
Get resolution for multiple monitors from winAPI:
MONITORINFO monitor;
monitor.cbSize=sizeof ( MONITORINFO ); //Create monitorinfo instance

GetMonitorInfo(,*monitor);

*/


	while (std::getline(infile, output)) //Store each line into directoryPath and operate on it
	{
		do
		{ 
			if (output!="endmonitor")
			{
				ScreenResolution hue;
				std::cout << "\nReading Monitor data...\n";

				//get width
				hue.width=std::stoi(output);
				std::cout << "Width:" << output << std::endl;

				//get height
				std::getline(infile, output); 
				hue.height=std::stoi(output);
				std::cout << "Height:" << output << std::endl;

				Screens.push_back(hue);
				std::getline(infile, output);
			}
		}
		while (output!="endmonitor");

		std::getline(infile, output);
		norepeat = std::stoi(output);
		if (norepeat)
			std::cout << "Wallpaper No-repeat mode has been Activated!\n";
		else
			std::cout << "Wallpaper No-repeat mode is Disabled.\n";

		std::getline(infile, output);
		taimu = std::stoi(output);
		std::cout << "Timer is:" << output << std::endl;
		
		infile.close();
	}

	output = "";

	//Grab the wallpaper history.
	std::vector<std::string> history;
	if (norepeat)
	{
		infile.open("History.txt");
		do
		{
			std::getline(infile, output);
			if (output != "")
				history.push_back(output);
		} while (!infile.eof());
		infile.close();
	}

	//BEGIN LOOP HERE
	do
	{
		//Selecting wallpapers

		std::vector<std::string> wps_filenames;
		std::vector<CImg<float>> wps;
		if (norepeat)
		std::fstream infile("History.txt");

		for(int i=0; i<Screens.size(); i++ )
		{

			bool duplicheck(false);
			srand (time(NULL));
			
			if (filenames.size() == 0)
			{
				std::cout << "\nNo files in the folders you specified!\nThat, or the file-fetching function failed. \nPress Enter to exit.\n";
				std::cin.ignore();
				return 1337;
			}
			else
				numb = rand() % filenames.size(); //reroll
			//std::cout << "Selected " << filenames[numb] << std::endl;

			//prevent getting twice the same wp, if possible.
			if (filenames.size() >= Screens.size())
			{
			
				do
				{
					
					duplicheck=false;

					for (int j=0; j<wps_filenames.size();j++)
						if (wps_filenames[j]==filenames[numb])
						{
							//std::cout << "Identical wallpapers detected! Reselecting."<< std::endl;
							srand (time(NULL));
							numb = rand() % filenames.size(); //reroll
							duplicheck =true;
							
						}

				} while (duplicheck==true);



				//Now, check if the name we selected is already in history.
				if (norepeat)
				do
				{

					//We have a wallpaper, let's check if we haven't already used it last time.
					if (filenames.size() == history.size())
						//All the wallpapers have been used, wipe history.
					{
						infile.close();
						remove("History.txt");
						history.clear();
						infile.open("History.txt");
					}

					duplicheck = false;

					for (int j = 0; j<history.size(); j++)
					if (history[j] == filenames[numb])
					{
						//std::cout << "Selected " << filenames[numb] << std::endl;
						//std::cout << "Wallpaper was already used! Reselecting."<< std::endl;
						srand(time(NULL));
						numb = rand() % filenames.size(); //reroll
						duplicheck = true;

					}

				} while (duplicheck == true);
			}
		
		wps_filenames.push_back(filenames[numb]);

		std::string junk;
		if (norepeat)
		history.push_back(filenames[numb]); //Write the wallpaper used inside the history.

		
		}
		if (norepeat)
		{
			for (int j = 0; j < history.size(); j++) //Write history to file.
				infile << history[j] + "\n";
			infile.close();
		}

		for (int j=0; j<wps_filenames.size();j++)
		{
			std::cout << "Selected " << wps_filenames[j] << std::endl;
			wps.push_back(CImg<float>(string2CA(wps_filenames[j]))); //add wallpaper
		}

		//resizing each wallpaper to the corresponding screen.
		for(int i=0; i<Screens.size(); i++ )
		{
			int test = wps[i].width();
			float ratioMonitor = (float)Screens[i].width/(float)Screens[i].height; //cast to float for proper division.
			float ratioWall = (float)wps[i].width()/(float)wps[i].height();
			if (compFloat(ratioWall, ratioMonitor, 0.05)) //compare
			{
				wps[i].resize(Screens[i].width,Screens[i].height,-100,-100,6);
			}
			else
				if (Screens[i].width*Screens[i].height < wps[i].width()*wps[i].height())
				{
					//Case 1: Wallpaper larger than screen.
					//Take the attribute with the least difference between wp and screen(width or height) 
					//and resize through proportionality.

					if (wps[i].width()-Screens[i].width<wps[i].height()-Screens[i].height)
					{
						int newheight = (Screens[i].width*wps[i].height())/wps[i].width();
						//In the case of square pictures being used, it's possible for the resulting picture to not fully cover the screen resolution.
						//We check for this occurence and change the resizing formula in consequence.
						if (newheight<Screens[i].height)
							wps[i].resize(Screens[i].width,Screens[i].height*Screens[i].width/newheight,Screens[i].height,-100,-100,6);
						else
							wps[i].resize(Screens[i].width,newheight,-100,-100,6);
						//Crop the extra pixels in the image' so that it fits the screen.
						newheight =(wps[i].height()-Screens[i].height)/2; 
						//wps[i].height is the resized image's height, as resize operates directly on the image.

						wps[i].crop(0,newheight,Screens[i].width,newheight+Screens[i].height);
					}
					else
					{
						int newwidth = (wps[i].width()*Screens[i].height)/wps[i].height();
						//In the case of square pictures being used, it's possible for the resulting picture to not fully cover the screen resolution.
						//We check for this occurence and change the resizing formula in consequence.
						if (newwidth<Screens[i].width)
							wps[i].resize(Screens[i].width,Screens[i].height*Screens[i].width/newwidth,-100,-100,6); //magic!
						else
							wps[i].resize(newwidth,Screens[i].height,-100,-100,6);
						//Crop the extra pixels in the image' so that it fits the screen.
						newwidth =(wps[i].width()-Screens[i].width)/2;
						//wps[i].width is the resized image's width, as resize operates directly on the image.
						wps[i].crop(newwidth,0,newwidth+Screens[i].width,Screens[i].height);
					}

				}
				else
				{
					//Case 2: Screen larger than wallpaper.
					//Take the attribute with the most difference between wp and screen(width or height) 
					//and resize through proportionality.
					//Simply put, reversal.

					if (wps[i].width()-Screens[i].width>wps[i].height()-Screens[i].height)
					{
						wps[i].resize(Screens[i].width,(Screens[i].width*wps[i].height())/wps[i].width(),-100,-100,6);
						int newheight =(wps[i].height()-Screens[i].height)/2; 
						//wps[i].height is the resized image's height, as resize operates directly on the image.
						wps[i].crop(0,newheight,Screens[i].width,newheight+Screens[i].height);
					}
					else
					{
						wps[i].resize((wps[i].width()*Screens[i].height)/wps[i].height(),Screens[i].height,-100,-100,6);
						int newwidth =(wps[i].width()-Screens[i].width)/2;
						//wps[i].width is the resized image's width, as resize operates directly on the image.
						wps[i].crop(newwidth,0,newwidth+Screens[i].width,Screens[i].height);
					}
				}
		}


		//create new image which is a combination of all wallpapers.
		//To make a multiscreen wallpaper windows will eat properly, we stitch all our previously rendered images.
		int totalwidth(0), maxheight(0);

		for(int i=0; i<wps.size(); i++ )
		{
			totalwidth = totalwidth+wps[i].width();
			if (wps[i].height() > maxheight) maxheight = wps[i].height();
		}

		CImg<float> FinalWP=wps[0];
		FinalWP.resize(totalwidth, maxheight);
		int widthstep=0;
		for(int i=0; i<wps.size(); i++ )
		{
			FinalWP.draw_image(widthstep,(maxheight-wps[i].height()),wps[i]);
			widthstep=widthstep+wps[i].width();
		}

		//bullshit windows things to get a temp folder
		TCHAR temp_file[265];
		GetTempPath(255, temp_file);
		lstrcat(temp_file, L"wp.bmp");

		std::wstring pathf = temp_file;
		std::cout << "Saving to "+ws2s(pathf)+"\n";

		//save it as .bmp for easy use by winAPI.
		FinalWP.save(ws2s(temp_file).c_str());

		//call winAPI, set as wallpaper!

		bool result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, const_cast<LPWSTR>(pathf.c_str()), SPIF_UPDATEINIFILE);

		//for W8 and up:

		/*HRESULT hr = CoInitialize(nullptr);
		IDesktopWallpaper *pDesktopWallpaper = nullptr;
		hr = CoCreateInstance(__uuidof(DesktopWallpaper), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pDesktopWallpaper));
		hr = pDesktopWallpaper->SetWallpaper(nullptr, const_cast<LPWSTR>(pathf.c_str()));

		if (!FAILED(hr)) std::cout << "Wallpaper set\n"; else std::cout << "Error! Wallpaper not set\n";*/

		if (result) std::cout << "Wallpaper set\n"; else std::cout << "Error! Wallpaper not set\n";
		
		//sleep, and redo.
		std::cout << "Next Wallpaper change in " << taimu << " seconds.\n";
		std::cout << "Press any key to change wallpapers now!\n";
		for(int i=0; i<wps.size(); i++ )
		{
			~wps[i]; //cleanup.
		}
		~FinalWP; //cleanup.

		//Black magic inbound.

		int taimu_elapsed(0);
		char kp();

		do
		{
			
			taimu_elapsed++;
			Sleep(1); //winAPI sleep takes milliseconds -> time in seconds * 10^3.
			//std::cout << taimu_elapsed << std::endl;

		}
		while (taimu_elapsed<taimu*1000 && _kbhit() == 0 );
		
		while (kbhit()) getch();         //clear buffer: avoid 500 wallpaper reselections in one keystroke.
			fflush (stdin) ;         // clear stdin's buffer
		//Sleep(taimu*1000); 
	}
	while (1);
	//END LOOP HERE

	return 0;
}
