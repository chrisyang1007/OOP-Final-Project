#ifndef _IMAGE_H_
#define _IMAGE_H_
#include<bits/stdc++.h>
#include"data_loader.h"
using namespace std;

class Image{
protected:
  int w, h;
  static Data_Loader data_loader;
public:
  Image(int _w = 0, int _h = 0);
  virtual ~Image();
  int get_w();
  int get_h();
  virtual bool LoadImage(string fname);
  virtual void DumpImage(string fname);
  virtual void Display_X_Server();
  virtual void Display_ASCII();
  virtual void Display_CMD();
  virtual void resize(int _w, int _h);
  virtual void Transform(int _w, int _h);
  virtual void LoadElements(vector<string> vec);
};

#endif
