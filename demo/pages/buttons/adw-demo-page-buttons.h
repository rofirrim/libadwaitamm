#pragma once

#include "templatewidget.h"
#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>

namespace Adw {

class DemoPageButtons : public Gtk::TemplateWidget<DemoPageButtons, Adw::Bin> {
  friend CppClassType;

protected:
  explicit DemoPageButtons(GtkWidget *obj) : TemplateWidgetBase(obj) { }

private:
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  static const char class_name[];
  void init_widget(Gtk::TemplateWidgetInit &i);
};

} // namespace Adw
