
#include <string>


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

	image(string date, int width, int height, rgb8 **p);

	~image();

};
