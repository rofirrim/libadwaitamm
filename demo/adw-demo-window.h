#pragma once

#include <libadwaitamm.h>
#include "templatebuilder.h"

namespace Adw {

class DemoWindow : public Gtk::TemplateBuilder<DemoWindow>,
                   public Adw::ApplicationWindow {

public:
  DemoWindow(const Glib::RefPtr<Gtk::Application> &application);
  // DemoWindow(GTypeInstance*) { g_assert_not_reached(); }
  ~DemoWindow();

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
};
} // namespace Adw
