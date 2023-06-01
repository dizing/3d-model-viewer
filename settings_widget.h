#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QColorDialog>
#include <QFrame>
#include <QWidget>

#include "viewer_widget.h"
namespace Ui {
class Settings_widget;
}

#include "settings_path.h"

class Settings_widget : public QWidget {
  Q_OBJECT

 public:
  explicit Settings_widget(QWidget *parent = nullptr);
  explicit Settings_widget(viewer_widget *view_field);
  ~Settings_widget();

 private slots:
  void ProjectionTypeChanged(int index);
  void EdgesTypeChanged(int index);
  void DisplayTypeChanged(int index);
  void BackgroundColorButtonClicked();
  void EdgesColorButtonClicked();
  void VerticesColorButtonClicked();
  void VerticesSizeChanged(double value);
  void EdgesThicknessChanged(double value);

 signals:
  void SettingsChanged(v_settings);

 private:
  v_settings generate_settings_from_window();
  void ChangeFrameColor(QFrame *frame, int red, int green, int blue);
  void InitializeElements();
  Ui::Settings_widget *ui;
  viewer_widget *view_field_;
  v_settings settings_ = get_standard_settings();
};

#endif  // SETTINGS_WIDGET_H
