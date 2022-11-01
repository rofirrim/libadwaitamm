#include "adw-demo-window.h"

#include <glib/gi18n.h>

#include "pages/about/adw-demo-page-about.h"
#include "pages/animations/adw-demo-page-animations.h"
#include "pages/avatar/adw-demo-page-avatar.h"
#include "pages/buttons/adw-demo-page-buttons.h"
#include "pages/carousel/adw-demo-page-carousel.h"
#include "pages/clamp/adw-demo-page-clamp.h"
#include "pages/dialogs/adw-demo-page-dialogs.h"
#include "pages/flap/adw-demo-page-flap.h"
#include "pages/leaflet/adw-demo-page-leaflet.h"
#include "pages/lists/adw-demo-page-lists.h"
#include "pages/styles/adw-demo-page-styles.h"
#include "pages/tab-view/adw-demo-page-tab-view.h"
#include "pages/toasts/adw-demo-page-toasts.h"
#include "pages/view-switcher/adw-demo-page-view-switcher.h"
#include "pages/welcome/adw-demo-page-welcome.h"

namespace Adw {

DemoWindow::DemoWindow(const Glib::RefPtr<Gtk::Application> &application)
    : Glib::ObjectBase("AdwDemoWindow"),
      Gtk::TemplateBuilder<DemoWindow>(
          this, "/org/gnome/Adwaitamm1/Demo/ui/adw-demo-window.ui",
          {{"color_scheme_button", &color_scheme_button},
           {"main_leaflet", &main_leaflet},
           {"subpage_leaflet", &subpage_leaflet}},
          {{"get_color_scheme_icon_name",
            Gtk::ptr_fun_to_mem_fun<&DemoWindow::get_color_scheme_icon_name>()},
           {"color_scheme_button_clicked_cb",
            Gtk::ptr_fun_to_mem_fun<
                &DemoWindow::color_scheme_button_clicked_cb>()},
           {"notify_visible_child_cb",
            Gtk::ptr_fun_to_mem_fun<&DemoWindow::notify_visible_child_cb>()},
           {"back_clicked_cb",
            Gtk::ptr_fun_to_mem_fun<&DemoWindow::back_clicked_cb>()},
           {"leaflet_back_clicked_cb",
            Gtk::ptr_fun_to_mem_fun<&DemoWindow::leaflet_back_clicked_cb>()},
           {"leaflet_next_page_cb",
            Gtk::ptr_fun_to_mem_fun<&DemoWindow::leaflet_next_page_cb>()}}),
      Adw::ApplicationWindow(application) {

  // FIXME: We will be able to remove this as we port these widgets.
  g_type_ensure(ADW_TYPE_DEMO_PAGE_ABOUT);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_ANIMATIONS);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_AVATAR);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_BUTTONS);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_CAROUSEL);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_CLAMP);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_DIALOGS);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_FLAP);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_LEAFLET);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_LISTS);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_STYLES);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_TAB_VIEW);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_TOASTS);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_VIEW_SWITCHER);
  g_type_ensure(ADW_TYPE_DEMO_PAGE_WELCOME);

  init_widget_template();

  auto simple_action_group = Gio::SimpleActionGroup::create();
  simple_action_group->add_action(
      "undo", sigc::mem_fun(*this, &DemoWindow::toast_undo_cb));
  insert_action_group("toast", simple_action_group);

  auto manager = StyleManager::get_default();
  manager->property_system_supports_color_schemes().signal_changed().connect(
      sigc::mem_fun(*this,
                    &DemoWindow::notify_system_supports_color_schemes_cb));

  main_leaflet->navigate(NavigationDirection::FORWARD);
}

void DemoWindow::color_scheme_button_clicked_cb() {
  auto manager = StyleManager::get_default();

  if (manager->get_dark()) {
    manager->set_color_scheme(ColorScheme::FORCE_LIGHT);
  } else {
    manager->set_color_scheme(ColorScheme::FORCE_DARK);
  }
}

void DemoWindow::notify_visible_child_cb() {
  main_leaflet->navigate(NavigationDirection::FORWARD);
}

void DemoWindow::back_clicked_cb() {
  main_leaflet->navigate(NavigationDirection::BACK);
}

void DemoWindow::leaflet_back_clicked_cb() {
  subpage_leaflet->navigate(NavigationDirection::BACK);
}

void DemoWindow::leaflet_next_page_cb() {
  subpage_leaflet->navigate(NavigationDirection::FORWARD);
}

void DemoWindow::toast_undo_cb() {
  g_warning("Not implemented yet");
  // adw_demo_page_toasts_undo(self->toasts_page->gobj());
}

void DemoWindow::notify_system_supports_color_schemes_cb() {
  auto manager = StyleManager::get_default();
  bool supports = manager->get_system_supports_color_schemes();

  color_scheme_button->set_visible(!supports);

  if (supports)
    manager->set_color_scheme(ColorScheme::DEFAULT);
}

char *DemoWindow::get_color_scheme_icon_name(gboolean dark) {
  return g_strdup(dark ? "light-mode-symbolic" : "dark-mode-symbolic");
}
}
