#include <check.h>
#include <stdio.h>

#define EPSILON 1e-6

#include "../settings_path.h"
#include "parser.h"
#include "transf_matrix_generator.h"
#include "vertex_processor.h"

START_TEST(test_error_1) {
  char path[256] = OBJECTS_PATH;
  strcat(path, "/test_ERROR.obj");
  v_model model;
  model.vertices3d = NULL;
  model.vertices4d = NULL;
  model.error_code = 0;
  model.file_path = path;
  parser(&model);
  int error = model.error_code;
  ck_assert_int_eq(error, ERROR);
}
END_TEST

START_TEST(test_error_2) {
  char path[256] = OBJECTS_PATH;
  strcat(path, "/test_ERROR_FILE.obj");
  v_model model;
  model.vertices3d = NULL;
  model.vertices4d = NULL;
  model.error_code = 0;
  model.file_path = path;
  parser(&model);
  int error = model.error_code;
  ck_assert_int_eq(error, ERROR_FILE);
}
END_TEST

START_TEST(test_error_3) {
  char path[256] = OBJECTS_PATH;
  strcat(path, "/test_ERROR_V.obj");
  v_model model;
  model.vertices3d = NULL;
  model.vertices4d = NULL;
  model.error_code = 0;
  model.file_path = path;
  parser(&model);
  int error = model.error_code;
  ck_assert_int_eq(error, ERROR_V);
}
END_TEST

START_TEST(test_error_4) {
  char path[256] = OBJECTS_PATH;
  strcat(path, "/test_EMPTY_FILE.obj");
  v_model model;
  model.vertices3d = NULL;
  model.vertices4d = NULL;
  model.error_code = 0;
  model.file_path = path;
  parser(&model);
  int error = model.error_code;
  ck_assert_int_eq(error, EMPTY_FILE);
}
END_TEST

START_TEST(test_1) {
  char path[256] = OBJECTS_PATH;
  strcat(path, "/test.obj");
  v_model model;
  model.vertices3d = NULL;
  model.vertices4d = NULL;
  model.error_code = 0;
  model.file_path = path;
  parser(&model);
  int error = model.error_code;
  ck_assert_int_eq(error, OK);
  double real_result_vert[100] = {0, 0,      0, 1,   0,    0,
                                  0, 100e-5, 0, 0.5, -0.5, 1.6};
  unsigned int real_result_ind[100] = {0, 1, 2, 3, 0, 2, 3, 1, 0, 3, 0, 0,
                                       2, 1, 2, 2, 2, 3, 2, 3, 0, 2, 0, 3,
                                       2, 3, 0, 3, 1, 0, 3, 0, 3, 2, 1, 2};
  int j = 0;
  for (size_t i = 0; i < model.vertices_count; ++i) {
    ck_assert_double_eq_tol(real_result_vert[j++], model.vertices3d[i].x,
                            EPSILON);
    ck_assert_double_eq_tol(real_result_vert[j++], model.vertices3d[i].y,
                            EPSILON);
    ck_assert_double_eq_tol(real_result_vert[j++], model.vertices3d[i].z,
                            EPSILON);
  }
  for (size_t i = 0; i < model.indices_count; ++i) {
    ck_assert_uint_eq(real_result_ind[i], model.indices[i]);
  }
  delete_model(&model);
}
END_TEST

START_TEST(test_2) {
  char path[256] = OBJECTS_PATH;
  strcat(path, "/test_triangle.obj");
  v_model model = initialize_model(path);
  v_vectorf4d real_result[] = {
      {.x = 0.475f, .y = -0.475f, .z = -0.475f, .w = 1.0f},
      {.x = -0.475f, .y = 0.475f, .z = -0.475f, .w = 1.0f},
      {.x = -0.475f, .y = -0.475f, .z = 0.475f, .w = 1.0f}};
  int error = model.error_code;
  ck_assert_int_eq(error, OK);
  for (int i = 0; i < 3; ++i) {
    ck_assert_double_eq_tol(model.vertices4d[i].x, real_result[i].x, EPSILON);
    ck_assert_double_eq_tol(model.vertices4d[i].y, real_result[i].y, EPSILON);
    ck_assert_double_eq_tol(model.vertices4d[i].z, real_result[i].z, EPSILON);
    ck_assert_double_eq_tol(model.vertices4d[i].w, real_result[i].w, EPSILON);
  }
  delete_model(&model);
}
END_TEST

