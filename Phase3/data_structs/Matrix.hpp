#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <math.h>

void multiplyMatrices(int la, int ca, const float* A, 
                     int lb, int cb, const float* B, 
                     float* R, int* lr = nullptr, int* cr = nullptr);

void buildRotMatrix(const float *x, const float *y, const float *z, float *m);

void cross(const float *a, const float *b, float *res);

void normalize(float *a);

float length(const float *a);

void getGlobalCatmullRomPoint(float gt, std::vector<std::vector<float>> controlPoints, float *pos, float *deriv);

#endif