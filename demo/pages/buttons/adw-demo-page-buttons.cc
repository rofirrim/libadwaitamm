#include "adw-demo-page-buttons.h"

#include <glib/gi18n.h>

namespace Adw {

const char DemoPageButtons::class_name[] = "AdwDemoPageButtons";

void DemoPageButtons::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/buttons/adw-demo-page-buttons.ui");
}

void DemoPageButtons::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}
} // namespace Adw
