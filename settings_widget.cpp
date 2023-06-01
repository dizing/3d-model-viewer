#include "settings_widget.h"

#include "ui_settings_widget.h"

Settings_widget::Settings_widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Settings_widget) {
  ui->setupUi(this);
}

Settings_widget::Settings_widget(viewer_widget *view_field)
    : Settings_widget() {
  view_field_ = view_field;
  QPalette palette = ui->gridWidget_2->palette();
  ui->thickness_edges_box->setButtonSymbols(QAbstractSpinBox::PlusMinus);
  ui->size_vertices_box->setButtonSymbols(QAbstractSpinBox::PlusMinus);

  settings_ = read_settings(SETTINGS_PATH);
  InitializeElements();

  connect(ui->Type_proj_box, SIGNAL(currentIndexChanged(int)),
          SLOT(ProjectionTypeChanged(int)));
  connect(ui->Type_edg_box, SIGNAL(currentIndexChanged(int)),
          SLOT(EdgesTypeChanged(int)));
  connect(ui->Display_edg_box, SIGNAL(currentIndexChanged(int)),
          SLOT(DisplayTypeChanged(int)));

  connect(ui->background_color_button, SIGNAL(released()),
          SLOT(BackgroundColorButtonClicked()));
  connect(ui->edges_color_button, SIGNAL(released()),
          SLOT(EdgesColorButtonClicked()));
  connect(ui->vertices_color_button, SIGNAL(released()),
          SLOT(VerticesColorButtonClicked()));

  connect(ui->thickness_edges_box, SIGNAL(valueChanged(double)),
          SLOT(EdgesThicknessChanged(double)));
  connect(ui->size_vertices_box, SIGNAL(valueChanged(double)),
          SLOT(VerticesSizeChanged(double)));
  connect(this, SIGNAL(SettingsChanged(v_settings)), view_field_,
          SLOT(updateSettings(v_settings)));

  emit SettingsChanged(settings_);
}

Settings_widget::~Settings_widget() {
  delete ui;
  save_settings(SETTINGS_PATH, settings_);
}

void Settings_widget::ProjectionTypeChanged(int index) {
  if (index == 0) {
    settings_.projection_type = v_settings_projection_parallel;
  } else if (index == 1) {
    settings_.projection_type = v_settings_projection_central;
  }
  emit SettingsChanged(settings_);
}
void Settings_widget::EdgesTypeChanged(int index) {
  if (index == 0) {
    settings_.line_type = v_settings_line_solid;
  } else if (index == 1) {
    settings_.line_type = v_settings_line_dotted;
  }
  emit SettingsChanged(settings_);
}
void Settings_widget::DisplayTypeChanged(int index) {
  if (index == 0) {
    settings_.display_type = v_settings_vertex_display_no;
  } else if (index == 1) {
    settings_.display_type = v_settings_vertex_display_circle;
  } else if (index == 2) {
    settings_.display_type = v_settings_vertex_display_square;
  }
  emit SettingsChanged(settings_);
}

void Settings_widget::BackgroundColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  settings_.background_color.r = color.red() / 255.0f;
  settings_.background_color.g = color.green() / 255.0f;
  settings_.background_color.b = color.blue() / 255.0f;
  ChangeFrameColor(ui->background_color_frame, color.red(), color.green(),
                   color.blue());
  emit SettingsChanged(settings_);
}

void Settings_widget::EdgesColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  settings_.line_color.r = color.red() / 255.0f;
  settings_.line_color.g = color.green() / 255.0f;
  settings_.line_color.b = color.blue() / 255.0f;
  ChangeFrameColor(ui->edges_color_frame, color.red(), color.green(),
                   color.blue());
  emit SettingsChanged(settings_);
}

void Settings_widget::VerticesColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  settings_.vertices_color.r = color.red() / 255.0f;
  settings_.vertices_color.g = color.green() / 255.0f;
  settings_.vertices_color.b = color.blue() / 255.0f;
  ChangeFrameColor(ui->vertices_color_frame, color.red(), color.green(),
                   color.blue());
  emit SettingsChanged(settings_);
}

void Settings_widget::VerticesSizeChanged(double value) {
  settings_.vertex_size = value;
  emit SettingsChanged(settings_);
}
void Settings_widget::EdgesThicknessChanged(double value) {
  settings_.line_width = value;
  emit SettingsChanged(settings_);
}

void Settings_widget::ChangeFrameColor(QFrame *frame, int red, int green,
                                       int blue) {
  char str[40];
  sprintf(str, "background-color: rgb(%d,%d,%d)", red, green, blue);
  frame->setStyleSheet(str);
}

void Settings_widget::InitializeElements() {
  ChangeFrameColor(
      ui->background_color_frame, settings_.background_color.r * 255,
      settings_.background_color.g * 255, settings_.background_color.b * 255);
  ChangeFrameColor(ui->edges_color_frame, settings_.line_color.r * 255,
                   settings_.line_color.g * 255, settings_.line_color.b * 255);
  ChangeFrameColor(ui->vertices_color_frame, settings_.vertices_color.r * 255,
                   settings_.vertices_color.g * 255,
                   settings_.vertices_color.b * 255);
  ui->thickness_edges_box->setValue(settings_.line_width);
  ui->size_vertices_box->setValue(settings_.vertex_size);
  if (settings_.line_type == v_settings_line_dotted) {
    ui->Type_edg_box->setCurrentIndex(1);
  } else if (settings_.line_type == v_settings_line_solid) {
    ui->Type_edg_box->setCurrentIndex(0);
  }
  if (settings_.display_type == v_settings_vertex_display_circle) {
    ui->Display_edg_box->setCurrentIndex(1);
  } else if (settings_.display_type == v_settings_vertex_display_square) {
    ui->Display_edg_box->setCurrentIndex(2);
  } else if (settings_.display_type == v_settings_vertex_display_no) {
    ui->Display_edg_box->setCurrentIndex(0);
  }
  if (settings_.projection_type == v_settings_projection_central) {
    ui->Type_proj_box->setCurrentIndex(1);
  } else if (settings_.projection_type == v_settings_projection_parallel) {
    ui->Type_proj_box->setCurrentIndex(0);
  }
}
