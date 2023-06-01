#if !defined(TRANSFORMATION_MATRIX_GENERATOR)
#define TRANSFORMATION_MATRIX_GENERATOR

/**
 * @file transf_matrix_generator.h
 * @author SevenStreams
 * @brief This file handles transforming matrix
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "math.h"
#include "viewer_blas.h"

/**
 * @brief The function generates scale matrix
 *
 * @param xScale The x scale
 * @param yScale The y scale
 * @param zScale The z scale
 * @return v_matrix4x4 Result matrix 4x4
 */
v_matrix4x4 generate_scale_matrix(float xScale, float yScale, float zScale);

/**
 * @brief The function rotates matrix by x
 *
 * @param angle x angle for rotation
 * @return v_matrix4x4 Result matrix 4x4
 */
v_matrix4x4 generate_Xaxis_rotation_matrix(float angle);

/**
 * @brief The function rotates matrix by y
 *
 * @param angle y angle for rotation
 * @return v_matrix4x4 Result matrix 4x4
 */
v_matrix4x4 generate_Yaxis_rotation_matrix(float angle);

/**
 * @brief The function rotates matrix by z
 *
 * @param angle z angle for rotation
 * @return v_matrix4x4 Result matrix 4x4
 */
v_matrix4x4 generate_Zaxis_rotation_matrix(float angle);

/**
 * @brief The function rotates matrix
 *
 * @param angleX x angle for rotation
 * @param angleY y angle for rotation
 * @param angleZ z angle for rotation
 * @return v_matrix4x4 Result matrix 4x4
 */
v_matrix4x4 generate_XYZaxis_rotation_matrix(float angleX, float angleY,
                                             float angleZ);

/**
 * @brief The function translates matrix
 *
 * @param vec 3d vector
 * @return v_matrix4x4 Result matrix 4x4
 */
v_matrix4x4 generate_translation_matrix(v_vectorf3d vec);

/**
 * @brief The function generates frustrum matrix
 *
 * @param left left number
 * @param right right number
 * @param bottom bottom number
 * @param top top number
 * @param nearVal near value number
 * @param farVal far value number
 * @return v_matrix4x4 Result matrix 4x4
 */
v_matrix4x4 generate_frustrum_matrix(double left, double right, double bottom,
                                     double top, double nearVal, double farVal);

/**
 * @brief The function generates matrix
 *
 * @return v_matrix4x4
 */
v_matrix4x4 generate_identity();

#endif  // TRANSFORMATION_MATRIX_GENERATOR
