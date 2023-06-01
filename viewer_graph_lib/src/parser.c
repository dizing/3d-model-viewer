#include "parser.h"

int add_to_vector(Vector *vertexes, char str[MAX_NUM]) {
  int error = OK;
  int number_count = 0;
  char *part = strtok(str, SEP);
  double number;
  while (part != NULL && !error) {
    part = strtok(NULL, SEP);
    if (part && str[0] == VECTOR) {
      if (sscanf(part, "%lf", &number)) {
        vector_add_item(vertexes, number);
      } else {
        error = ERROR_V;
      }
      if (!error) number_count++;
    }
  }
  if (number_count < 3) {
    error = ERROR;
  } else if (number_count > 4 && str[0] == VECTOR) {
    error = ERROR_V;
  }
  return error;
}

int parse_face(Vector *vert_indexes, char str[MAX_NUM]) {
  str[strlen(str) + 1] = '\0';
  int error_status = 0;
  int i = 1;
  int state = 2;
  int number_start = 0;
  int number = 0;
  int first_index = 0;
  int isFirstIndex = 0;
  int last_index = 0;
  int isLastIndex = 0;
  int isNumber = 0;
  while (!error_status && (str[i] || str[i - 1])) {
    switch (state) {
      case 1:
        if (!isdigit(str[i]) || str[i] != '-') {
          state = 2;
          if (!isFirstIndex) {
            sscanf(str + number_start, "%d", &first_index);
            isFirstIndex = 1;
          } else if (!isLastIndex) {
            sscanf(str + number_start, "%d", &last_index);
            isLastIndex = 1;
          } else {
            sscanf(str + number_start, "%d", &number);
            vector_add_item(vert_indexes, first_index);
            vector_add_item(vert_indexes, last_index);
            vector_add_item(vert_indexes, number);
            isNumber = 1;
            last_index = number;
          }
          if (error_status) {
            break;
          }
        } else {
          ++i;
        }
        break;
      case 2:
        if (str[i] == ' ') {
          state = 3;
        }
        ++i;
        break;
      case 3:
        if (isdigit(str[i]) || str[i] == '-') {
          state = 1;
          number_start = i;
        }
        ++i;
        break;
    }
  }
  if (!isNumber) {
    error_status = ERROR;
  }
  return error_status;
}

void vertexes_to_model(Vector *vertexes, v_model *model) {
  model->vertices_count = vertexes->size / 3;
  v_vectorf3d *vertices3d = malloc(sizeof(v_vectorf3d) * model->vertices_count);
  for (size_t i = 0, j = 0; i < model->vertices_count; ++i) {
    vertices3d[i].x = vertexes->data[j++];
    vertices3d[i].y = vertexes->data[j++];
    vertices3d[i].z = vertexes->data[j++];
  }
  model->vertices3d = vertices3d;
}

void indexes_to_model(Vector *vertex_indexes, v_model *model) {
  unsigned int *indices = malloc((vertex_indexes->size) * sizeof(int));
  if (model->vertices_count < 1) model->error_code = ERROR_V;
  for (size_t i = 0; i < vertex_indexes->size && !model->error_code; ++i) {
    int index = (int)vertex_indexes->data[i];
    if (index > (int)model->vertices_count) {
      index = (index % model->vertices_count) - 1;
    } else if (index > 0) {
      index -= 1;
    } else if (index == 0) {
      model->error_code = ERROR_F;
    }
    if (index < 0) {
      index = model->vertices_count + index;
    }
    indices[i] = (unsigned)index;
  }
  model->indices = indices;
  model->indices_count = vertex_indexes->size;
}

int file_exists(const char *filename) {
  int error = OK;
  FILE *file = fopen(filename, "r");
  if (!file) {
    error = ERROR_FILE;
  } else {
    fclose(file);
  }
  return error;
}

int file_empty(const char *filename) {
  int error = OK;
  FILE *file = fopen(filename, "r");
  if (getc(file) == EOF) error = EMPTY_FILE;
  fclose(file);
  return error;
}

void process(v_model *model) {
  model->error_code = OK;
  FILE *file = fopen(model->file_path, "r");
  char str[MAX_NUM];
  char *estr;
  Vector *vertexes = vector_new(START_CAPACITY);
  Vector *vertex_indexes = vector_new(START_CAPACITY);
  estr = fgets(str, sizeof(str), file);
  if (!estr) model->error_code = ERROR_FILE;
  while (estr && !model->error_code) {
    if (str[0] == VECTOR && str[1] == SPACE) {
      model->error_code = add_to_vector(vertexes, str);
    } else if (str[0] == FACE) {
      model->error_code = parse_face(vertex_indexes, str);
    }
    estr = fgets(str, sizeof(str), file);
  }
  if (!model->error_code) {
    vertexes_to_model(vertexes, model);
    indexes_to_model(vertex_indexes, model);
  }
  fclose(file);
  vector_free(vertexes);
  vector_free(vertex_indexes);
}

void parser(v_model *model) {
  setlocale(LC_ALL, "C");
  model->error_code = file_exists(model->file_path);
  if (!model->error_code) model->error_code = file_empty(model->file_path);
  if (!model->error_code) process(model);
}
