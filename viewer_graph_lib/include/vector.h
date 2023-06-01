#if !defined(VECTOR_H)
#define VECTOR_H

/**
 * @file vector.h
 * @author SevenStreams
 * @brief This file handles working with vector
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Lis of error types
 *
 */
enum { OK, ERROR, ERROR_FILE, ERROR_V, ERROR_F, EMPTY_FILE };

/**
 * @brief Vector of numbers list
 *
 */
typedef struct Vector {
  double *data;
  size_t size;
  size_t capacity;
} Vector;

/**
 * @brief The function creates new vector
 *
 * @param initial_capacity Initial capacity of vector
 * @return Vector* The vector
 */
Vector *vector_new(size_t initial_capacity);

/**
 * @brief The function frees vector
 *
 * @param v The vector
 */
void vector_free(Vector *v);

/**
 * @brief The function reserves new memory for vector
 *
 * @param v The vector
 * @param new_capacity New capacity of vector
 * @return int An error
 */
int vector_reserve(Vector *v, size_t new_capacity);

/**
 * @brief The function adds new item to vector
 *
 * @param v The vector
 * @param item An item for adding
 */
void vector_add_item(Vector *v, double item);

#endif  // VECTOR_H