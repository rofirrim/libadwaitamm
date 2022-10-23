#include "adw-demo-preferences-window.h"

namespace Adw {

DemoPreferencesWindow::DemoPreferencesWindow()
    : Glib::ObjectBase("AdwDemoPreferencesWindow"), Glib::ExtraClassInit(
                                                        class_init, nullptr,
                                                        instance_init),
      Adw::PreferencesWindow() {
  subpage1 = Glib::wrap(GTK_WIDGET(gtk_widget_get_template_child(
      GTK_WIDGET(gobj()), get_type(), "subpage1")));
  subpage2 = Glib::wrap(GTK_WIDGET(gtk_widget_get_template_child(
      GTK_WIDGET(gobj()), get_type(), "subpage2")));

  auto simple_action_group = Gio::SimpleActionGroup::create();
  simple_action_group->add_action(
      "show", sigc::mem_fun(*this, &DemoPreferencesWindow::toast_show));
  insert_action_group("toast", simple_action_group);
}

DemoPreferencesWindow::~DemoPreferencesWindow() {
  gtk_widget_dispose_template(GTK_WIDGET(gobj()), get_type());
}

template <void (DemoPreferencesWindow::*P)()>
static void adapter(GTypeInstance *instance) {
  DemoPreferencesWindow *w =
      dynamic_cast<DemoPreferencesWindow *>(Glib::wrap(GTK_WIDGET(instance)));
  (w->*P)();
}

void DemoPreferencesWindow::class_init(void *g_class, void *class_data) {
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(g_class);
  gtk_widget_class_set_template_from_resource(
      widget_class,
      "/org/gnome/Adwaitamm1/Demo/ui/adw-demo-preferences-window.ui");

  gtk_widget_class_bind_template_child_full(widget_class, "subpage1", FALSE, 0);
  gtk_widget_class_bind_template_child_full(widget_class, "subpage2", FALSE, 0);

  gtk_widget_class_bind_template_callback_full(
      widget_class, "subpage1_activated_cb",
      reinterpret_cast<GCallback>(
          adapter<&DemoPreferencesWindow::subpage1_activated>));
  gtk_widget_class_bind_template_callback_full(
      widget_class, "subpage2_activated_cb",
      reinterpret_cast<GCallback>(
          adapter<&DemoPreferencesWindow::subpage2_activated>));
  gtk_widget_class_bind_template_callback_full(
      widget_class, "return_to_preferences_cb",
      reinterpret_cast<GCallback>(
          adapter<&DemoPreferencesWindow::return_to_preferences>));
}

void DemoPreferencesWindow::instance_init(GTypeInstance *instance,
                                          void *g_class) {
  gtk_widget_init_template(GTK_WIDGET(instance));
}

void DemoPreferencesWindow::subpage1_activated() { present_subpage(subpage1); }

void DemoPreferencesWindow::subpage2_activated() { present_subpage(subpage2); }

void DemoPreferencesWindow::return_to_preferences() { close_subpage(); }

void DemoPreferencesWindow::toast_show() {
  auto toast = new Adw::Toast("Example Toast");
  add_toast(toast);
}

GType DemoPreferencesWindow::gtype;

} // namespace Adw
