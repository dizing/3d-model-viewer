#if !defined(VIEWER_BLAS_H)
#define VIEWER_BLAS_H
/**
 * @file viewer_blas.h
 * @author SevenStreams
 * @brief This file handles working with vector and matrixes
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

/**
 * @brief 3D Vector
 *
 */
typedef struct v_vector3d_t {
  union {
    float x, r;
  };
  union {
    float y, g;
  };
  union {
    float z, b;
  };
} v_vectorf3d;

/**
 * @brief 4D Vector
 *
 */
typedef struct v_vector4d_t {
  union {
    float x, r;
  };
  union {
    float y, g;
  };
  union {
    float z, b;
  };
  float w;
} v_vectorf4d;

/**
 * @brief Matrix 4x4
 *
 */
typedef struct v_matrix4x4_t {
  v_vectorf4d row1;
  v_vectorf4d row2;
  v_vectorf4d row3;
  v_vectorf4d row4;
} v_matrix4x4;

/**
 * @brief Matrix 3x3
 *
 */
typedef struct v_matrix3x3_t {
  v_vectorf4d row1;
  v_vectorf3d row2;
  v_vectorf3d row3;
} v_matrix3x3;

/**
 * @brief The function handles matrix multiplication
 *
 * @param M Number of rows
 * @param N Number of columns
 * @param K Number of rows
 * @param A First matrix
 * @param B Second matrix
 * @param C The result of multiplication
 */
void mult(int M, int N, int K, const float* A, const float* B, float* C);

/**
 * @brief The function handles multiplication of 2 matrixes 4x4
 *
 * @param first First matrix 4x4
 * @param second Second matrix 4x4
 * @return v_matrix4x4 The result of matrix multiplication
 */
v_matrix4x4 matrix_mult_4x4(v_matrix4x4 first, v_matrix4x4 second);

/**
 * @brief Make the 4d vector from 3d vector
 *
 * @param vector 3d vector
 * @return v_vectorf4d 4d vector
 */
v_vectorf4d get4d_by3d(v_vectorf3d vector);

#endif  // VIEWER_BLAS_H
