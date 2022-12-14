#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageAbout : public Gtk::TemplateWidget<DemoPageAbout, Adw::Bin> {
  friend CppClassType;

protected:
  explicit DemoPageAbout(GtkWidget *obj) : TemplateWidgetBase(obj) { }

private:
  void demo_run();

  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);
};

} // namespace Adw
