
#include "fileReader.h"

using namespace std;


///[OLD]Reads the first .rgb-file in the default folder and returns a pointer to an image represented as a two dimensional array if a .rgb-file is found. 
///[OLD]Returns NULL if a .rgb-file is not detected.

///Reads the rgb-file from the path.
image *readImageFile(string path)
{

	string pathRev = reverseString(path);
	string date = reverseString(pathRev.substr(15,19));
	int width = atoi(reverseString(pathRev.substr(9,4)).c_str());  //Template YYYY_MM_DD'HH_MM'SS_UU'wWWWWhHHHH.rgb where WWWW is width and HHHH is height.
	int height = atoi(reverseString(pathRev.substr(4,4)).c_str());
	
	rgb8 **p = new rgb8*[width];
	for(int k=0;k<width;k++)
	{
		p[k] = new rgb8[height];
	}

	ifstream myFile;
	myFile.open(path, ios::in | ios::binary | ios::ate);

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


///Reads the next rgb-file in the folder pointed by path.
/*image *readNextImageFile(string path)
{
	string fileName;
	

	DIR *dir;
	class dirent *ent;

	dir = opendir(path.c_str());
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

	return readImageFile(path+fileName);
	
}
*/
void writeImagePPM(string path, image *img, fileType type)
{
	if(img->pixel==nullptr||img->height<1||img->width<1)
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
	string fileName = img->date + "-w" + to_string(img->width) + "h" + to_string(img->height) + fileEnding;

	ofstream file;
	file.open(path+fileName, ios::binary);
	int size = img->width*img->height*3;
	char *memblock = new char[size];

	for(int i=0;i<img->height;i++)
	{
		int offset = i*img->width*3;
		for(int j=0;j<img->width;j++)
		{
			int n = offset+j*3;
			memblock[n+0] = img->pixel[j][i].red;
			memblock[n+1] = img->pixel[j][i].green;
			memblock[n+2] = img->pixel[j][i].blue;
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

/*
int main()
{
	image *img;

	//img=readNextImageFile("Files\\");
	img=readImageFile("Input\\2014_02_18-13_32-21-w2592h1936.rgb");

	invertFilter(img);

	fileType outType = PPM;
	writeImagePPM("Output\\",img,outType);

	delete img;

	//system("pause");
	return 0;
}*/