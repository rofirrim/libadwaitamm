#pragma once

#include <libadwaitamm.h>

#include "templatewidget.h"

namespace Adw {

class DemoPreferencesWindow
    : public Gtk::TemplateWidget<DemoPreferencesWindow,
                                 Adw::PreferencesWindow> {
public:
  static DemoPreferencesWindow *create();

private:
  explicit DemoPreferencesWindow(GtkWidget *obj);

  void subpage1_activated();
  void subpage2_activated();
  void return_to_preferences();
  void toast_show();

  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  Gtk::Widget *subpage1;
  Gtk::Widget *subpage2;

  friend CppClassType;
};
} // namespace Adw
