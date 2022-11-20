#include "adw-demo-page-flap.h"
#include "adw-flap-demo-window.h"

#include <glib/gi18n.h>

namespace Adw {

const char DemoPageFlap::class_name[] = "AdwDemoPageFlap";

void DemoPageFlap::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/flap/adw-demo-page-flap.ui");
  s.install_action("demo.run",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageFlap::demo_run_cb>());
}

void DemoPageFlap::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

void DemoPageFlap::demo_run_cb() {
  Adw::FlapDemoWindow *window = Adw::FlapDemoWindow::create();
  Gtk::Window *root = dynamic_cast<Gtk::Window*>(this->get_root());

  window->set_transient_for(*root);
  window->present();
}

} // namespace Adw
