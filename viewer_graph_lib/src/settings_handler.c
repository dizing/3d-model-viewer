#include "settings_handler.h"

v_settings read_settings(const char* settings_file_path) {
  v_settings settings;

  FILE* input = fopen(settings_file_path, "rb");
  if (input != NULL && fread(&settings, sizeof(settings), 1, input) >= 1) {
    fclose(input);
  } else {
    standart_initialize(&settings);
  }
  return settings;
}

v_settings get_standard_settings() {
  v_settings settings;
  standart_initialize(&settings);
  return settings;
}

void save_settings(const char* settings_file_path, v_settings settings) {
  FILE* output = fopen(settings_file_path, "wb");
  if (output != NULL) {
    fwrite(&settings, sizeof(settings), 1, output);
    fclose(output);
  } else {
    settings.error_code = v_settings_CANT_WRITE;
  }
}

void standart_initialize(v_settings* settings) {
  settings->error_code = 0;
  v_vectorf3d background_color = {
      .r = 79 / 255.0f, .g = 33 / 255.0f, .b = 176 / 255.0f};
  settings->background_color = background_color;
  v_vectorf3d line_color = {.r = 1.0f, .g = 1.0f, .b = 1.0f};
  settings->line_color = line_color;
  v_vectorf3d vertices3d_color = {.r = 1.0f, .g = 1.0f, .b = 1.0f};
  settings->vertices_color = vertices3d_color;
  settings->line_width = 2.0f;
  settings->line_type = v_settings_line_solid;
  settings->vertex_size = 3.0f;
  settings->display_type = v_settings_vertex_display_circle;
  settings->projection_type = v_settings_projection_parallel;
}