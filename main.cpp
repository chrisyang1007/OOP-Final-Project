#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "photo_mosaic.h"
#include "bit_field_filter.h"
#include <bits/stdc++.h>

using namespace std;

void processAndDisplay(Image* img, const string& baseFilename, uint8_t filters, const string& filterNames, double theta = 0.0, int length = 15) {
    cout << "Loading image: " << baseFilename << endl;
    if (!img->LoadImage(baseFilename)) {
        cerr << "Error loading image: " << baseFilename << endl;
        return;
    }

    cout << "Applying filters: " << filterNames << endl;
    img->ApplyFilter(filters, theta, length);

    string outputFilename = "filtered_" + filterNames + "_" + baseFilename.substr(baseFilename.find_last_of("/") + 1);
    img->DumpImage(outputFilename);

    cout << "Filters: " << filterNames << " | Image: " << baseFilename << endl;
    
    // Display ASCII version
    cout << "Displaying ASCII version:" << endl;
    img->Display_ASCII();

    // Display filtered image
    img->Display_X_Server();
    img->Display_CMD();
}

void applyBitFieldFilters(Image* img, const string& baseFilename, uint8_t filterCombination, double theta, int length) {
    string filterNames;

    if (filterCombination & BOX_FILTER)
        filterNames += "box_filter+";
    if (filterCombination & INTENSITY_TRANSFORM)
        filterNames += "intensity_transform+";
    if (filterCombination & HISTOGRAM_EQUALIZE)
        filterNames += "histogram_equalize+";
    if (filterCombination & ALPHA_TRIMMED_MEAN)
        filterNames += "alpha_trimmed_mean+";
    if (filterCombination & SOBEL_GRADIENT)
        filterNames += "sobel_gradient+";
    if (filterCombination & LINEAR_MOTION_BLURRING)
        filterNames += "linear_motion_blurring+";

    // Remove the trailing '+'
    if (!filterNames.empty())
        filterNames.pop_back();

    processAndDisplay(img, baseFilename, filterCombination, filterNames, theta, length);
}

int main() {
    string filenameGray = "Image-Folder/Kano.jpg";
    string filenameRGB = "Image-Folder/lena.jpg";
    cout << "BOX_FILTER              0b00000001" << endl << "INTENSITY_TRANSFORM     0b00000010" << endl << "HISTOGRAM_EQUALIZE      0b00000100" << endl << "ALPHA_TRIMMED_MEAN      0b00001000" << endl << "SOBEL_GRADIENT          0b00010000"<< endl << "LINEAR_MOTION_BLURRING  0b00100000"<< endl;
    cout << "Enter bit field (e.g., 0b00001001): ";
    string input;
    cin >> input;

    // Convert bit field string to uint8_t
    uint8_t filterCombination = static_cast<uint8_t>(bitset<8>(input.substr(2)).to_ulong());

    double theta = 0.0;
    int length = 15;
    if (filterCombination & LINEAR_MOTION_BLURRING) {
        cout << "Enter theta (angle in radians, e.g., 0.5): ";
        cin >> theta;
        cout << "Enter length of motion blur (e.g., 15): ";
        cin >> length;
    }

    // Process and display grayscale image with the filter combination
    cout << "Processing grayscale image..." << endl;
    Image* imgGray = new GrayImage();
    applyBitFieldFilters(imgGray, filenameGray, filterCombination, theta, length);
    delete imgGray;

    // Process and display RGB image with the filter combination
    cout << "Processing RGB image..." << endl;
    Image* imgRGB = new RGBImage();
    applyBitFieldFilters(imgRGB, filenameRGB, filterCombination, theta, length);
    delete imgRGB;
    
    Image *img3 = new PhotoMosaic();
    img3->LoadImage("Image-Folder/Albmuth.JPG");
    img3->LoadElements("Image-Folder/cifar10/");
    img3->Transform(32,32);
    img3->DumpImage("img3.jpg");
    img3->Display_X_Server();
    delete img3;

    return 0;
}

