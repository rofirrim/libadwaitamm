#include "adw-demo-page-view-switcher.h"

#include <glib/gi18n.h>

#include "adw-view-switcher-demo-window.h"

namespace Adw {

const char DemoPageViewSwitcher::class_name[] = "AdwDemoPageViewSwitcher";

void DemoPageViewSwitcher::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource("/org/gnome/Adwaitamm1/Demo/ui/pages/view-switcher/"
                 "adw-demo-page-view-switcher.ui");
  s.install_action(
      "demo.run",
      Gtk::ptr_fun_to_mem_fun<&DemoPageViewSwitcher::demo_run_cb>());
}

void DemoPageViewSwitcher::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

void DemoPageViewSwitcher::demo_run_cb() {
  Gtk::Window &root = *dynamic_cast<Gtk::Window *>(get_root());

  ViewSwitcherDemoWindow *window = ViewSwitcherDemoWindow::create();
  window->set_transient_for(root);
  window->present();
}
} // namespace Adw
