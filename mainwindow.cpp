#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  settings_widget = new Settings_widget(ui->view_field);
  QPalette palette = ui->menubar->palette();
  palette.setColor(QPalette::ButtonText, Qt::black);
  palette.setColor(QPalette::Text, Qt::black);
  ui->menubar->addAction("Settings", this, SLOT(SettingsClicked()));
  ui->menubar->setPalette(palette);
  ui->menubar->setStyleSheet("QMenu { color: black; }");

  connect(ui->actionOpen, SIGNAL(triggered()), SLOT(FileOpenClicked()));
  connect(ui->action_save_image, SIGNAL(triggered()),
          SLOT(SaveAsImageClicked()));
  connect(ui->action_save_gif, SIGNAL(triggered()), SLOT(SaveAsGifClicked()));
  connect(ui->view_field, SIGNAL(changeRotationAngles(v_vectorf3d)),
          SLOT(RotationsAnglesChanged(v_vectorf3d)));
  connect(ui->rotation_slider_x, SIGNAL(valueChanged(int)), ui->view_field,
          SLOT(RotationXChangeOutside(int)));
  connect(ui->rotation_slider_y, SIGNAL(valueChanged(int)), ui->view_field,
          SLOT(RotationYChangeOutside(int)));
  connect(ui->rotation_slider_z, SIGNAL(valueChanged(int)), ui->view_field,
          SLOT(RotationZChangeOutside(int)));
  connect(ui->view_field, SIGNAL(changeScaling(float)),
          SLOT(ScalingChanged(float)));
  connect(ui->scaling_slider, SIGNAL(valueChanged(int)), ui->view_field,
          SLOT(ScalingChangeOutside(int)));
  connect(ui->view_field, SIGNAL(changeTranslation(v_vectorf3d)),
          SLOT(TranslationChanged(v_vectorf3d)));

  connect(ui->translation_slider_x, SIGNAL(valueChanged(int)),
          SLOT(translationSliderXHandler(int)));
  connect(ui->translation_slider_y, SIGNAL(valueChanged(int)),
          SLOT(translationSliderYHandler(int)));
  connect(ui->translation_slider_z, SIGNAL(valueChanged(int)),
          SLOT(translationSliderZHandler(int)));

  connect(this, SIGNAL(translationSliderXChanged(float)), ui->view_field,
          SLOT(TranslationXChangeOutside(float)));
  connect(this, SIGNAL(translationSliderYChanged(float)), ui->view_field,
          SLOT(TranslationYChangeOutside(float)));
  connect(this, SIGNAL(translationSliderZChanged(float)), ui->view_field,
          SLOT(TranslationZChangeOutside(float)));

  connect(ui->reset_button, SIGNAL(clicked()), SLOT(ResetAllValues()));
  ResetAllValues();
}

MainWindow::~MainWindow() {
  delete settings_widget;
  delete ui;
}

void MainWindow::SettingsClicked() {
  if (!settings_widget->isVisible()) {
    settings_widget->show();
  } else {
    settings_widget->hide();
  }
}

void MainWindow::closeEvent(QCloseEvent *) { settings_widget->close(); }

void MainWindow::FileOpenClicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open Model"), OBJECTS_PATH, tr("Object files (*.obj)"));

  if (!fileName.isNull()) {
    v_model model = initialize_model(fileName.toLatin1().constData());
    if (model.error_code == 0) {
      ui->view_field->changeModel(model);
      QString base = QFileInfo(fileName).baseName();
      ui->file_name_label->setText("File name: " + base);
      ui->vertices_number_label->setText(
          QString("Vertices number: %1").arg(model.vertices_count));
      ui->edges_number_label->setText(
          QString("Edges number: %1").arg(model.indices_count * 3));
    } else {
      QMessageBox msgBox;
      char error_str[35];
      sprintf(error_str, "Error in model loading(code %d)", model.error_code);
      msgBox.setText(error_str);
      msgBox.exec();
    }
  }
}

void MainWindow::SaveAsImageClicked() {
  QImage image = ui->view_field->grabFramebuffer();
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/",
                                                  tr("Images (*.bmp *.jpeg)"));
  image.save(fileName);
}

void MainWindow::SaveAsGifClicked() {
  gif_iterator_ = 0;
  gif_ = new QGifImage(QSize(640, 480));
  gif_->setDefaultDelay(100);
  startTimer(100);
}
void MainWindow::timerEvent(QTimerEvent *event) {
  gif_->addFrame(ui->view_field->grabFramebuffer().scaled(
      QSize(640, 480), Qt::IgnoreAspectRatio));
  gif_iterator_++;
  if (gif_iterator_ == 50) {  // 10 fps 5 seconds
    killTimer(event->timerId());
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/",
                                                    tr("Images (*.gif)"));
    gif_->save(fileName);
    delete gif_;
    gif_ = NULL;
  }
}

