#if !defined(MODEL_H)
#define MODEL_H

/**
 * @file model.h
 * @author SevenStreams
 * @brief This file handles working with model
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "viewer_blas.h"
#define MINIMIZE_FACTOR 0.95f;
// Struct that contains all information about model
typedef struct v_model_t {
  const char* file_path;
  v_vectorf3d* vertices3d;
  v_vectorf4d* vertices4d;
  size_t vertices_count;  // size of vertices3d array
  unsigned int* indices;
  size_t indices_count;  // indices_count * 3 == size of indices array
  int error_code;        // if 0 -- there is no errors yet
} v_model;

#include "parser.h"
/**
 * @brief The functions handles initializing model
 *
 * @param file_path File's path
 * @return v_model The model for processing
 */
v_model initialize_model(const char* file_path);

/**
 * @brief The functions handles normalizing model
 *
 * @param model The model for processing
 */
void normalize_model(v_model* model);

/**
 * @brief The functions handles creating 4d vertices
 *
 * @param model The model for processing
 */
void generate4d_from3d(v_model* model);

/**
 * @brief The functions handles freeing model
 *
 * @param model The model for processing
 */
void delete_model(v_model* model);

/**
 * @brief The functions finds max of 2 numbers
 *
 * @param a First number
 * @param b Second number
 * @return float The max value
 */
float max(float a, float b);

/**
 * @brief The functions finds min of 2 numbers
 *
 * @param a First number
 * @param b Second number
 * @return float The min value
 */
float min(float a, float b);

#endif  // MODEL_H
