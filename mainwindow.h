#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define HALF_SCALE_SLIDER 50.0f
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>

#include "qgifimage.h"
#include "settings_path.h"
#include "settings_widget.h"
extern "C" {
#include "model.h"
#include "settings_handler.h"
#include "transf_matrix_generator.h"
#include "vertex_processor.h"
#include "viewer_blas.h"
}
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void SettingsClicked();
  void FileOpenClicked();
  void SaveAsImageClicked();
  void SaveAsGifClicked();
  void timerEvent(QTimerEvent *event) override;
  void RotationsAnglesChanged(v_vectorf3d rotation_angles);
  void ScalingChanged(float scale);
  void TranslationChanged(v_vectorf3d translation_vector);
  void translationSliderXHandler(int value);
  void translationSliderYHandler(int value);
  void translationSliderZHandler(int value);

  void ResetAllValues();

 signals:
  // void SlidersChanged(v_vectorf3d);
  void translationSliderXChanged(float value);
  void translationSliderYChanged(float value);
  void translationSliderZChanged(float value);

 private:
  Ui::MainWindow *ui;
  Settings_widget *settings_widget;
  QGifImage *gif_;
  int gif_iterator_;

  // QWidget interface
 protected:
  void closeEvent(QCloseEvent *event) override;
};
#endif  // MAINWINDOW_H
