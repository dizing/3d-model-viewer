#include "viewer_widget.h"

viewer_widget::viewer_widget(QWidget *parent) : QOpenGLWidget{parent} {
  setFocusPolicy(Qt::ClickFocus);
}

viewer_widget::~viewer_widget() {
  if (vertices_copy_ != nullptr) {
    delete[] vertices_copy_;
  }
  if (ModelInitialized_) {
    delete_model(&model_);
  }
}

void viewer_widget::changeModel(v_model model) {
  if (ModelInitialized_) {
    delete_model(&model_);
  }
  ModelInitialized_ = true;
  model_ = model;
  if (vertices_copy_ != nullptr) {
    delete[] vertices_copy_;
  }
  vertices_copy_ = new v_vectorf4d[model_.vertices_count];
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned int) * model_.indices_count, model_.indices,
               GL_DYNAMIC_DRAW);
  ResetState();
  updateVertexBuffer();
  update();
}

void viewer_widget::initializeGL() {
  initializeOpenGLFunctions();
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
}

void viewer_widget::resizeGL(int, int) {
  // if (a > b) {
  //   resize(b, b);
  // } else {
  //   resize(a, a);
  // }
}

void viewer_widget::enable_settings() {
  glLineWidth(settings_.line_width);
  glClearColor(settings_.background_color.r, settings_.background_color.g,
               settings_.background_color.b, 1.0f);
  glColor3f(settings_.line_color.r, settings_.line_color.g,
            settings_.line_color.b);  // color of vertices
  glLineStipple(1, 0x0101);
  if (settings_.line_type == v_settings_line_dotted) {
    glEnable(GL_LINE_STIPPLE);
  } else if (settings_.line_type == v_settings_line_solid) {
    glDisable(GL_LINE_STIPPLE);
  }
  glPointSize(settings_.vertex_size);
  if (settings_.display_type == v_settings_vertex_display_circle) {
    glEnable(GL_POINT_SMOOTH);
  } else if (settings_.display_type == v_settings_vertex_display_square) {
    glDisable(GL_POINT_SMOOTH);
  }
  if (settings_.projection_type == v_settings_projection_central) {
    projection_matrix_ = generate_identity();
    view_matrix_ = generate_identity();
  } else if (settings_.projection_type == v_settings_projection_parallel) {
    projection_matrix_ =
        generate_frustrum_matrix(-0.065, 0.065, -0.065, 0.065, 0.1, 2.0);
    view_matrix_ =
        generate_translation_matrix({.x = 0.0f, .y = 0.0f, .z = -1.0f});
  }
  if (ModelInitialized_) {
    updateVertexBuffer();
  }
}

void viewer_widget::paintGL() {
  enable_settings();

  glClear(GL_COLOR_BUFFER_BIT);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  if (ModelInitialized_) {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(settings_.line_color.r, settings_.line_color.g,
              settings_.line_color.b);  // color of vertices
    glDrawElements(GL_TRIANGLES, model_.indices_count, GL_UNSIGNED_INT, 0);
    if (settings_.display_type != v_settings_vertex_display_no) {
      glColor3f(settings_.vertices_color.r, settings_.vertices_color.g,
                settings_.vertices_color.b);  // color of points
      glDrawArrays(GL_POINTS, 0, model_.vertices_count);
    }
    glDisableVertexAttribArray(0);
  }
}

void viewer_widget::updateVertexBuffer() {
  v_matrix4x4 result_matrix =
      matrix_mult_4x4(view_matrix_, compileModelMatrix());
  result_matrix = matrix_mult_4x4(projection_matrix_, result_matrix);
  if (ModelInitialized_) {
    f4d_vertex_array_processing(model_.vertices4d, vertices_copy_,
                                model_.vertices_count, result_matrix);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * model_.vertices_count,
                 vertices_copy_, GL_DYNAMIC_DRAW);
  }
}

void viewer_widget::updateSettings(v_settings settings) {
  settings_ = settings;
  update();
}

