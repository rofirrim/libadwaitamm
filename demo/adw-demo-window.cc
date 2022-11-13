#include "adw-demo-window.h"
#include <libadwaitamm/private/applicationwindow_p.h>

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

// TODO: Remove
#include "templatebuilder.h"

namespace Adw {

//////////////////////
// DemoWindow_Class //
//////////////////////

void DemoWindow_Class::class_init_function(void *g_class, void *class_data) {
  Adw::ApplicationWindow_Class::class_init_function(g_class, class_data);

  Gtk::TemplateWidgetSetup s(
      GTK_WIDGET_CLASS(g_class),
      "/org/gnome/Adwaitamm1/Demo/ui/adw-demo-window.ui");

  s.bind_widget("color_scheme_button");
  s.bind_widget("main_leaflet");
  s.bind_widget("subpage_leaflet");

  s.bind_callback(
      "get_color_scheme_icon_name",
      Gtk::ptr_fun_to_mem_fun<&DemoWindow::get_color_scheme_icon_name>());
  s.bind_callback(
      "color_scheme_button_clicked_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoWindow::color_scheme_button_clicked_cb>());
  s.bind_callback(
      "notify_visible_child_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoWindow::notify_visible_child_cb>());
  s.bind_callback("back_clicked_cb",
                  Gtk::ptr_fun_to_mem_fun<&DemoWindow::back_clicked_cb>());
  s.bind_callback(
      "leaflet_back_clicked_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoWindow::leaflet_back_clicked_cb>());
  s.bind_callback("leaflet_next_page_cb",
                  Gtk::ptr_fun_to_mem_fun<&DemoWindow::leaflet_next_page_cb>());
}

const char DemoWindow_Class::class_name[] = "AdwDemoWindow";

////////////////
// DemoWindow //
////////////////

DemoWindow *
DemoWindow::create(const Glib::RefPtr<Gtk::Application> &application) {
  Glib::ConstructParams params(type_class_.init(), "application",
                               Glib::unwrap(application), nullptr);
  GObject *obj = g_object_new_with_properties(
      DemoWindow::get_type(), params.n_parameters, params.parameter_names,
      params.parameter_values);
  return DemoWindow::wrap(obj);
}

DemoWindow::DemoWindow(GtkWidget *obj) : TemplateWidgetBase(obj) {}

void DemoWindow::instance_init_function(GTypeInstance *instance,
                                        void *g_class) {
  g_type_ensure(Adw::DemoPageAbout::get_type());
  // g_type_ensure(ADW_TYPE_DEMO_PAGE_ABOUT);
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

  gtk_widget_init_template(GTK_WIDGET(instance));

  DemoWindow *this_ = DemoWindow::wrap(G_OBJECT(instance));

  this_->color_scheme_button =
      Glib::wrap(GTK_WIDGET(gtk_widget_get_template_child(
          GTK_WIDGET(instance), G_OBJECT_TYPE(instance),
          "color_scheme_button")));
  this_->main_leaflet = Glib::wrap(ADW_LEAFLET(gtk_widget_get_template_child(
      GTK_WIDGET(instance), G_OBJECT_TYPE(instance), "main_leaflet")));
  this_->subpage_leaflet = Glib::wrap(ADW_LEAFLET(gtk_widget_get_template_child(
      GTK_WIDGET(instance), G_OBJECT_TYPE(instance), "subpage_leaflet")));


  auto simple_action_group = Gio::SimpleActionGroup::create();
  simple_action_group->add_action(
      "undo", sigc::mem_fun(*this_, &DemoWindow::toast_undo_cb));
  this_->insert_action_group("toast", simple_action_group);

  auto manager = StyleManager::get_default();
  manager->property_system_supports_color_schemes().signal_changed().connect(
      sigc::mem_fun(*this_,
                    &DemoWindow::notify_system_supports_color_schemes_cb));

  this_->main_leaflet->navigate(NavigationDirection::FORWARD);
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

} // namespace Adw
