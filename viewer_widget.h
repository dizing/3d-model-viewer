#ifndef VIEWER_WIDGET_H
#define VIEWER_WIDGET_H
#define GL_SILENCE_DEPRECATION
#define FACTOR_SCALE_SLIDER 50.0f
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QWheelEvent>
#include <QtGlobal>

extern "C" {
#include "model.h"
#include "settings_handler.h"
#include "transf_matrix_generator.h"
#include "vertex_processor.h"
#include "viewer_blas.h"
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLWidget>
#else
#include <QOpenGLFunctions>
#include <QtOpenGLWidgets/QOpenGLWidget>
#endif

class viewer_widget : public QOpenGLWidget,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                      protected QOpenGLFunctions_3_3_Compatibility
#else
                      protected QOpenGLFunctions
#endif
{
  Q_OBJECT

 public:
  viewer_widget(QWidget *parent = nullptr);
  ~viewer_widget();
  GLuint VBO;
  GLuint EBO;
  void changeModel(v_model model);
 signals:
  void changeRotationAngles(v_vectorf3d);
  void changeScaling(float);
  void changeTranslation(v_vectorf3d);
 private slots:
  void RotationXChangeOutside(int value);
  void RotationYChangeOutside(int value);
  void RotationZChangeOutside(int value);
  void ScalingChangeOutside(int value);
  void TranslationXChangeOutside(float value);
  void TranslationYChangeOutside(float value);
  void TranslationZChangeOutside(float value);
  void updateSettings(v_settings settings);

 protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void wheelEvent(QWheelEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void updateVertexBuffer();
  void enable_settings();
  v_matrix4x4 compileModelMatrix();
  void ResetState();

 private:
  v_model model_;
  bool ModelInitialized_ = false;
  v_vectorf4d *vertices_copy_ = nullptr;
  float scale_;
  bool dragging = false;
  int last_x, last_y;
  v_settings settings_ = get_standard_settings();
  v_matrix4x4 projection_matrix_;
  v_matrix4x4 view_matrix_;
  v_vectorf3d rotation_angles_;
  v_vectorf3d translation_vector_;
};

#endif  // VIEWER_WIDGET_H
