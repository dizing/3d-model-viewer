#if !defined(SETTINGS_HANDLER_H)
#define SETTINGS_HANDLER_H

/**
 * @file settings_handler.h
 * @author SevenStreams
 * @brief This file handles working with settings
 * @version 0.1
 * @date 2023-05-21
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "stdio.h"
#include "viewer_blas.h"

/**
 * @brief Error codes for settings
 *
 */
typedef enum e_settings_error_codes {
  v_settings_OK,
  v_settings_CANT_WRITE
} v_settings_error_codes;

/**
 * @brief Line types for settings
 *
 */
typedef enum e_settings_line_types {
  v_settings_line_dotted,
  v_settings_line_solid
} v_settings_line_types;

/**
 * @brief Projection types for settings
 *
 */
typedef enum e_settings_projection_types {
  v_settings_projection_parallel,
  v_settings_projection_central
} v_settings_projection_types;

/**
 * @brief Vertex display types for settings
 *
 */
typedef enum e_settings_vertex_display_types {
  v_settings_vertex_display_no,
  v_settings_vertex_display_circle,
  v_settings_vertex_display_square
} v_settings_vertex_display_types;

/**
 * @brief Settings structure
 *
 */
typedef struct v_settings_t {
  v_settings_vertex_display_types display_type;
  v_settings_projection_types projection_type;
  v_settings_line_types line_type;
  float line_width;
  float vertex_size;
  v_vectorf3d background_color;
  v_vectorf3d line_color;
  v_vectorf3d vertices_color;
  v_settings_error_codes error_code;  // if 0 -- there is no errors yet
} v_settings;

/**
 * @brief The function extracts settings from a file
 *
 * @param settings_file_path The settings file path
 * @return v_settings The settings object
 */
v_settings read_settings(const char *settings_file_path);

/**
 * @brief The function gets the standard settings
 *
 * @return v_settings The settings object
 */
v_settings get_standard_settings();

/**
 * @brief The function saves settings into a file
 *
 * @param settings_file_path The settings file path
 */
void save_settings(const char *settings_file_path, v_settings);

/**
 * @brief The function initializing settings
 *
 * @param settings The settings object
 */
void standart_initialize(v_settings *settings);

#endif  // SETTINGS_HANDLER_H
