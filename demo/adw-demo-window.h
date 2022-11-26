#pragma once

#include "templatewidget.h"
#include <libadwaitamm.h>
#include <libadwaitamm/private/applicationwindow_p.h>
#include "pages/toasts/adw-demo-page-toasts.h"

namespace Adw {

class DemoWindow
    : public Gtk::TemplateWidget<DemoWindow, Adw::ApplicationWindow> {
public:
  static DemoWindow *create(const Glib::RefPtr<Gtk::Application> &application);

protected:
  explicit DemoWindow(GtkWidget *obj) : TemplateWidgetBase(obj) { }

private:
  void color_scheme_button_clicked_cb();
  void notify_visible_child_cb();
  void back_clicked_cb();
  void leaflet_back_clicked_cb();
  void leaflet_next_page_cb();
  void toast_undo_cb();
  void notify_system_supports_color_schemes_cb();
  char *get_color_scheme_icon_name(gboolean dark);

  Adw::Leaflet *main_leaflet = nullptr;
  Adw::Leaflet *subpage_leaflet = nullptr;
  Gtk::Widget *color_scheme_button = nullptr;
  Adw::DemoPageToasts* toasts_page = nullptr;

  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  // Top level windows are not managed.
  static bool is_managed() { return false; }

  friend CppClassType;
};

} // namespace Adw
