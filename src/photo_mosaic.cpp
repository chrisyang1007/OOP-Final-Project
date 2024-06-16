#include"photo_mosaic.h"
using namespace std;

PhotoMosaic::PhotoMosaic(){}

PhotoMosaic::~PhotoMosaic(){
  for(int i = 0; i < elements.size(); i++)
    elements[i].~RGBImage();
}


void PhotoMosaic::LoadElements(vector<string> vec){
  elements.resize(vec.size());
  for(int i = 0; i < vec.size(); i++){
    elements[i].LoadImage(vec[i]);
  }
}

void PhotoMosaic::Normalize(int _w, int _h){
  for(int i = 0; i < elements.size(); i++)
    elements[i].resize(_w, _h);
}

void PhotoMosaic::Transform(int _w, int _h){
  Normalize(_w, _h);
  vector<vector<double>> avgRGB(elements.size(), vector<double>(3,0));
  vector<double> temp1, temp2;
  
  //full size element
  for(int i = 0; i < elements.size(); i++)
    avgRGB[i] = elements[i].Average_RGB(0, 0, _w, _h);
  int numCol = w / _w, numRow = h / _h, ans;
  double min;
  for(int n = 0; n < numRow; n++){
    for(int m = 0; m < numCol; m++){
      temp1 = Average_RGB(m * _w, n * _h, (m + 1) * _w, (n + 1) * _h);
      ans = 0;
      min = pow(avgRGB[0][0] - temp1[0], 2) + pow(avgRGB[0][1] - temp1[1], 2) + pow(avgRGB[0][2] - temp1[2], 2);
      for(int i = 1; i < elements.size(); i++){
        double a = pow(avgRGB[i][0] - temp1[0], 2) + pow(avgRGB[i][1] - temp1[1], 2) + pow(avgRGB[i][2] - temp1[2], 2);
        if(a < min){
          min = a;
          ans = i;
        }
      }
      for(int i = 0; i < _h; i++)
        for(int j = 0; j < _w; j++)
          for(int k = 0; k < 3; k++)
            pixels[n * _h + i][m * _w + j][k] = elements[ans].pixels[i][j][k];
    }
  }
  
  //right border
  for(int i = 0; i < elements.size(); i++)
    avgRGB[i] = elements[i].Average_RGB(0, 0, w - numCol * _w, _h);
  for(int n = 0; n < numRow; n++){
    temp1 = Average_RGB(numCol * _w, n * _h, w, (n + 1) * _h);
    ans = 0;
    min = pow(avgRGB[0][0] - temp1[0], 2) + pow(avgRGB[0][1] - temp1[1], 2) + pow(avgRGB[0][2] - temp1[2], 2);
    for(int i = 1; i < elements.size(); i++){
      double a = pow(avgRGB[i][0] - temp1[0], 2) + pow(avgRGB[i][1] - temp1[1], 2) + pow(avgRGB[i][2] - temp1[2], 2);
      if(a < min){
        min = a;
        ans = i;
      }
    }
    for(int i = 0; i < _h; i++)
      for(int j = 0; j < w - numCol * _w; j++)
        for(int k = 0; k < 3; k++)
          pixels[n * _h + i][numCol * _w + j][k] = elements[ans].pixels[i][j][k];
  }
  
  //down border
  for(int i = 0; i < elements.size(); i++)
    avgRGB[i] = elements[i].Average_RGB(0, 0, _w, h - numRow * _h);
  for(int m = 0; m < numCol; m++){
    temp1 = Average_RGB(m * _w, numRow * _h, (m + 1) * _w, h);
    ans = 0;
    min = pow(avgRGB[0][0] - temp1[0], 2) + pow(avgRGB[0][1] - temp1[1], 2) + pow(avgRGB[0][2] - temp1[2], 2);
    for(int i = 1; i < elements.size(); i++){
      double a = pow(avgRGB[i][0] - temp1[0], 2) + pow(avgRGB[i][1] - temp1[1], 2) + pow(avgRGB[i][2] - temp1[2], 2);
      if(a < min){
        min = a;
        ans = i;
      }
    }
    for(int i = 0; i < h - numRow * _h; i++)
      for(int j = 0; j < _w; j++)
        for(int k = 0; k < 3; k++)
          pixels[numRow * _h + i][m * _w + j][k] = elements[ans].pixels[i][j][k];
  }
  
  //the down-right spot
  for(int i = 0; i < elements.size(); i++)
    avgRGB[i] = elements[i].Average_RGB(0, 0, w - numCol * _w, h - numRow * _h);
  temp1 = Average_RGB(numCol * _w, numRow * _h, w, h);
  ans = 0;
  min = pow(avgRGB[0][0] - temp1[0], 2) + pow(avgRGB[0][1] - temp1[1], 2) + pow(avgRGB[0][2] - temp1[2], 2);
  for(int i = 1; i < elements.size(); i++){
    double a = pow(avgRGB[i][0] - temp1[0], 2) + pow(avgRGB[i][1] - temp1[1], 2) + pow(avgRGB[i][2] - temp1[2], 2);
    if(a < min){
      min = a;
      ans = i;
    }
  }
  for(int i = 0; i < h - numRow * _h; i++)
      for(int j = 0; j < w - numCol * _w; j++)
        for(int k = 0; k < 3; k++)
          pixels[numRow * _h + i][numCol * _w + j][k] = elements[ans].pixels[i][j][k];
  
}

