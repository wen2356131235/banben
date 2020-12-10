#include "pch.h"
#include "Picture.h"

Picture::Picture(CImage* pimage)
{
	pCImage = pimage;
	ID = -1;
}
Picture::~Picture()
{
	if(pCImage)
	delete pCImage;
}

void Picture::SaveImage()
{
	CString path;
	path.Format(".\\����ͼƬ\\%06d.png",ID);
	pCImage->Save(path);
}
