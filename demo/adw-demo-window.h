#pragma once

#include "namedclass.h"
#include "templatewidget.h"
#include <libadwaitamm.h>

namespace Adw {

class DemoWindow_Class;
class DemoWindow;

class DemoWindow_Class
    : public Gtk::TemplateWidgetClass<DemoWindow_Class, DemoWindow,
                                      adw_application_window_get_type> {
private:
  static void class_init_function(void *g_class, void *class_data);
  static const char class_name[];

  friend TemplateWidgetClassBase;
};

class DemoWindow
    : public Gtk::TemplateWidget<DemoWindow_Class, DemoWindow,
                                 Adw::ApplicationWindow, AdwApplicationWindow> {
public:
  static DemoWindow *create(const Glib::RefPtr<Gtk::Application> &application);

private:
  explicit DemoWindow(GtkWidget *obj);

  void color_scheme_button_clicked_cb();
  void notify_visible_child_cb();
  void back_clicked_cb();
  void leaflet_back_clicked_cb();
  void leaflet_next_page_cb();
  void toast_undo_cb();
  void notify_system_supports_color_schemes_cb();
  char *get_color_scheme_icon_name(gboolean dark);

  Adw::Leaflet *main_leaflet = nullptr;
  Adw::Leaflet *subpage_leaflet = nullptr;
  Gtk::Widget *color_scheme_button = nullptr;

  static void instance_init_function(GTypeInstance *instance, void *g_class);

  friend DemoWindow_Class;
  friend DemoWindow_Class::TemplateWidgetClassBase;

  // friend TemplateWidgetBase;
};

#if 0
class DemoWindow_Class : public Glib::NamedClass {
public:
  const Glib::Class &init();
  static void class_init_function(void *g_class, void *class_data);

  static Glib::ObjectBase *wrap_new(GObject *);
};

class DemoWindow : public Adw::ApplicationWindow {

public:
  static DemoWindow *create(const Glib::RefPtr<Gtk::Application> &application);
  ~DemoWindow();

  static GType get_type() G_GNUC_CONST;
  static DemoWindow *wrap(GObject *);

protected:
  explicit DemoWindow(GtkWidget *obj);

private:
  void color_scheme_button_clicked_cb();
  void notify_visible_child_cb();
  void back_clicked_cb();
  void leaflet_back_clicked_cb();
  void leaflet_next_page_cb();
  void toast_undo_cb();
  void notify_system_supports_color_schemes_cb();
  char *get_color_scheme_icon_name(gboolean dark);

  Adw::Leaflet *main_leaflet = nullptr;
  Adw::Leaflet *subpage_leaflet = nullptr;
  Gtk::Widget *color_scheme_button = nullptr;

  static void instance_init_function(GTypeInstance *instance, void *g_class);
  static DemoWindow_Class demo_window_class_;
  friend class DemoWindow_Class;
};
#endif

} // namespace Adw
