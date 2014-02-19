

#include <string>

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

	image(rgb8 **p)
	{
		date="NULL";
		width=3;
		height=3;
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