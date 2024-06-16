#ifndef _PHOTO_MOSAIC_H_
#define _PHOTO_MOSAIC_H_

#include"data_loader.h"
#include"rgb_image.h"

class PhotoMosaic: public RGBImage{
  private:
    vector<RGBImage> elements;
  public:
    PhotoMosaic();
    ~PhotoMosaic();
    void LoadElements(vector<string> vec);
    void Normalize(int _w, int _h);
    void Transform(int _w, int _h);
    

};

#endif
