#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "photo_mosaic.h"
#include "bit_field_filter.h"
#include <bits/stdc++.h>



int main(int argc, char *argv[]){
    Data_Loader data_loader;
    
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/mnist/img_100.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_CMD();
    img1->Display_ASCII();
    delete img1;
    

    /*Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/lena.jpg");
    img2->DumpImage("img2.jpg");
    img2->Display_CMD();
    img2->Display_ASCII();
    delete img2;
    
    Image *img3 = new PhotoMosaic();
    vector<string> fnames;
    data_loader.List_Directory("Image-Folder/cifar10/", fnames);
    img3->LoadImage("Image-Folder/Albmuth.JPG");
    img3->LoadElements(fnames);
    img3->Transform(32,32);
    img3->DumpImage("img3.jpg");
    delete img3;*/
    

    // some bit field filter design driven code here

    // some photo mosaic driven code here

    // more ...
    return 0;
}
