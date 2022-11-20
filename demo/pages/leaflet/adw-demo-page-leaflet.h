#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageLeaflet : public Gtk::TemplateWidget<DemoPageLeaflet, Adw::Bin> {
  friend CppClassType;

protected:
  DemoPageLeaflet(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  static unsigned int signal_next_page;
  void init_widget(Gtk::TemplateWidgetInit &i);

  void next_row_activated_cb();

  std::unique_ptr<Glib::Property<Adw::LeafletTransitionType>> p_transition_type;

};

} // namespace Adw
