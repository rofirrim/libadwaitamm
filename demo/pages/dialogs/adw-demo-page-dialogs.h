#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageDialogs : public Gtk::TemplateWidget<DemoPageDialogs, Adw::Bin> {
  friend CppClassType;

protected:
  DemoPageDialogs(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static unsigned int signal_add_toast;
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  void demo_message_dialog_cb();
};

} // namespace Adw
