#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Initializes the matrix to the identity matrix.
 */
void init_identity_matrix(float matrix[3][3]);

/**
 * Prints the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Adds two matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

/**
 * Multiplies a matrix by a scalar.
 */
void multiply_matrix_scalar(const float matrix[3][3], float scalar, float result[3][3]);

/**
 * Multiplies two matrices.
 */
void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3]);

/**
 * Transforms a point using a matrix.
 * Input and output points are 3 element arrays (homogeneous coordinates).
 */
void transform_point(const float matrix[3][3], const float point[3], float result[3]);

/**
 * Applies a scaling transformation to the matrix (around the origin).
 */
void scale(float matrix[3][3], float sx, float sy);

/**
 * Applies a translation (shift) transformation to the matrix.
 */
void shift(float matrix[3][3], float tx, float ty);

/**
 * Applies a rotation transformation to the matrix (around the origin).
 * Angle is given in degrees.
 */
void rotate(float matrix[3][3], float angle_degrees);

#endif // MATRIX_H
