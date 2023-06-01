#include "transf_matrix_generator.h"

v_matrix4x4 generate_scale_matrix(float xScale, float yScale, float zScale) {
  v_matrix4x4 result = {{.x = 1.0f * xScale, .y = 0.0f, .z = 0.0f, .w = 0.0f},
                        {.x = 0.0f, .y = 1.0f * yScale, .z = 0.0f, .w = 0.0f},
                        {.x = 0.0f, .y = 0.0f, .z = 1.0f * zScale, .w = 0.0f},
                        {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}};
  return result;
}

v_matrix4x4 generate_Xaxis_rotation_matrix(float angle) {
  v_matrix4x4 result = {
      {.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f},
      {.x = 0.0f, .y = cosf(angle), .z = -sin(angle), .w = 0.0f},
      {.x = 0.0f, .y = sin(angle), .z = cos(angle), .w = 0.0f},
      {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}};
  return result;
}

v_matrix4x4 generate_Yaxis_rotation_matrix(float angle) {
  v_matrix4x4 result = {
      {.x = cosf(angle), .y = 0.0f, .z = sinf(angle), .w = 0.0f},
      {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f},
      {.x = -sinf(angle), .y = 0.0f, .z = cos(angle), .w = 0.0f},
      {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}};
  return result;
}

v_matrix4x4 generate_Zaxis_rotation_matrix(float angle) {
  v_matrix4x4 result = {
      {.x = cosf(angle), .y = -sinf(angle), .z = 0.0f, .w = 0.0f},
      {.x = sinf(angle), .y = cosf(angle), .z = 0.0f, .w = 0.0f},
      {.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f},
      {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}};
  return result;
}

v_matrix4x4 generate_XYZaxis_rotation_matrix(float angleX, float angleY,
                                             float angleZ) {
  v_matrix4x4 matrix = generate_Xaxis_rotation_matrix(angleX);
  matrix = matrix_mult_4x4(matrix, generate_Yaxis_rotation_matrix(angleY));
  matrix = matrix_mult_4x4(matrix, generate_Zaxis_rotation_matrix(angleZ));
  return matrix;
}

v_matrix4x4 generate_translation_matrix(v_vectorf3d vec) {
  v_matrix4x4 result = {{.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = vec.x},
                        {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = vec.y},
                        {.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = vec.z},
                        {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}};
  return result;
}

// opengl frustrum copy
v_matrix4x4 generate_frustrum_matrix(double left, double right, double bottom,
                                     double top, double nearVal,
                                     double farVal) {
  float A = (right + left) / (right - left);
  float B = (top + bottom) / (top - bottom);
  float C = -(farVal + nearVal) / (farVal - nearVal);
  float D = -2 * farVal * nearVal / (farVal - nearVal);
  float E = 2 * nearVal / (right - left);
  float F = 2 * nearVal / (top - bottom);
  v_matrix4x4 result = {{.x = E, .y = 0.0f, .z = A, .w = 0.0f},
                        {.x = 0.0f, .y = F, .z = B, .w = 0.0f},
                        {.x = 0.0f, .y = 0.0f, .z = C, .w = D},
                        {.x = 0.0f, .y = 0.0f, .z = -1, .w = 1.0f}};
  return result;
}

v_matrix4x4 generate_identity() {
  v_matrix4x4 result = {{.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f},
                        {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f},
                        {.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f},
                        {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}};
  return result;
}