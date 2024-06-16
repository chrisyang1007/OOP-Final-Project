#include"rgb_image.h"
#include<algorithm>
using namespace std;

RGBImage::RGBImage(){}

RGBImage::RGBImage(int _w, int _h, int ***_pixels): Image(_w, _h){
  pixels = _pixels;
}

RGBImage::~RGBImage(){
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++)
      delete [] pixels[i][j];
    delete [] pixels[i];
  }
  delete [] pixels;
  pixels = NULL;
}

bool RGBImage::LoadImage(string fname){
  pixels = data_loader.Load_RGB(fname, &w, &h);
  return 1;
}

void RGBImage::DumpImage(string fname){
  data_loader.Dump_RGB(w, h, pixels, fname);
  return;
}

void RGBImage::Display_X_Server(){
  data_loader.Display_RGB_X_Server(w, h, pixels);
  return;
}

void RGBImage::Display_ASCII(){
  data_loader.Display_RGB_ASCII(w, h, pixels);
  return;
}

void RGBImage::Display_CMD(){
  DumpImage("temp.png");
  data_loader.Display_RGB_CMD("temp.png");
  remove("temp.png");
  return;
}

void RGBImage::resize(int _w, int _h){
  int ***temp1, ***temp2;
  double ratio;
  
  //w-axis resize
  temp1 = new int**[h];
  for(int i = 0; i < h; i++){
    temp1[i] = new int*[_w];
    for(int j = 0; j < _w; j++){
      temp1[i][j] = new int[3];
      for(int k = 0; k < 3; k++)
        temp1[i][j][k] = 0;
    }
  }
  
  ratio = double(_w) / double(w);
  if(ratio >= 1){
    vector<int> existSpot;
    for(int i = 0, p = 0; p < _w; i++, p = i * ratio){
      existSpot.push_back(p);
      for(int j = 0; j < h; j++)
        for(int k = 0; k < 3; k++)
          temp1[j][p][k] = pixels[j][i][k];
    }
    for(int i = 0; i < _w; i++){
      auto it = lower_bound(existSpot.begin(), existSpot.end(), i);
      if(*it != i){
        int front = *(it -1), back = *it;
        for(int j = 0; j < h; j++)
          for(int k = 0; k < 3; k++)
            temp1[j][i][k] = ((back - i) * temp1[j][front][k] + (i - front) * temp1[j][back][k]) / (back - front);
      }
    }
  }else if(ratio < 1){
    vector<int> times(_w, 0);
    for(int i = 0, p = 0; p < _w; i++, p = i * ratio){
      times[p]++;
      for(int j = 0; j < h; j++)
        for(int k = 0; k < 3; k++)
          temp1[j][p][k] += pixels[j][i][k];
    }
    for(int i = 0; i < h; i++)
      for(int j = 0; j < _w; j++)
        for(int k = 0; k < 3; k++)
          temp1[i][j][k] /= times[j];
  }
  //h-axis resize
  temp2 = new int**[_h];
  for(int i = 0; i < _h; i++){
    temp2[i] = new int*[_w];
    for(int j = 0; j < _w; j++){
      temp2[i][j] = new int[3];
      for(int k = 0; k < 3; k++)
        temp2[i][j][k] = 0;
    }
  }
  
  ratio = double(_h) / double(h);
  if(ratio >= 1){
    vector<int> existSpot;
    for(int i = 0, p = 0; p < _h; i++, p = i * ratio){
      existSpot.push_back(p);
      for(int j = 0; j < _w; j++)
        for(int k = 0; k < 3; k++)
          temp2[p][j][k] = temp1[i][j][k];
    }
    for(int i = 0; i < _h; i++){
      auto it = lower_bound(existSpot.begin(), existSpot.end(), i);
      if(*it != i){
        int front = *(it -1), back = *it;
        for(int j = 0; j < _w; j++)
          for(int k = 0; k < 3; k++)
            temp2[i][j][k] = ((back - i) * temp2[front][j][k] + (i - front) * temp2[back][j][k]) / (back - front);
      }
    }
  }else if(ratio < 1){
    vector<int> times(_h, 0);
    for(int i = 0, p = 0; p < _h; i++, p = i * ratio){
      times[p]++;
      for(int j = 0; j < _w; j++)
        for(int k = 0; k < 3; k++)
          temp2[p][j][k] += temp1[i][j][k];
    }
    for(int i = 0; i < _h; i++)
      for(int j = 0; j < _w; j++)
        for(int k = 0; k < 3; k++)
          temp2[i][j][k] /= times[i];
  }
  
  //delete and return
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++)
      delete [] pixels[i][j];
    delete [] pixels[i];
  }
  delete [] pixels; 
  
  for(int i = 0; i < h; i++){
    for(int j = 0; j < _w; j++)
      delete [] temp1[i][j];
    delete [] temp1[i];
  }
  delete [] temp1;
  
  w = _w;
  h = _h;
  pixels = temp2;
}

vector<double> RGBImage::Average_RGB(int x1, int y1, int x2, int y2){
  vector<double> ans(3,0);
  for(int k = 0; k < 3; k++){
    for(int i = y1; i < y2; i++)
      for(int j = x1; j < x2; j++)
        ans[k] += pixels[i][j][k];
    ans[k] /= (x2 - x1) * (y2 - y1);
  }
  return ans;
}

int*** RGBImage::getPixels(){
  return pixels;
}

