
#ifndef _GRAY_IMAGE_H_
#define _GRAY_IMAGE_H_

#include "image.h"

class GrayImage : public Image{
private:
  int **pixels;

public:
  GrayImage();
  GrayImage(int _w, int _h, int **_pixels);
  ~GrayImage();
  bool LoadImage(string fname);
  void DumpImage(string fname);
  void Display_X_Server();
  void Display_ASCII();
  void Display_CMD();
  void ApplyFilter(uint8_t options, double theta = 0.0, int length = 15);
};

#endif
