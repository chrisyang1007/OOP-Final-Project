#include"gray_image.h"
#include"bit_field_filter.h"
using namespace std;

GrayImage::GrayImage(){
  w = 0;
  h = 0;
  pixels = NULL;
}

GrayImage::GrayImage(int _w, int _h, int **_pixels): Image(_w, _h){
  pixels = _pixels;
}

GrayImage::~GrayImage(){
  for(int i = 0; i < h; i++)
    delete [] pixels[i];
  delete [] pixels;
  pixels = NULL;
}

bool GrayImage::LoadImage(string fname){
  if(pixels){
    for(int i = 0; i < h; i++)
      delete [] pixels[i];
    delete [] pixels;
    pixels = NULL;
  }
  pixels = data_loader.Load_Gray(fname, &w, &h);
  return 1;
  
  
}

void GrayImage::DumpImage(string fname){
  data_loader.Dump_Gray(w, h, pixels, fname);
  return;
}

void GrayImage::Display_X_Server(){
  data_loader.Display_Gray_X_Server(w, h, pixels);
  return;
}

void GrayImage::Display_ASCII(){
  data_loader.Display_Gray_ASCII(w, h, pixels);
  return;
}

void GrayImage::Display_CMD(){
  DumpImage("temp.png");
  data_loader.Display_Gray_CMD("temp.png");
  remove("temp.png");
  return;
}

void GrayImage::ApplyFilter(uint8_t options, double theta, int length) {
  applyFilters(pixels, w, h, options, theta, length);
}
