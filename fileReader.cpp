#include <iostream>
#include <fstream>
#include <string>

#include <sys/types.h>
#include "dirent.h"

#ifdef _WIN32
#include <Windows.h>
#define WINDOWS true
#endif
/*
#else
#ifdef

#include "dirent.h"
#define WINDOWS false
#endif
#endif
*/

using namespace std;

struct rgb8 
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

class image
{
	public:
	string date;
	int width;
	int height;
	rgb8 **p;

	image(string date, int width, int height, rgb8 **p)
	{
		this->date=date;
		this->width=width;
		this->height=height;
		this->p=p;
	}

	~image()
	{
		for(int i=0;i<width;i++)
		{
			delete p[i];
		}
		delete p;
	}

};

enum fileType
{
	RGB,PPM 
};

///Reads the first .rgb-file in the default folder and returns a pointer to an image represented as a two dimensional array if a .rgb-file is found. 
///Returns NULL if a .rgb-file is not detected.
image *readImageFile() 
{
	string fileName;
	
	DIR *dir;
	class dirent *ent;

	dir = opendir("Files\\");
	ent = readdir(dir);

	bool found = false;
	while(ent != NULL&&!found)
	{
		string file = ent->d_name;

		int finding = file.find(".rgb");
		if(finding != string::npos)
		{
			fileName = file;
			found = true;
		}
		else
		{
			ent = readdir(dir);
		}
	}
	closedir(dir);
	if(ent == NULL)
	{
		return NULL;
	}

	string date = fileName.substr(0,22);
	int width = atoi(fileName.substr(24,4).c_str());  //Template YYYY_MM_DD'HH_MM'SS_UU'wWWWW'hHHHH.rgb where WWWW is width and HHHH is height.
	int height = atoi(fileName.substr(29,4).c_str());


	rgb8 **p = new rgb8*[width];
	for(int k=0;k<width;k++)
	{
		p[k] = new rgb8[height];
	}

	ifstream myFile;
	myFile.open("Files\\"+fileName, ios::in | ios::binary | ios::ate);

	streampos size;
	char *memblock;
	
	if(myFile.is_open())
	{
		size = myFile.tellg();
		memblock = new char[size];
		myFile.seekg(0,ios::beg);
		myFile.read(memblock,size);
		myFile.close();

		for(int i=0;i<height;i++)
		{
			int offset = i*width*3;
			for(int j=0;j<width;j++)
			{
				int n = offset+j*3;
				
				p[j][i].red = memblock[n+0];
				p[j][i].green = memblock[n+1];
				p[j][i].blue = memblock[n+2];
			}
		}
		delete[] memblock;
	}
	return new image(date,width,height,p);
}

void writeImagePPM(image *img, fileType type)
{
	if(img->p==nullptr||img->height<1||img->width<1)
	{
		cout<<"Image parameters where not correct."<<endl;
		return;
	}

	string fileEnding;
	switch(type)
	{
		case PPM:
			fileEnding = ".ppm";
			break;
		case RGB:
			fileEnding = ".rgb";
			break;
	}

	//Template YYYY_MM_DD'HH_MM'SS_UU'wWWWW'hHHHH.ppm where WWWW is width and HHHH is height.
	string fileName = img->date + "'w" + to_string(img->width) + "h" + to_string(img->height) + fileEnding;

	ofstream file;
	file.open("Files\\"+fileName, ios::binary);
	int size = img->width*img->height*3;
	char *memblock = new char[size];

	for(int i=0;i<img->height;i++)
	{
		int offset = i*img->width*3;
		for(int j=0;j<img->width;j++)
		{
			int n = offset+j*3;
			memblock[n+0] = img->p[j][i].red;
			memblock[n+1] = img->p[j][i].green;
			memblock[n+2] = img->p[j][i].blue;
		}
	}
	
	if(type==PPM)
	{
		string ppmHeader = "P6 "+to_string(img->width)+" "+ to_string(img->height) +" 255";
		char *header = new char[ppmHeader.length()+1]; 
		strcpy(header,ppmHeader.c_str());
		header[ppmHeader.length()]=' ';
	
		file.write(header, ppmHeader.length()+1);
		delete header;
	}
	file.write(memblock, size);
	file.close();

	delete[] memblock;
}

void binaryFilter(image *img)
{
	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			img->p[i][j].red = img->p[i][j].green = img->p[i][j].blue = (img->p[i][j].red + img->p[i][j].green + img->p[i][j].blue)/3;
		}
	}
}

void invertFilter(image *img)
{
	for(int i=0;i<img->width;i++)
	{
		for(int j=0;j<img->height;j++)
		{
			img->p[i][j].red = 255-img->p[i][j].red;
			img->p[i][j].green = 255-img->p[i][j].green;
			img->p[i][j].blue = 255-img->p[i][j].blue;
		}
	}
}

int main()
{
	image *img;
	
	
	img = readImageFile();
	if(img == NULL)
	{
		cout<<"No relevant file was found."<<endl;
		system("pause");
		return -1;
	}

	invertFilter(img);

	fileType outType = PPM;
	writeImagePPM(img,outType);

	delete img;
	system("pause");
	return 0;
}