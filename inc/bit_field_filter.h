#ifndef BIT_FIELD_FILTER_H
#define BIT_FIELD_FILTER_H

#include <stdint.h>

// Define bit field constants for filters
#define BOX_FILTER              0b00000001
#define INTENSITY_TRANSFORM     0b00000010
#define HISTOGRAM_EQUALIZE      0b00000100
#define ALPHA_TRIMMED_MEAN      0b00001000
#define SOBEL_GRADIENT          0b00010000
#define LINEAR_MOTION_BLURRING  0b00100000

void applyBoxFilter(int **pixels, int w, int h);
void applyIntensityTransform(int **pixels, int w, int h);
void applyHistogramEqualization(int **pixels, int w, int h);
void applyAlphaTrimmedMeanFilter(int **pixels, int w, int h, int kernel_size = 3, double alpha = 0.1);
void applySobelGradient(int **pixels, int w, int h);
void applyLinearMotionBlurring(int **pixels, int w, int h, double theta, int length);
void applyFilters(int **pixels, int w, int h, uint8_t options, double theta = 0.0, int length = 15);
void applyFilters(int ***pixels, int w, int h, uint8_t options, double theta = 0.0, int length = 15);

#endif // BIT_FIELD_FILTER_H
