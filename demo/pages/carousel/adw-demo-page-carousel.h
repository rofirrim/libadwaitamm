#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageCarousel : public Gtk::TemplateWidget<DemoPageCarousel, Adw::Bin> {
  friend CppClassType;

protected:
  DemoPageCarousel(GtkWidget *obj) : TemplateWidgetBase(obj) {}

  static void setup_template(Gtk::TemplateWidgetSetup &s);
  static const char class_name[];
  void init_widget(Gtk::TemplateWidgetInit &i);

  void notify_orientation_cb();
  void notify_indicators_cb();
  void carousel_return_cb();

  Gtk::Box *box;
  Adw::Carousel *carousel;
  Gtk::Stack *indicators_stack;
  Adw::ComboRow *orientation_row;
  Adw::ComboRow *indicators_row;
};

} // namespace Adw