void MainWindow::RotationsAnglesChanged(v_vectorf3d rotation_angles) {
  char str_x[10];
  char str_y[10];
  char str_z[10];
  double x_angle = std::fmod(rotation_angles.x, M_PI * 2) * (180.0 / M_PI);
  double y_angle = std::fmod(rotation_angles.y, M_PI * 2) * (180.0 / M_PI);
  double z_angle = std::fmod(rotation_angles.z, M_PI * 2) * (180.0 / M_PI);
  x_angle = (x_angle > -0.001) ? x_angle : 360 + x_angle;
  y_angle = (y_angle > -0.001) ? y_angle : 360 + y_angle;
  z_angle = (z_angle > -0.001) ? z_angle : 360 + z_angle;
  sprintf(str_x, "%.2f", x_angle);
  sprintf(str_y, "%.2f", y_angle);
  sprintf(str_z, "%.2f", z_angle);
  ui->rotation_label_x->setText(str_x);
  ui->rotation_label_y->setText(str_y);
  ui->rotation_label_z->setText(str_z);
  ui->rotation_slider_x->blockSignals(true);
  ui->rotation_slider_y->blockSignals(true);
  ui->rotation_slider_z->blockSignals(true);
  ui->rotation_slider_x->setValue(x_angle);
  ui->rotation_slider_y->setValue(y_angle);
  ui->rotation_slider_z->setValue(z_angle);
  ui->rotation_slider_x->blockSignals(false);
  ui->rotation_slider_y->blockSignals(false);
  ui->rotation_slider_z->blockSignals(false);
}

void MainWindow::ScalingChanged(float scale) {
  char str[10];
  sprintf(str, "%.2f", scale);
  ui->scaling_label->setText(str);
  bool oldState = ui->scaling_slider->blockSignals(true);
  ui->scaling_slider->setValue(scale * 50);
  ui->scaling_slider->blockSignals(oldState);
}

void MainWindow::TranslationChanged(v_vectorf3d translation_vector) {
  char str_x[10];
  char str_y[10];
  char str_z[10];
  sprintf(str_x, "%.2f", translation_vector.x);
  sprintf(str_y, "%.2f", translation_vector.y);
  sprintf(str_z, "%.2f", translation_vector.z);
  ui->translation_label_x->setText(str_x);
  ui->translation_label_y->setText(str_y);
  ui->translation_label_z->setText(str_z);
  ui->translation_slider_x->blockSignals(true);
  ui->translation_slider_y->blockSignals(true);
  ui->translation_slider_z->blockSignals(true);
  ui->translation_slider_x->setValue(HALF_SCALE_SLIDER +
                                     translation_vector.x * HALF_SCALE_SLIDER);
  ui->translation_slider_y->setValue(HALF_SCALE_SLIDER +
                                     translation_vector.y * HALF_SCALE_SLIDER);
  ui->translation_slider_z->setValue(HALF_SCALE_SLIDER +
                                     translation_vector.z * HALF_SCALE_SLIDER);
  ui->translation_slider_x->blockSignals(false);
  ui->translation_slider_y->blockSignals(false);
  ui->translation_slider_z->blockSignals(false);
}

void MainWindow::translationSliderXHandler(int value) {
  float real_value = (value - HALF_SCALE_SLIDER) / HALF_SCALE_SLIDER;
  emit translationSliderXChanged(real_value);
}
void MainWindow::translationSliderYHandler(int value) {
  float real_value = (value - HALF_SCALE_SLIDER) / HALF_SCALE_SLIDER;
  emit translationSliderYChanged(real_value);
}
void MainWindow::translationSliderZHandler(int value) {
  float real_value = (value - HALF_SCALE_SLIDER) / HALF_SCALE_SLIDER;
  emit translationSliderZChanged(real_value);
}

void MainWindow::ResetAllValues() {
  ui->rotation_slider_x->setValue(0.0f);
  ui->rotation_slider_y->setValue(0.0f);
  ui->rotation_slider_z->setValue(0.0f);
  ui->scaling_slider->setValue(HALF_SCALE_SLIDER);
  ui->translation_slider_x->setValue(HALF_SCALE_SLIDER);
  ui->translation_slider_y->setValue(HALF_SCALE_SLIDER);
  ui->translation_slider_z->setValue(HALF_SCALE_SLIDER);
}
