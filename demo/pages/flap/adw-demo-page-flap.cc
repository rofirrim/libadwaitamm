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
  AdwFlapDemoWindow *window = adw_flap_demo_window_new();
  GtkRoot *root = gtk_widget_get_root(GTK_WIDGET(this->gobj()));

  gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(root));
  gtk_window_present(GTK_WINDOW(window));
}

} // namespace Adw
