#include"data_loader.h"
#include"image.h"
using namespace std;

Image::Image(int _w, int _h){
  w = _w;
  h = _h;
}

Image::~Image(){
}

int Image::get_w(){
  return w;
}

int Image::get_h(){
  return h;
}

bool Image::LoadImage(string fname){
  return 1;
}

void Image::DumpImage(string fname){
  return;
}

void Image::Display_X_Server(){
  return;
}

void Image::Display_ASCII(){
  return;
}

void Image::Display_CMD(){
  return;
}

void Image::resize(int _w, int _h){
  return;
}

void Image::Transform(int _w, int _h){
  return;
}

void Image::LoadElements(string fname){
  return;
}

void Image::ApplyFilter(uint8_t options, double theta, int length){}