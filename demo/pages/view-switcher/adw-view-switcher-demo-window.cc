#include "adw-view-switcher-demo-window.h"

#include <glib/gi18n.h>

namespace Adw {

const char ViewSwitcherDemoWindow::class_name[] = "AdwViewSwitcherDemoWindow";

ViewSwitcherDemoWindow *ViewSwitcherDemoWindow::create() {
  GObject *obj = g_object_new_with_properties(
      ViewSwitcherDemoWindow::get_type(), 0, nullptr, nullptr);
  return ViewSwitcherDemoWindow::wrap(obj);
}

void ViewSwitcherDemoWindow::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource("/org/gnome/Adwaitamm1/Demo/ui/pages/view-switcher/"
                 "adw-view-switcher-demo-window.ui");
}

void ViewSwitcherDemoWindow::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

} // namespace Adw
