#include "adw-demo-page-styles.h"

#include <glib/gi18n.h>

#include "adw-style-demo-window.h"

namespace Adw {
const char DemoPageStyles::class_name[] = "AdwDemoPageStyles";
void DemoPageStyles::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/styles/adw-demo-page-styles.ui");
  s.install_action("demo.run",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageStyles::demo_run_cb>());
}

void DemoPageStyles::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

void DemoPageStyles::demo_run_cb() {
  Adw::StyleDemoWindow *window = Adw::StyleDemoWindow::create();

  Gtk::Window *root = dynamic_cast<Gtk::Window *>(get_root());

  window->set_transient_for(*root);
  window->present();
}

} // namespace Adw
