#include "bit_field_filter.h"
#include "CImg.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

using namespace cimg_library;
using namespace std;

#define MAX_GRAY_VALUE 255

void applyBoxFilter(int **pixels, int w, int h) {
    CImg<int> img(w, h, 1, 1, 0);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            img(x, y) = pixels[y][x];
        }
    }
    img.blur(1); // Use default blur intensity
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            pixels[y][x] = img(x, y);
        }
    }
}

void applyIntensityTransform(int **pixels, int w, int h) {
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            pixels[y][x] = min(255, max(0, pixels[y][x] + 50)); // Adjust intensity
        }
    }
}

void applyHistogramEqualization(int **pixels, int w, int h) {
    // Calculate histogram
    int hist[MAX_GRAY_VALUE + 1] = {0};
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            hist[pixels[y][x]]++;
        }
    }

    // Calculate cumulative distribution function (CDF)
    float cdf[MAX_GRAY_VALUE + 1] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i <= MAX_GRAY_VALUE; ++i) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Normalize CDF
    for (int i = 0; i <= MAX_GRAY_VALUE; ++i) {
        cdf[i] = cdf[i] / (w * h);
    }

    // Create the lookup table for equalization
    int equalized[MAX_GRAY_VALUE + 1];
    for (int i = 0; i <= MAX_GRAY_VALUE; ++i) {
        equalized[i] = static_cast<int>(cdf[i] * MAX_GRAY_VALUE + 0.5);
    }

    // Apply equalization
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            pixels[y][x] = equalized[pixels[y][x]];
        }
    }
}

void applyAlphaTrimmedMeanFilter(int **pixels, int w, int h, int kernel_size, double alpha) {
    int half_k = kernel_size / 2;
    vector<int> window;
    CImg<int> img(w, h, 1, 1, 0);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            img(x, y) = pixels[y][x];
        }
    }

    for (int y = half_k; y < h - half_k; ++y) {
        for (int x = half_k; x < w - half_k; ++x) {
            window.clear();
            for (int ky = -half_k; ky <= half_k; ++ky) {
                for (int kx = -half_k; kx <= half_k; ++kx) {
                    window.push_back(img(x + kx, y + ky));
                }
            }
            sort(window.begin(), window.end());
            int trim = static_cast<int>(alpha * window.size());
            int sum = 0;
            for (int i = trim; i < static_cast<int>(window.size()) - trim; ++i) {
                sum += window[i];
            }
            pixels[y][x] = sum / (window.size() - 2 * trim);
        }
    }
}

void applySobelGradient(int **pixels, int w, int h) {
    const int sobel_x[3][3] = {
        { 1, 0, -1 },
        { 2, 0, -2 },
        { 1, 0, -1 }
    };

    const int sobel_y[3][3] = {
        { 1, 2, 1 },
        { 0, 0, 0 },
        { -1, -2, -1 }
    };

    CImg<int> img(w, h, 1, 1, 0);
    CImg<int> grad_x(w, h, 1, 1, 0);
    CImg<int> grad_y(w, h, 1, 1, 0);
    CImg<int> grad_mag(w, h, 1, 1, 0);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            img(x, y) = pixels[y][x];
        }
    }

    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            int gx = 0;
            int gy = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    gx += sobel_x[ky + 1][kx + 1] * img(x + kx, y + ky);
                    gy += sobel_y[ky + 1][kx + 1] * img(x + kx, y + ky);
                }
            }
            grad_x(x, y) = gx;
            grad_y(x, y) = gy;
            grad_mag(x, y) = static_cast<int>(sqrt(gx * gx + gy * gy));
        }
    }

    // Normalize gradient magnitude
    int max_grad = 0;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (grad_mag(x, y) > max_grad) {
                max_grad = grad_mag(x, y);
            }
        }
    }

    // Apply intensity adjustment
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            pixels[y][x] = static_cast<int>((grad_mag(x, y) / static_cast<float>(max_grad)) * 255);
        }
    }
}

void applyLinearMotionBlurring(int **pixels, int w, int h, double theta, int length) {
    CImg<int> img(w, h, 1, 1, 0);
    CImg<int> result(w, h, 1, 1, 0);

    // Copy pixels to CImg object
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            img(x, y) = pixels[y][x];
        }
    }

    // Define the motion blur kernel
    int half_length = length / 2;
    vector<int> kernel(length, 0);
    for (int i = -half_length; i <= half_length; ++i) {
        kernel[i + half_length] = 1;
    }

    // Calculate the cosine and sine of the angle
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    // Apply the motion blur kernel
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum = 0;
            int count = 0;
            for (int k = -half_length; k <= half_length; ++k) {
                int new_x = x + k * cos_theta;
                int new_y = y + k * sin_theta;
                if (new_x >= 0 && new_x < w && new_y >= 0 && new_y < h) {
                    sum += img(new_x, new_y);
                    count++;
                }
            }
            result(x, y) = sum / count;
        }
    }

    // Copy the result back to the pixels array
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            pixels[y][x] = result(x, y);
        }
    }
}

void applyFilters(int **pixels, int w, int h, uint8_t options, double theta, int length) {
    if (options & BOX_FILTER) applyBoxFilter(pixels, w, h);
    if (options & INTENSITY_TRANSFORM) applyIntensityTransform(pixels, w, h);
    if (options & HISTOGRAM_EQUALIZE) applyHistogramEqualization(pixels, w, h);
    if (options & ALPHA_TRIMMED_MEAN) applyAlphaTrimmedMeanFilter(pixels, w, h, 3, 0.1);
    if (options & SOBEL_GRADIENT) applySobelGradient(pixels, w, h);
    if (options & LINEAR_MOTION_BLURRING) applyLinearMotionBlurring(pixels, w, h, theta, length);
}

void applyFilters(int ***pixels, int w, int h, uint8_t options, double theta, int length) {
    for (int channel = 0; channel < 3; ++channel) {
        int **channelPixels = new int*[h];
        for (int i = 0; i < h; ++i) {
            channelPixels[i] = new int[w];
            for (int j = 0; j < w; ++j) {
                channelPixels[i][j] = pixels[i][j][channel];
            }
        }

        applyFilters(channelPixels, w, h, options, theta, length);

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                pixels[i][j][channel] = channelPixels[i][j];
            }
            delete[] channelPixels[i];
        }
        delete[] channelPixels;
    }
}
