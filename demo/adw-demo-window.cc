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

// I am not sure how to wrap this in a more C++-esque way.
static char *get_color_scheme_icon_name(gpointer user_data, gboolean dark) {
  return g_strdup(dark ? "light-mode-symbolic" : "dark-mode-symbolic");
}

namespace Adw {

DemoWindow::DemoWindow(const Glib::RefPtr<Gtk::Application>& application)
    : Glib::ObjectBase("AdwDemoWindow"), Glib::ExtraClassInit(class_init,
                                                              nullptr,
                                                              instance_init),
      Adw::ApplicationWindow(application) {
  color_scheme_button = Glib::wrap(GTK_WIDGET(gtk_widget_get_template_child(
      GTK_WIDGET(gobj()), get_type(), "color_scheme_button")));
  main_leaflet = Glib::wrap(ADW_LEAFLET(gtk_widget_get_template_child(
      GTK_WIDGET(gobj()), get_type(), "main_leaflet")));
  subpage_leaflet = Glib::wrap(ADW_LEAFLET(gtk_widget_get_template_child(
      GTK_WIDGET(gobj()), get_type(), "subpage_leaflet")));

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

DemoWindow::~DemoWindow() {
  gtk_widget_dispose_template(GTK_WIDGET(gobj()), get_type());
}

template <void (DemoWindow::*P)()>
static void adapter(GTypeInstance *instance) {
  DemoWindow *w = dynamic_cast<DemoWindow *>(Glib::wrap(GTK_WIDGET(instance)));
  (w->*P)();
}

void DemoWindow::class_init(void *g_class, void *class_data) {
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(g_class);
  gtk_widget_class_add_binding_action(widget_class, GDK_KEY_q, GDK_CONTROL_MASK,
                                      "window.close", NULL);

  gtk_widget_class_set_template_from_resource(
      widget_class, "/org/gnome/Adwaitamm1/Demo/ui/adw-demo-window.ui");

  gtk_widget_class_bind_template_child_full(widget_class, "color_scheme_button",
                                            FALSE, 0);
  gtk_widget_class_bind_template_child_full(widget_class, "main_leaflet", FALSE,
                                            0);
  gtk_widget_class_bind_template_child_full(widget_class, "subpage_leaflet",
                                            FALSE, 0);
  gtk_widget_class_bind_template_child_full(widget_class, "toasts_page", FALSE,
                                            0);

  // FIXME: This is really weird to wrap sensibly. What is user_data? A
  // Gtk::Button?
  gtk_widget_class_bind_template_callback_full(
      widget_class, "get_color_scheme_icon_name",
      GCallback(get_color_scheme_icon_name));
  gtk_widget_class_bind_template_callback_full(
      widget_class, "color_scheme_button_clicked_cb",
      GCallback(adapter<&DemoWindow::color_scheme_button_clicked_cb>));
  gtk_widget_class_bind_template_callback_full(
      widget_class, "notify_visible_child_cb",
      GCallback(adapter<&DemoWindow::notify_visible_child_cb>));
  gtk_widget_class_bind_template_callback_full(
      widget_class, "back_clicked_cb",
      GCallback(adapter<&DemoWindow::back_clicked_cb>));
  gtk_widget_class_bind_template_callback_full(
      widget_class, "leaflet_back_clicked_cb",
      GCallback(adapter<&DemoWindow::leaflet_back_clicked_cb>));
  gtk_widget_class_bind_template_callback_full(
      widget_class, "leaflet_next_page_cb",
      GCallback(adapter<&DemoWindow::leaflet_next_page_cb>));
}

void DemoWindow::instance_init(GTypeInstance *instance,
                                          void *g_class) {
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

  gtk_widget_init_template(GTK_WIDGET(instance));
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

GType DemoWindow::gtype = 0;

}
