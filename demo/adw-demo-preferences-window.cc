#include "adw-demo-preferences-window.h"

namespace Adw {

DemoPreferencesWindow::DemoPreferencesWindow()
    : Glib::ObjectBase("AdwDemoPreferencesWindow"),
      Gtk::TemplateBuilder<DemoPreferencesWindow>(
          this, "/org/gnome/Adwaitamm1/Demo/ui/adw-demo-preferences-window.ui",
          {{"subpage1", &subpage1}, {"subpage2", &subpage2}},
          {{"subpage1_activated_cb",
            Gtk::ptr_fun_to_mem_fun<
                &DemoPreferencesWindow::subpage1_activated>()},
           {"subpage2_activated_cb",
            Gtk::ptr_fun_to_mem_fun<
                &DemoPreferencesWindow::subpage2_activated>()},
           {"return_to_preferences_cb",
            Gtk::ptr_fun_to_mem_fun<
                &DemoPreferencesWindow::return_to_preferences>()}}),
      Adw::PreferencesWindow() {
  init_widget_template();

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
