#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageButtons : public Gtk::TemplateWidget<DemoPageButtons, Adw::Bin> {
  friend CppClassType;

protected:
  explicit DemoPageButtons(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);
};

} // namespace Adw
