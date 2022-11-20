#include "adw-style-demo-window.h"

#include <glib/gi18n.h>

static void dummy_cb(GtkWidget *sender, const char *name, GVariant *param) {}

static GtkSelectionMode selection_mode_for_folded(gpointer data,
                                                  gboolean folded) {
  return folded ? GTK_SELECTION_NONE : GTK_SELECTION_BROWSE;
}

namespace Adw {

const char StyleDemoWindow::class_name[] = "AdwStyleDemoWindow";

StyleDemoWindow *StyleDemoWindow::create() {
  GObject *obj = g_object_new_with_properties(StyleDemoWindow::get_type(), 0,
                                              nullptr, nullptr);
  return StyleDemoWindow::wrap(obj);
}

StyleDemoWindow::~StyleDemoWindow() {
  delete header_bar_window;
  delete status_page_window;
  delete sidebar_window;
}

void StyleDemoWindow::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/styles/adw-style-demo-window.ui");
  s.bind_widget("header_bar_window");
  s.bind_widget("status_page_window");
  s.bind_widget("sidebar_window");
  s.bind_widget("sidebar_leaflet");

  s.bind_callback("selection_mode_for_folded",
                  GCallback(selection_mode_for_folded));
  s.bind_callback("sidebar_back_cb",
                  Gtk::ptr_fun_to_mem_fun<&StyleDemoWindow::sidebar_back_cb>());
  s.bind_callback(
      "sidebar_forward_cb",
      Gtk::ptr_fun_to_mem_fun<&StyleDemoWindow::sidebar_forward_cb>());

  s.install_action("style.header-bar", GCallback(header_bar_cb));
  s.install_action("style.status-page", GCallback(status_page_cb));
  s.install_action("style.sidebar", GCallback(sidebar_cb));
  s.install_action("style.dummy", GCallback(dummy_cb));
}

void StyleDemoWindow::init_widget(Gtk::TemplateWidgetInit &i) {
  p_devel = std::make_unique<Glib::Property<bool>>(*this, "devel", false);
  p_progress = std::make_unique<Glib::Property<bool>>(*this, "progress", false);

  // We cannot use gtk_widget_class_install_property_action because we register
  // properties at instantiation initialisation.
  auto simple_action_group = Gio::SimpleActionGroup::create();
  auto proxy_devel = Glib::PropertyProxy<bool>(this, "devel");
  simple_action_group->add_action(
      Gio::PropertyAction::create("devel", proxy_devel));
  proxy_devel.signal_changed().connect([this, proxy_devel]() {
    if (proxy_devel.get_value()) {
      add_css_class("devel");
      header_bar_window->add_css_class("devel");
      status_page_window->add_css_class("devel");
    } else {
      remove_css_class("devel");
      header_bar_window->remove_css_class("devel");
      status_page_window->remove_css_class("devel");
    }
  });
  simple_action_group->add_action(Gio::PropertyAction::create(
      "progress", Glib::PropertyProxy<bool>(this, "progress")));
  insert_action_group("style", simple_action_group);

  i.init_template();

  i.bind_widget(header_bar_window, "header_bar_window");
  i.bind_widget(status_page_window, "status_page_window");
  i.bind_widget(sidebar_window, "sidebar_window");
  i.bind_widget(sidebar_leaflet, "sidebar_leaflet");
}

void StyleDemoWindow::sidebar_back_cb() {
  sidebar_leaflet->navigate(Adw::NavigationDirection::BACK);
}

void StyleDemoWindow::sidebar_forward_cb() {
  sidebar_leaflet->navigate(Adw::NavigationDirection::FORWARD);
}

void StyleDemoWindow::header_bar_cb(GtkWidget *sender, const char *name,
                                    GVariant *param) {
  Adw::StyleDemoWindow *self = Adw::StyleDemoWindow::wrap(G_OBJECT(sender));
  self->header_bar_window->present();
}

void StyleDemoWindow::status_page_cb(GtkWidget *sender, const char *name,
                                     GVariant *param) {
  Adw::StyleDemoWindow *self = Adw::StyleDemoWindow::wrap(G_OBJECT(sender));
  self->status_page_window->present();
}

void StyleDemoWindow::sidebar_cb(GtkWidget *sender, const char *name,
                                 GVariant *param) {
  Adw::StyleDemoWindow *self = Adw::StyleDemoWindow::wrap(G_OBJECT(sender));
  self->sidebar_window->present();
}

} // namespace Adw
