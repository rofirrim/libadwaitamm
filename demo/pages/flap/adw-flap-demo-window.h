#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/window_p.h>
#include "templatewidget.h"

namespace Adw {

class FlapDemoWindow : public Gtk::TemplateWidget<FlapDemoWindow, Adw::Window> {
  friend CppClassType;

public:
  static FlapDemoWindow *create();

protected:
  explicit FlapDemoWindow(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  // This is a window so it is not managed.
  static bool is_managed() { return false; }

  void stack_notify_visible_child_cb();
  void start_toggle_button_toggled_cb(GtkToggleButton *button_);

  Adw::Flap *flap;
  Gtk::Widget *reveal_btn_start;
  Gtk::Widget *reveal_btn_end;
};

} // namespace Adw
