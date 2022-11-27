#include "adw-demo-page-welcome.h"

#include <glib/gi18n.h>

namespace Adw {

const char DemoPageWelcome::class_name[] = "AdwDemoPageWelcome";

void DemoPageWelcome::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/welcome/adw-demo-page-welcome.ui");
}

void DemoPageWelcome::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

} // namespace Adw