START_TEST(test_3) {
  v_vectorf4d a[] = {{.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
                     {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 1.0f},
                     {.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 1.0f}};
  v_matrix4x4 rotation = generate_XYZaxis_rotation_matrix(M_PI, 0.0f, 0.0f);
  v_vectorf4d b[3];
  f4d_vertex_array_processing(a, b, 3, rotation);
  v_vectorf4d real_result[] = {{.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
                               {.x = 0.0f, .y = -1.0f, .z = 0.0f, .w = 1.0f},
                               {.x = 0.0f, .y = 0.0f, .z = -1.0f, .w = 1.0f}};
  for (int i = 0; i < 3; ++i) {
    ck_assert_double_eq_tol(b[i].x, real_result[i].x, EPSILON);
    ck_assert_double_eq_tol(b[i].y, real_result[i].y, EPSILON);
    ck_assert_double_eq_tol(b[i].z, real_result[i].z, EPSILON);
    ck_assert_double_eq_tol(b[i].w, real_result[i].w, EPSILON);
  }
}
END_TEST

START_TEST(test_4) {
  v_vectorf4d a[] = {{.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
                     {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 1.0f},
                     {.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 1.0f}};
  v_matrix4x4 scale_matrix = generate_scale_matrix(0.5f, 0.5f, 0.5f);
  v_vectorf4d b[3];
  f4d_vertex_array_processing(a, b, 3, scale_matrix);
  v_vectorf4d real_result[] = {{.x = 0.5f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
                               {.x = 0.0f, .y = 0.5f, .z = 0.0f, .w = 1.0f},
                               {.x = 0.0f, .y = 0.0f, .z = 0.5f, .w = 1.0f}};
  for (int i = 0; i < 3; ++i) {
    ck_assert_double_eq_tol(b[i].x, real_result[i].x, EPSILON);
    ck_assert_double_eq_tol(b[i].y, real_result[i].y, EPSILON);
    ck_assert_double_eq_tol(b[i].z, real_result[i].z, EPSILON);
    ck_assert_double_eq_tol(b[i].w, real_result[i].w, EPSILON);
  }
}
END_TEST

START_TEST(test_5) {
  v_vectorf4d a[] = {{.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
                     {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 1.0f},
                     {.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 1.0f}};
  v_vectorf3d x_right = {.x = 0.5f, .y = 0.0f, .z = 0.0f};
  v_matrix4x4 translation_matrix = generate_translation_matrix(x_right);
  v_vectorf4d b[3];
  f4d_vertex_array_processing(a, b, 3, translation_matrix);
  v_vectorf4d real_result[] = {{.x = 1.5f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
                               {.x = 0.5f, .y = 1.0f, .z = 0.0f, .w = 1.0f},
                               {.x = 0.5f, .y = 0.0f, .z = 1.0f, .w = 1.0f}};
  for (int i = 0; i < 3; ++i) {
    ck_assert_double_eq_tol(b[i].x, real_result[i].x, EPSILON);
    ck_assert_double_eq_tol(b[i].y, real_result[i].y, EPSILON);
    ck_assert_double_eq_tol(b[i].z, real_result[i].z, EPSILON);
    ck_assert_double_eq_tol(b[i].w, real_result[i].w, EPSILON);
  }
}
END_TEST

START_TEST(test_6) {
  v_matrix4x4 identity = generate_identity();
  v_vectorf4d real_result[] = {{.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f},
                               {.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f},
                               {.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f},
                               {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f}};
  v_vectorf4d *b = &(identity.row1);
  for (int i = 0; i < 4; ++i) {
    ck_assert_double_eq_tol(b[i].x, real_result[i].x, EPSILON);
    ck_assert_double_eq_tol(b[i].y, real_result[i].y, EPSILON);
    ck_assert_double_eq_tol(b[i].z, real_result[i].z, EPSILON);
    ck_assert_double_eq_tol(b[i].w, real_result[i].w, EPSILON);
  }
}
END_TEST

START_TEST(test_7) {
  v_vectorf4d a[] = {{.x = 1.0f, .y = 0.0f, .z = -1.5f, .w = 1.0f},
                     {.x = 0.0f, .y = 1.0f, .z = -0.5f, .w = 1.0f},
                     {.x = 0.0f, .y = 0.0f, .z = -1.0f, .w = 1.0f}};
  v_matrix4x4 frustrum_matrix =
      generate_frustrum_matrix(-0.065, 0.065, -0.065, 0.065, 0.1, 2.0);
  ;
  v_vectorf4d b[3];
  f4d_vertex_array_processing(a, b, 3, frustrum_matrix);
  v_vectorf4d real_result[] = {
      {.x = 0.615385f, .y = 0.0f, .z = 0.578947f, .w = 1.0f},
      {.x = 0.0f, .y = 1.025641f, .z = 0.228070f, .w = 1.0f},
      {.x = 0.0f, .y = 0.0f, .z = 0.447368f, .w = 1.0f}};
  for (int i = 0; i < 3; ++i) {
    ck_assert_double_eq_tol(b[i].x, real_result[i].x, EPSILON);
    ck_assert_double_eq_tol(b[i].y, real_result[i].y, EPSILON);
    ck_assert_double_eq_tol(b[i].z, real_result[i].z, EPSILON);
    ck_assert_double_eq_tol(b[i].w, real_result[i].w, EPSILON);
  }
}
END_TEST

int main(void) {
  Suite *s = suite_create("viewer");
  SRunner *sr = srunner_create(s);
  int number_failed;

  TCase *viewer = tcase_create("viewer");
  suite_add_tcase(s, viewer);
  tcase_add_test(viewer, test_error_1);
  tcase_add_test(viewer, test_error_2);
  tcase_add_test(viewer, test_error_3);
  tcase_add_test(viewer, test_error_4);

  tcase_add_test(viewer, test_1);
  tcase_add_test(viewer, test_2);
  tcase_add_test(viewer, test_3);
  tcase_add_test(viewer, test_4);
  tcase_add_test(viewer, test_5);
  tcase_add_test(viewer, test_6);
  tcase_add_test(viewer, test_7);

  srunner_run_all(sr, CK_ENV);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : -1;
}
