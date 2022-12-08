/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_header_bar_pack(void) {
  Adw::HeaderBar bar;
  Gtk::Switch widget1;

  bar.pack_start(&widget1);

  Gtk::Switch widget2;

  bar.pack_end(&widget2);
}

static void test_adw_header_bar_title_widget(void) {
  Adw::HeaderBar bar;
  g_assert_null(bar.get_title_widget());

  Gtk::Switch widget;
  bar.set_title_widget(&widget);

  g_assert(bar.get_title_widget()->gobj() == (GtkWidget *)widget.gobj());

  bar.set_title_widget(nullptr);
  g_assert_null(bar.get_title_widget());
}

static void test_adw_header_bar_show_start_title_buttons(void) {
  Adw::HeaderBar bar;

  g_assert_true(bar.get_show_start_title_buttons());

  bar.set_show_start_title_buttons(false);
  g_assert_false(bar.get_show_start_title_buttons());

  bar.set_show_start_title_buttons(true);
  g_assert_true(bar.get_show_start_title_buttons());
}

static void test_adw_header_bar_show_end_title_buttons(void) {
  Adw::HeaderBar bar;

  g_assert_true(bar.get_show_end_title_buttons());

  bar.set_show_end_title_buttons(false);
  g_assert_false(bar.get_show_end_title_buttons());

  bar.set_show_end_title_buttons(true);
  g_assert_true(bar.get_show_end_title_buttons());
}

static void test_adw_header_bar_decoration_layout(void) {
  Adw::HeaderBar bar;

  g_assert(bar.get_decoration_layout() == "");

  bar.set_decoration_layout(":");

  g_assert(bar.get_decoration_layout() == ":");

  bar.set_decoration_layout("");
  g_assert(bar.get_decoration_layout() == "");
}

static void test_adw_header_bar_centering_policy(void) {
  Adw::HeaderBar bar;

  g_assert(bar.get_centering_policy() == Adw::CenteringPolicy::LOOSE);

  bar.set_centering_policy(Adw::CenteringPolicy::STRICT);
  g_assert(bar.get_centering_policy() == Adw::CenteringPolicy::STRICT);

  bar.set_centering_policy(Adw::CenteringPolicy::LOOSE);
  g_assert(bar.get_centering_policy() == Adw::CenteringPolicy::LOOSE);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/HeaderBar/pack", test_adw_header_bar_pack);
  g_test_add_func("/Adwaita/HeaderBar/title_widget",
                  test_adw_header_bar_title_widget);
  g_test_add_func("/Adwaita/HeaderBar/show_start_title_buttons",
                  test_adw_header_bar_show_start_title_buttons);
  g_test_add_func("/Adwaita/HeaderBar/show_end_title_buttons",
                  test_adw_header_bar_show_end_title_buttons);
  g_test_add_func("/Adwaita/HeaderBar/decoration_layout",
                  test_adw_header_bar_decoration_layout);
  g_test_add_func("/Adwaita/HeaderBar/centering_policy",
                  test_adw_header_bar_centering_policy);

  return g_test_run();
}
