#ifndef _RGB_IMAGE_H_
#define _RGB_IMAGE_H_

#include "image.h"

class RGBImage : public Image{
public:
  int ***pixels;

public:
  RGBImage();
  RGBImage(int _w, int _h, int ***_pixels);
  ~RGBImage();
  bool LoadImage(string fname);
  void DumpImage(string fname);
  void Display_X_Server();
  void Display_ASCII();
  void Display_CMD();
  void resize(int _w, int _h);
  vector<double> Average_RGB(int x1, int y1, int x2, int y2);
  int*** getPixels();
  //friend void PhotoMosaic::Transform(int _w, int _h);


};

#endif
