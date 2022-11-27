#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/window_p.h>
#include "templatewidget.h"

namespace Adw {

class ViewSwitcherDemoWindow
    : public Gtk::TemplateWidget<ViewSwitcherDemoWindow, Adw::Window> {
  friend CppClassType;
public:
  static ViewSwitcherDemoWindow* create();

protected:
  explicit ViewSwitcherDemoWindow(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  static bool is_managed() { return false; }
};

} // namespace Adw
