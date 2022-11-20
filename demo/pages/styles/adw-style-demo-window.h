#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/window_p.h>
#include "templatewidget.h"

namespace Adw {

class StyleDemoWindow
    : public Gtk::TemplateWidget<StyleDemoWindow, Adw::Window> {
  friend CppClassType;
public:
  static StyleDemoWindow* create();

protected:
  explicit StyleDemoWindow(GtkWidget *obj) : TemplateWidgetBase(obj) {}
  ~StyleDemoWindow() override;

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  void sidebar_back_cb();
  void sidebar_forward_cb();

  static void header_bar_cb(GtkWidget *sender, const char *name, GVariant *param);
  static void status_page_cb(GtkWidget *sender, const char *name, GVariant *param);
  static void sidebar_cb(GtkWidget *sender, const char *name, GVariant *param);

  std::unique_ptr<Glib::Property<bool>> p_devel;
  std::unique_ptr<Glib::Property<bool>> p_progress;

  Gtk::Window *header_bar_window;
  Gtk::Window *status_page_window;
  Gtk::Window *sidebar_window;
  Adw::Leaflet *sidebar_leaflet;

  static bool is_managed() { return false; }
};

} // namespace Adw
