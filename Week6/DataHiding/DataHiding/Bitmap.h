#ifndef _BITMAP_H_
#define _BITMAP_H_
#include <string>
class Bitmap
{
private:
	
	int DataOffset();
public:
	unsigned char* data;
	//Bitmap(const string& data);
	Bitmap(const std::string& data);
	~Bitmap();
	bool isBitmap();
	int BitPerPixel();
	//true if can use this file to store data
	bool isGood();
	int Width();
	int Height();
	unsigned char* operator()(const int& i, const int& j);
};
Bitmap::Bitmap(const std::string& data)
{
	this->data = new unsigned char[data.length()];
	memcpy(this->data, data.c_str(), data.length());
}
Bitmap::~Bitmap()
{
	if (data != NULL)
		delete[]data;
}
int Bitmap::BitPerPixel()
{
	return *(unsigned __int16*)(&data[28]);
}

bool Bitmap::isGood()
{
	return (this->isBitmap() && this->BitPerPixel() == 24);
}
int Bitmap::DataOffset()
{
	return *((int*)(data + 10));
}
bool Bitmap::isBitmap()
{
	return char(data[0]) == 'B' && char(data[1]) == 'M';
}

int Bitmap::Width()
{
	return *((int*)(data + 18));
}

int Bitmap::Height()
{
	return *((int*)(data + 22));
}

unsigned char* Bitmap::operator()(const int& i, const int& j)
{
	int bpp = this->BitPerPixel();
	int rowSize = this->Width() * bpp / 8;
	if (rowSize & 3)
		rowSize += 4 - (rowSize & 3);
	

	int off = this->DataOffset() ;
	//off += i*rowSize + bpp*j;
	return &data[this->DataOffset() + i*rowSize + bpp / 8 * j];
	//return row;
}



#endif