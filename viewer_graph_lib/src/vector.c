#include "vector.h"

Vector *vector_new(size_t initial_capacity) {
  Vector *v;
  v = malloc(sizeof(Vector));
  if (v != NULL) {
    v->data = malloc(initial_capacity * sizeof(double));
    if (v->data != NULL) {
      v->capacity = initial_capacity;
      v->size = 0;
    } else {
      vector_free(v);
      v = NULL;
    }
  }
  return v;
}

void vector_free(Vector *v) {
  if (v != NULL) {
    if (v->data != NULL) free(v->data);
    free(v);
  }
}

int vector_reserve(Vector *v, size_t new_capacity) {
  int error = 0;
  double *tmp;
  if ((tmp = realloc(v->data, new_capacity * sizeof(double))) != NULL) {
    v->data = tmp;
    v->capacity = new_capacity;
  } else {
    error = 1;
  }
  return error;
}

void vector_add_item(Vector *v, double item) {
  if (v->size == v->capacity) vector_reserve(v, 2 * v->capacity);
  v->data[v->size] = item;
  v->size++;
}