#include "matrix.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
    float b[3][3];
    float c[3][3];

    printf("Matrix A:\n");
    print_matrix(a);

    init_zero_matrix(b);
    b[1][1] = 8.0f;
    b[2][0] = -3.0f;
    b[2][2] = 5.0f;

    printf("Matrix B:\n");
    print_matrix(b);

    add_matrices(a, b, c);
    printf("A + B = C:\n");
    print_matrix(c);

    printf("Identity matrix:\n");
    float identity[3][3];
    init_identity_matrix(identity);
    print_matrix(identity);

    printf("A scaled by 2.0:\n");
    multiply_matrix_scalar(a, 2.0f, c);
    print_matrix(c);

    printf("A * Identity:\n");
    multiply_matrices(a, identity, c);
    print_matrix(c);

    printf("Transforming point (1, 2, 1) with A:\n");
    float point[3] = {1.0f, 2.0f, 1.0f};
    float transformed_point[3];
    transform_point(a, point, transformed_point);
    printf("(%.3f, %.3f, %.3f)\n\n", transformed_point[0], transformed_point[1], transformed_point[2]);

    printf("Applying scale (2, 3) to Identity:\n");
    init_identity_matrix(c);
    scale(c, 2.0f, 3.0f);
    print_matrix(c);

    printf("Applying shift (5, -2) to Identity:\n");
    init_identity_matrix(c);
    shift(c, 5.0f, -2.0f);
    print_matrix(c);

    printf("Applying rotation (90 degrees) to Identity:\n");
    init_identity_matrix(c);
    rotate(c, 90.0f);
    print_matrix(c);

    return 0;
}
