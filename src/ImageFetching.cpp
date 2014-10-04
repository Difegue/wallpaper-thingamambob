#include "ImageFetching.h"

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

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string r(buf);
	delete[] buf;
	return r;
}

std::vector<std::string> FetchDirectories(std::string text){ 

	std::cout << "Beginning fetching..." << std::endl;
	//Fetching images
	std::vector<std::string> filenames; //vector of all filenames
	std::string directoryPath; 

	//Opening text file containing folders
	std::ifstream infile(text);

	while (getline(infile, directoryPath)) //Store each line into directoryPath and operate on it
	{
		WIN32_FIND_DATA FindFileData; 
		HANDLE hFind = INVALID_HANDLE_VALUE;

		hFind = FindFirstFile(s2ws(directoryPath+"\*").c_str(), &FindFileData);

		if(hFind  != INVALID_HANDLE_VALUE)
		{
			do
			{
				//if(FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				//{
					if (ws2s(FindFileData.cFileName)!="." && ws2s(FindFileData.cFileName)!=".." && ws2s(FindFileData.cFileName)!="Thumbs.db") //awful workaround
					{
						filenames.push_back(directoryPath+ws2s(FindFileData.cFileName));
						std::cout << "Adding " << ws2s(FindFileData.cFileName) << std::endl;
					}
				//}
			} while(FindNextFile(hFind, &FindFileData));

			FindClose(hFind);
		}
	
		
	}
return filenames;
};