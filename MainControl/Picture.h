#pragma once
class Picture
{
public:
	Picture(CImage* pimage);

	~Picture();
	int ID;
	CImage* pCImage;
	void SaveImage();
};

