#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageLists : public Gtk::TemplateWidget<DemoPageLists, Adw::Bin> {
  friend CppClassType;

protected:
  explicit DemoPageLists(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static unsigned int signal_add_toast;
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  void entry_apply_cb();
};

} // namespace Adw
