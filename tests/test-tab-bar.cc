/*
 * Copyright (C) 2020 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_tab_bar_view(void) {
  Adw::TabBar bar;

  notified = 0;
  bar.property_view().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::TabView *view = bar.get_property<Adw::TabView *>("view");
  g_assert(view == nullptr);

  bar.set_view(nullptr);
  g_assert(notified == 0);

  view = Gtk::make_managed<Adw::TabView>();
  bar.set_view(view);
  g_assert_true(bar.get_view() == view);
  g_assert(notified == 1);

  bar.set_property<Adw::TabView *>("view", nullptr);
  g_assert(bar.get_view() == nullptr);
  g_assert(notified == 2);
}

static void test_adw_tab_bar_start_action_widget(void) {
  Adw::TabBar bar;

  notified = 0;
  bar.property_start_action_widget().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Gtk::Widget *widget = bar.get_property<Gtk::Widget *>("start-action-widget");
  g_assert(widget == nullptr);

  bar.set_start_action_widget(nullptr);
  g_assert(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  bar.set_start_action_widget(widget);
  g_assert_true(bar.get_start_action_widget() == widget);
  g_assert(notified == 1);

  bar.set_property<Gtk::Widget *>("start-action-widget", nullptr);
  g_assert_null(bar.get_start_action_widget());
  g_assert(notified == 2);
}

static void test_adw_tab_bar_end_action_widget(void) {
  Adw::TabBar bar;

  notified = 0;
  bar.property_end_action_widget().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Gtk::Widget *widget = bar.get_property<Gtk::Widget *>("end-action-widget");
  g_assert(widget == nullptr);

  bar.set_end_action_widget(nullptr);
  g_assert(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  bar.set_end_action_widget(widget);
  g_assert_true(bar.get_end_action_widget() == widget);
  g_assert(notified == 1);

  bar.set_property<Gtk::Widget *>("end-action-widget", nullptr);
  g_assert(bar.get_end_action_widget() == nullptr);
  g_assert(notified == 2);
}

static void test_adw_tab_bar_autohide(void) {
  Adw::TabBar bar;

  notified = 0;
  bar.property_autohide().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool autohide = bar.get_property<bool>("autohide");
  g_assert_true(autohide);

  bar.set_autohide(true);
  g_assert(notified == 0);

  bar.set_autohide(false);
  g_assert_false(bar.get_autohide());
  g_assert(notified == 1);

  bar.set_property<bool>("autohide", true);
  g_assert_true(bar.get_autohide());
  g_assert(notified == 2);
}

static void test_adw_tab_bar_tabs_revealed(void) {
  Adw::TabBar bar;

  notified = 0;
  bar.property_tabs_revealed().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool tabs_revealed = bar.get_property<bool>("tabs-revealed");
  g_assert_false(tabs_revealed);
  g_assert_false(bar.get_tabs_revealed());
  g_assert(notified == 0);

  bar.set_autohide(false);
  g_assert_false(bar.get_tabs_revealed());
  g_assert(notified == 0);

  Adw::TabView *view = Gtk::make_managed<Adw::TabView>();
  bar.set_view(view);
  g_assert_true(bar.get_tabs_revealed());
  g_assert(notified == 1);

  bar.set_autohide(true);
  g_assert_false(bar.get_tabs_revealed());
  g_assert(notified == 2);

  Glib::RefPtr<Adw::TabPage> page =
      view->append_pinned(Gtk::make_managed<Gtk::Button>());
  g_assert_true(bar.get_tabs_revealed());
  g_assert(notified == 3);

  view->set_page_pinned(page, false);
  g_assert_false(bar.get_tabs_revealed());
  g_assert(notified == 4);

  view->append(Gtk::make_managed<Gtk::Button>());
  g_assert_true(bar.get_tabs_revealed());
  g_assert(notified == 5);

  view->close_page(page);
  g_assert_false(bar.get_tabs_revealed());
  g_assert(notified == 6);

  bar.set_autohide(false);
  g_assert_true(bar.get_tabs_revealed());
  g_assert(notified == 7);
}

static void test_adw_tab_bar_expand_tabs(void) {
  Adw::TabBar bar;

  notified = 0;
  bar.property_expand_tabs().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool expand_tabs = bar.get_property<bool>("expand-tabs");
  g_assert_true(expand_tabs);

  bar.set_expand_tabs(true);
  g_assert(notified == 0);

  bar.set_expand_tabs(false);
  g_assert_false(bar.get_expand_tabs());
  g_assert(notified == 1);

  bar.set_property<bool>("expand-tabs", true);
  g_assert_true(bar.get_expand_tabs());
  g_assert(notified == 2);
}

static void test_adw_tab_bar_inverted(void) {
  Adw::TabBar bar;

  notified = 0;
  bar.property_inverted().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool inverted = bar.get_property<bool>("inverted");
  g_assert_false(inverted);

  bar.set_inverted(false);
  g_assert(notified == 0);

  bar.set_inverted(true);
  g_assert_true(bar.get_inverted());
  g_assert(notified == 1);

  bar.set_property<bool>("inverted", false);
  g_assert_false(bar.get_inverted());
  g_assert(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/TabBar/view", test_adw_tab_bar_view);
  g_test_add_func("/Adwaita/TabBar/start_action_widget",
                  test_adw_tab_bar_start_action_widget);
  g_test_add_func("/Adwaita/TabBar/end_action_widget",
                  test_adw_tab_bar_end_action_widget);
  g_test_add_func("/Adwaita/TabBar/autohide", test_adw_tab_bar_autohide);
  g_test_add_func("/Adwaita/TabBar/tabs_revealed",
                  test_adw_tab_bar_tabs_revealed);
  g_test_add_func("/Adwaita/TabBar/expand_tabs", test_adw_tab_bar_expand_tabs);
  g_test_add_func("/Adwaita/TabBar/inverted", test_adw_tab_bar_inverted);

  return g_test_run();
}
