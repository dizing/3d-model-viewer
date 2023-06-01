#include "viewer_blas.h"

void mult(int M, int N, int K, const float* A, const float* B, float* C) {
  for (int i = 0; i < M; ++i) {
    float* c = C + i * N;
    for (int j = 0; j < N; ++j) c[j] = 0;
    for (int k = 0; k < K; ++k) {
      const float* b = B + k * N;
      float a = A[i * K + k];
      for (int j = 0; j < N; ++j) c[j] += a * b[j];
    }
  }
}

v_matrix4x4 matrix_mult_4x4(v_matrix4x4 first, v_matrix4x4 second) {
  v_matrix4x4 result;
  mult(4, 4, 4, &(first.row1.x), &(second.row1.x), &(result.row1.x));
  return result;
}

v_vectorf4d get4d_by3d(v_vectorf3d vector) {
  v_vectorf4d result;
  result.x = vector.x;
  result.y = vector.y;
  result.z = vector.z;
  result.w = 1.0f;
  return result;
}
