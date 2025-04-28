#include "matrix.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void init_zero_matrix(float matrix[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            matrix[i][j] = 0.0f;
}

void init_identity_matrix(float matrix[3][3])
{
    init_zero_matrix(matrix);
    for (int i = 0; i < 3; ++i)
        matrix[i][i] = 1.0f;
}

void print_matrix(const float matrix[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%7.3f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            c[i][j] = a[i][j] + b[i][j];
}

void multiply_matrix_scalar(const float matrix[3][3], float scalar, float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            result[i][j] = matrix[i][j] * scalar;
}

void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3])
{
    int i, j, k;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = 0.0f;
            for (k = 0; k < 3; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transform_point(const float matrix[3][3], const float point[3], float result[3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        result[i] = 0.0f;
        for (j = 0; j < 3; ++j) {
            result[i] += matrix[i][j] * point[j];
        }
    }
}

void scale(float matrix[3][3], float sx, float sy)
{
    float scale_matrix[3][3];
    init_identity_matrix(scale_matrix);
    scale_matrix[0][0] = sx;
    scale_matrix[1][1] = sy;

    float temp[3][3];
    multiply_matrices(scale_matrix, matrix, temp);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = temp[i][j];
}

void shift(float matrix[3][3], float tx, float ty)
{
    float shift_matrix[3][3];
    init_identity_matrix(shift_matrix);
    shift_matrix[0][2] = tx;
    shift_matrix[1][2] = ty;

    float temp[3][3];
    multiply_matrices(shift_matrix, matrix, temp);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = temp[i][j];
}

void rotate(float matrix[3][3], float angle_degrees)
{
    float radians = angle_degrees * (float)M_PI / 180.0f;
    float rotate_matrix[3][3];
    init_identity_matrix(rotate_matrix);

    rotate_matrix[0][0] = cosf(radians);
    rotate_matrix[0][1] = -sinf(radians);
    rotate_matrix[1][0] = sinf(radians);
    rotate_matrix[1][1] = cosf(radians);

    float temp[3][3];
    multiply_matrices(rotate_matrix, matrix, temp);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = temp[i][j];
}
