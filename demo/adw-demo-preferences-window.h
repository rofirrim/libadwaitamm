#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/preferenceswindow_p.h>

#include "templatewidget.h"

namespace Adw {

class DemoPreferencesWindow_Class;
class DemoPreferencesWindow;

class DemoPreferencesWindow_Class
    : public Gtk::TemplateWidgetClass<
          DemoPreferencesWindow_Class, DemoPreferencesWindow,
          Adw::PreferencesWindow_Class, adw_preferences_window_get_type> {
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  static const char class_name[];

  friend TemplateWidgetClassBase;
};

class DemoPreferencesWindow
    : public Gtk::TemplateWidget<DemoPreferencesWindow_Class,
                                 DemoPreferencesWindow, Adw::PreferencesWindow,
                                 AdwPreferencesWindow> {
public:
  static DemoPreferencesWindow *create();

private:
  explicit DemoPreferencesWindow(GtkWidget *obj);

  void subpage1_activated();
  void subpage2_activated();
  void return_to_preferences();
  void toast_show();

  void init_widget(Gtk::TemplateWidgetInit &i);

  Gtk::Widget *subpage1;
  Gtk::Widget *subpage2;

  friend DemoPreferencesWindow_Class;
  friend DemoPreferencesWindow_Class::TemplateWidgetClassBase;
};
} // namespace Adw
