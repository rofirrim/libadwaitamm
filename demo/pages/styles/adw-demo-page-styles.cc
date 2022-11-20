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
  AdwStyleDemoWindow *window = adw_style_demo_window_new();
  GtkRoot *root = gtk_widget_get_root(GTK_WIDGET(this->gobj()));

  gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(root));
  gtk_window_present(GTK_WINDOW(window));
}

} // namespace Adw
