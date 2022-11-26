#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageToasts : public Gtk::TemplateWidget<DemoPageToasts, Adw::Bin> {
  friend CppClassType;

public:
  void undo();

protected:
  explicit DemoPageToasts(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  void add_toast(const Glib::RefPtr<Adw::Toast> &toast);
  void toast_add_cb();
  void toast_add_with_button_cb();
  void toast_add_with_long_title_cb();
  void toast_dismiss_cb();

  static unsigned int signal_add_toast;

  Glib::RefPtr<Adw::Toast> undo_toast;
  int toast_undo_items = 0;
};

} // namespace Adw
