#if !defined(VERTEX_PROCESSOR_H)
#define VERTEX_PROCESSOR_H
/**
 * @file vertex_processor.h
 * @author SevenStreams
 * @brief This file handles working with vertexes
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "stdio.h"
#include "viewer_blas.h"
// HEADER ONLY FUNCTIONS FOR MORE FAST COMPUTATIONS

/**
 * @brief The function apply transformation matrix to the vector
 *
 * @param start_vertex
 * @param transf_matrix
 * @return v_vectorf4d The result vector
 */
static inline v_vectorf4d single_f4d_vertex_processing(
    v_vectorf4d start_vertex, v_matrix4x4 transf_matrix) {
  v_vectorf4d result;
  mult(4, 1, 4, &(transf_matrix.row1.x), &(start_vertex.x), &(result.x));
  result.x = result.x / result.w;
  result.y = result.y / result.w;
  result.z = result.z / result.w;
  result.w = 1;
  return result;
}

/**
 * @brief The function apply transformation matrix to all vectors in the list
 *
 * @param in_array Input 4d vector
 * @param out_array Output 4d vector
 * @param count Number of vectors
 * @param transf_matrix Matrix 4x4
 */
static inline void f4d_vertex_array_processing(v_vectorf4d *in_array,
                                               v_vectorf4d *out_array,
                                               int count,
                                               v_matrix4x4 transf_matrix) {
  for (int i = 0; i < count; ++i) {
    out_array[i] = single_f4d_vertex_processing(in_array[i], transf_matrix);
  }
}

#endif  // VERTEX_PROCESSOR_H
