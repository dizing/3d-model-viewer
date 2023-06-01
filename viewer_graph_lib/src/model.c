#include "model.h"

v_model initialize_model(const char *file_path) {
  v_model model;
  model.vertices3d = NULL;
  model.vertices4d = NULL;
  model.file_path = file_path;
  model.error_code = 0;
  parser(&model);

  if (model.error_code == 0) {
    normalize_model(&model);
  }
  if (model.error_code == 0) {
    generate4d_from3d(&model);
  }
  return model;
}

void normalize_model(v_model *model) {
  float max_x, min_x;
  max_x = min_x = model->vertices3d[0].x;
  float max_y, min_y;
  max_y = min_y = model->vertices3d[0].y;
  float max_z, min_z;
  max_z = min_z = model->vertices3d[0].z;

  for (size_t i = 1; i < model->vertices_count; ++i) {
    max_x = max(max_x, model->vertices3d[i].x);
    min_x = min(min_x, model->vertices3d[i].x);
    max_y = max(max_y, model->vertices3d[i].y);
    min_y = min(min_y, model->vertices3d[i].y);
    max_z = max(max_z, model->vertices3d[i].z);
    min_z = min(min_z, model->vertices3d[i].z);
  }
  float all_max = max(max(max_x, max_y), max_z);
  float all_min = min(min(min_x, min_y), min_z);
  float all_max_abs = max(fabsf(all_max), fabsf(all_min));
  // first centralize then normalize(-1 .. 1) then make slightly smaller
  for (size_t i = 0; i < model->vertices_count && all_max_abs > 1; ++i) {
    model->vertices3d[i].x = model->vertices3d[i].x - (max_x + min_x) / 2;
    model->vertices3d[i].x /= all_max_abs;
    model->vertices3d[i].x *= MINIMIZE_FACTOR;
    model->vertices3d[i].y = model->vertices3d[i].y - (max_y + min_y) / 2;
    model->vertices3d[i].y /= all_max_abs;
    model->vertices3d[i].y *= MINIMIZE_FACTOR;
    model->vertices3d[i].z = model->vertices3d[i].z - (max_z + min_z) / 2;
    model->vertices3d[i].z /= all_max_abs;
    model->vertices3d[i].z *= MINIMIZE_FACTOR;
  }
}

void generate4d_from3d(v_model *model) {
  model->vertices4d = malloc(sizeof(v_vectorf4d) * model->vertices_count);
  for (size_t i = 0; i < model->vertices_count; ++i) {
    model->vertices4d[i] = get4d_by3d(model->vertices3d[i]);
  }
}

void delete_model(v_model *model) {
  if (model->indices) {
    free(model->indices);
    model->indices = NULL;
  }
  if (model->vertices3d != NULL) {
    free(model->vertices3d);
    model->vertices3d = NULL;
  };
  if (model->vertices4d != NULL) {
    free(model->vertices4d);
    model->vertices4d = NULL;
  };
}

float max(float a, float b) { return (a > b) ? a : b; }

float min(float a, float b) { return (a > b) ? b : a; }