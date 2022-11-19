#include "adw-demo-page-clamp.h"

#include <glib/gi18n.h>

namespace Adw {
const char DemoPageClamp::class_name[] = "AdwDemoPageClamp";

void DemoPageClamp::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/clamp/adw-demo-page-clamp.ui");
}

void DemoPageClamp::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

} // namespace Adw
