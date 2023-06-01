#if !defined(PARSER_H)
#define PARSER_H

/**
 * @file parser.h
 * @author SevenStreams
 * @brief This file handles parsing of .obj files
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <ctype.h>
#include <locale.h>
#include <stdio.h>

#include "model.h"
#include "vector.h"
#include "viewer_blas.h"

#define MAX_NUM 1000
#define SEP " "
#define START_CAPACITY 4
enum { VECTOR = 'v', FACE = 'f', SPACE = ' ' };

/**
 * @brief This function checks whether file exists or not
 *
 * @param filename File's name
 * @return int An error
 */
int file_exists(const char *filename);

/**
 * @brief This function checks whether file if empty or not
 *
 * @param filename File's name
 * @return int An error
 */
int file_empty(const char *filename);

/**
 * @brief The function handles parsing model
 *
 * @param model
 */
void process(v_model *model);

/**
 * @brief The function handles checking model before parsing
 *
 * @param model The model for processing
 */
void parser(v_model *model);

/**
 * @brief This function handles adding list of vertexes to model
 *
 * @param vertexes A list of vertexes
 * @param model The model for processing
 */
void vertexes_to_model(Vector *vertexes, v_model *model);

/**
 * @brief This function handles adding list of indexes to model
 *
 * @param vertex_indexes A list of vertexes' indexes
 * @param model The model for processing
 */
void indexes_to_model(Vector *vertex_indexes, v_model *model);

/**
 * @brief This function handles extracting numbers and adding them to the list
 * of vertexes
 *
 * @param vertexes A list of vertexes
 * @param str An input string from file
 * @return int An error
 */
int add_to_vector(Vector *vertexes, char str[MAX_NUM]);

/**
 * @brief This function handles extracting numbers and adding them to the list
 * of indexes
 *
 * @param vert_indexes A list of vertexes' indexes
 * @param str An input string from file
 * @return int An error
 */
int parse_face(Vector *vertexes, char str[MAX_NUM]);

#endif  // PARSER_H