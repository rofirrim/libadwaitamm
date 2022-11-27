#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageViewSwitcher
    : public Gtk::TemplateWidget<DemoPageViewSwitcher, Adw::Bin> {
  friend CppClassType;

protected:
  DemoPageViewSwitcher(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  void demo_run_cb();
};

} // namespace Adw
