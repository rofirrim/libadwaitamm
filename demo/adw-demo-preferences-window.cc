#include "adw-demo-preferences-window.h"

namespace Adw {

///////////////////////////
// DemoPreferencesWindow //
///////////////////////////

const char DemoPreferencesWindow::class_name[] =
    "AdwDemoPreferencesWindow";

void DemoPreferencesWindow::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/adw-demo-preferences-window.ui");

  s.bind_widget("subpage1");
  s.bind_widget("subpage2");

  s.bind_callback(
      "subpage1_activated_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoPreferencesWindow::subpage1_activated>());
  s.bind_callback(
      "subpage2_activated_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoPreferencesWindow::subpage2_activated>());
  s.bind_callback(
      "return_to_preferences_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoPreferencesWindow::return_to_preferences>());
}

DemoPreferencesWindow *DemoPreferencesWindow::create() {
  GObject *obj = g_object_new_with_properties(DemoPreferencesWindow::get_type(),
                                              0, nullptr, nullptr);
  return DemoPreferencesWindow::wrap(obj);
}

void DemoPreferencesWindow::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();

  i.bind_widget(subpage1, "subpage1");
  i.bind_widget(subpage2, "subpage2");

  auto simple_action_group = Gio::SimpleActionGroup::create();
  simple_action_group->add_action(
      "show", sigc::mem_fun(*this, &DemoPreferencesWindow::toast_show));
  insert_action_group("toast", simple_action_group);
}

void DemoPreferencesWindow::subpage1_activated() { present_subpage(subpage1); }

void DemoPreferencesWindow::subpage2_activated() { present_subpage(subpage2); }

void DemoPreferencesWindow::return_to_preferences() { close_subpage(); }

void DemoPreferencesWindow::toast_show() {
  auto toast = new Adw::Toast("Example Toast");
  add_toast(toast);
}

} // namespace Adw
