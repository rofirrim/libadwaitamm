#pragma once

#include "templatewidget.h"
#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>

namespace Adw {

class DemoPageAbout : public Gtk::TemplateWidget<DemoPageAbout, Adw::Bin> {
  friend CppClassType;

protected:
  explicit DemoPageAbout(GtkWidget *obj);

private:
  void demo_run();

  static void setup_template(Gtk::TemplateWidgetSetup &s);
  static const char class_name[];
  void init_widget(Gtk::TemplateWidgetInit &i);
};

} // namespace Adw
