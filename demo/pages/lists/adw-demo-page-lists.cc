#include "adw-demo-page-lists.h"

#include <glib/gi18n.h>

namespace Adw {

const char DemoPageLists::class_name[] = "AdwDemoPageLists";
unsigned int DemoPageLists::signal_add_toast;

void DemoPageLists::setup_template(Gtk::TemplateWidgetSetup &s) {
  signal_add_toast =
      g_signal_new("add-toast", s.get_class_type(), G_SIGNAL_RUN_FIRST, 0, NULL,
                   NULL, NULL, G_TYPE_NONE, 1, ADW_TYPE_TOAST);

  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/lists/adw-demo-page-lists.ui");

  s.bind_callback("entry_apply_cb",
                  Gtk::ptr_fun_to_mem_fun<&DemoPageLists::entry_apply_cb>());
}

void DemoPageLists::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

void DemoPageLists::entry_apply_cb() {
  auto toast = new Adw::Toast("Changes applied");

  g_signal_emit(gobj(), signal_add_toast, 0, toast->gobj());
}

} // namespace Adw
