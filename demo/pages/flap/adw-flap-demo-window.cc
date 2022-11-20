#include "adw-flap-demo-window.h"

#include <glib/gi18n.h>

static char *fold_policy_name(AdwEnumListItem *item, gpointer user_data) {
  switch (adw_enum_list_item_get_value(item)) {
  case ADW_FLAP_FOLD_POLICY_NEVER:
    return g_strdup(_("Never"));
  case ADW_FLAP_FOLD_POLICY_ALWAYS:
    return g_strdup(_("Always"));
  case ADW_FLAP_FOLD_POLICY_AUTO:
    return g_strdup(_("Auto"));
  default:
    return NULL;
  }
}

static char *transition_type_name(AdwEnumListItem *item, gpointer user_data) {
  switch (adw_enum_list_item_get_value(item)) {
  case ADW_FLAP_TRANSITION_TYPE_OVER:
    return g_strdup(_("Over"));
  case ADW_FLAP_TRANSITION_TYPE_UNDER:
    return g_strdup(_("Under"));
  case ADW_FLAP_TRANSITION_TYPE_SLIDE:
    return g_strdup(_("Slide"));
  default:
    return NULL;
  }
}

namespace Adw {

const char FlapDemoWindow::class_name[] = "AdwFlapDemoWindow";

FlapDemoWindow *FlapDemoWindow::create() {
  GObject *obj = g_object_new_with_properties(FlapDemoWindow::get_type(), 0,
                                              nullptr, nullptr);
  return FlapDemoWindow::wrap(obj);
}

void FlapDemoWindow::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/flap/adw-flap-demo-window.ui");

  s.bind_widget("flap");
  s.bind_widget("reveal_btn_start");
  s.bind_widget("reveal_btn_end");

  s.bind_callback("start_toggle_button_toggled_cb",
                  Gtk::ptr_fun_to_mem_fun<
                      &FlapDemoWindow::start_toggle_button_toggled_cb>());

  s.bind_callback("stack_notify_visible_child_cb",
                  Gtk::ptr_fun_to_mem_fun<
                      &FlapDemoWindow::stack_notify_visible_child_cb>());

  s.bind_callback("fold_policy_name", GCallback(fold_policy_name));
  s.bind_callback("transition_type_name", GCallback(transition_type_name));
}

void FlapDemoWindow::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();

  i.bind_widget(flap, "flap");
  i.bind_widget(reveal_btn_start, "reveal_btn_start");
  i.bind_widget(reveal_btn_end, "reveal_btn_end");
}

void FlapDemoWindow::stack_notify_visible_child_cb() {
  if (flap->get_folded() && !flap->get_locked())
    flap->set_reveal_flap(false);
}

void FlapDemoWindow::start_toggle_button_toggled_cb(GtkToggleButton *button_) {
  Gtk::ToggleButton *button = Glib::wrap(button_);
  if (button->get_active()) {
    flap->set_flap_position(Gtk::PackType::START);
    reveal_btn_end->hide();
    reveal_btn_start->show();
  } else {
    flap->set_flap_position(Gtk::PackType::END);
    reveal_btn_start->hide();
    reveal_btn_end->show();
  }
}

} // namespace Adw
