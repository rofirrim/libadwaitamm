#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageTabView : public Gtk::TemplateWidget<DemoPageTabView, Adw::Bin> {
  friend CppClassType;

protected:
  explicit DemoPageTabView(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  void demo_run_cb();
};

} // namespace Adw