void viewer_widget::wheelEvent(QWheelEvent *event) {
  QPoint numDegrees = event->angleDelta() / 8;
  if (numDegrees.y() >= 15) {  // Maximize
    scale_ *= 1.1;
  } else if (numDegrees.y() <= -15) {  // Minimize
    scale_ *= 0.9;
  }
  updateVertexBuffer();
  update();
  emit changeScaling(scale_);
  event->accept();
}

void viewer_widget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    dragging = true;
    last_x = event->pos().x();
    last_y = event->pos().y();
  }
  event->accept();
}

void viewer_widget::mouseMoveEvent(QMouseEvent *event) {
  if ((event->buttons() & Qt::LeftButton) && dragging) {
    rotation_angles_.y += (event->pos().y() - last_y) / 100.0f;
    rotation_angles_.x += (event->pos().x() - last_x) / 100.0f;
    updateVertexBuffer();
    update();
    last_x = event->pos().x();
    last_y = event->pos().y();
  }
  emit changeRotationAngles(rotation_angles_);
  event->accept();
}

void viewer_widget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton && dragging) {
    dragging = false;
  }
  event->accept();
}

void viewer_widget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_W) {
    translation_vector_.y += 0.1f;
  } else if (event->key() == Qt::Key_S) {
    translation_vector_.y -= 0.1f;
  } else if (event->key() == Qt::Key_A) {
    translation_vector_.x -= 0.1f;
  } else if (event->key() == Qt::Key_D) {
    translation_vector_.x += 0.1f;
  } else {
    return;
  }
  emit changeTranslation(translation_vector_);
  updateVertexBuffer();
  update();
}

v_matrix4x4 viewer_widget::compileModelMatrix() {
  v_matrix4x4 matrix = generate_XYZaxis_rotation_matrix(
      rotation_angles_.y, rotation_angles_.x, rotation_angles_.z);
  matrix =
      matrix_mult_4x4(matrix, generate_scale_matrix(scale_, scale_, scale_));
  matrix =
      matrix_mult_4x4(matrix, generate_translation_matrix(translation_vector_));
  return matrix;
}

void viewer_widget::RotationXChangeOutside(int value) {
  rotation_angles_.x = value * M_PI / 180.0f;
  updateVertexBuffer();
  update();
  emit changeRotationAngles(rotation_angles_);
}
void viewer_widget::RotationYChangeOutside(int value) {
  rotation_angles_.y = value * M_PI / 180.0f;
  updateVertexBuffer();
  update();
  emit changeRotationAngles(rotation_angles_);
}
void viewer_widget::RotationZChangeOutside(int value) {
  rotation_angles_.z = value * M_PI / 180.0f;
  updateVertexBuffer();
  update();
  emit changeRotationAngles(rotation_angles_);
}

void viewer_widget::ScalingChangeOutside(int value) {
  scale_ = value / FACTOR_SCALE_SLIDER;
  updateVertexBuffer();
  update();
  emit changeScaling(scale_);
}

void viewer_widget::TranslationXChangeOutside(float value) {
  translation_vector_.x = value;
  updateVertexBuffer();
  update();
  emit changeTranslation(translation_vector_);
}
void viewer_widget::TranslationYChangeOutside(float value) {
  translation_vector_.y = value;
  updateVertexBuffer();
  update();
  emit changeTranslation(translation_vector_);
}
void viewer_widget::TranslationZChangeOutside(float value) {
  translation_vector_.z = value;
  updateVertexBuffer();
  update();
  emit changeTranslation(translation_vector_);
}

void viewer_widget::ResetState() {
  scale_ = 1.0f;
  rotation_angles_.x = 0.0f;
  rotation_angles_.y = 0.0f;
  rotation_angles_.z = 0.0f;
  translation_vector_.x = 0.0f;
  translation_vector_.y = 0.0f;
  translation_vector_.z = 0.0f;
  emit changeRotationAngles(rotation_angles_);
  emit changeScaling(scale_);
  emit changeTranslation(translation_vector_);
}