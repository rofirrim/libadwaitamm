#pragma once

#include <libadwaitamm.h>
#include <libadwaitamm/private/window_p.h>
#include "templatewidget.h"

namespace Adw {

class TabViewDemoWindow
    : public Gtk::TemplateWidget<TabViewDemoWindow, Adw::Window> {
  friend CppClassType;

public:
  static TabViewDemoWindow *create();

  void prepopulate();

protected:
  explicit TabViewDemoWindow(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  static bool is_managed() { return false; }

  void page_detached_cb(AdwTabPage *page);
  void setup_menu_cb(AdwTabPage *page, AdwTabView *view);
  AdwTabView *create_window_cb();
  void indicator_activated_cb(AdwTabPage *page);
  gboolean extra_drag_drop_cb(AdwTabPage *page, GValue *value);

  void tab_new();

  Glib::RefPtr<Adw::TabPage> add_page(const Glib::RefPtr<Adw::TabPage> &parent,
                                      const Glib::ustring &title,
                                      Glib::RefPtr<Gio::Icon> icon);

  Glib::RefPtr<Gio::Icon> get_random_icon();
  static void init_icon_names(const Glib::RefPtr<Gtk::IconTheme> &theme);

  Glib::RefPtr<Adw::TabPage> get_current_page();

  void tab_change_indicator(const Glib::RefPtr<Gio::SimpleAction> &action,
                            bool indicator);
  void tab_change_icon(const Glib::RefPtr<Gio::SimpleAction> &action,
                       bool enable_icon);
  void tab_move_to_new_window();
  void tab_duplicate();

  void set_tab_action_enabled(const Glib::ustring &name, bool enabled);
  void set_tab_action_state(const Glib::ustring &name, bool enabled);

  void window_new();

  bool text_to_tooltip(const GValue *input_, GValue *output_);

  static Glib::RefPtr<Gio::Icon>
  get_indicator_icon(const Glib::RefPtr<Adw::TabPage> &page);
  static Glib::ustring
  get_indicator_tooltip(const Glib::RefPtr<Adw::TabPage> &page);

  Adw::TabView *view;
  Adw::TabBar *tab_bar;
  Glib::RefPtr<Gio::SimpleActionGroup> tab_action_group;

  Glib::RefPtr<Adw::TabPage> menu_page;
};

} // namespace Adw
