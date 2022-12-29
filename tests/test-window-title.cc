/*
 * Copyright (C) 2021 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_window_title_title(void) {
  Adw::WindowTitle window_title("Some title", "");

  notified = 0;
  window_title.property_title().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring title = window_title.get_property<Glib::ustring>("title");
  g_assert(title == "Some title");

  window_title.set_title("Some title");
  g_assert(notified == 0);

  window_title.set_title("Another title");
  g_assert(window_title.get_title() == "Another title");
  g_assert(notified == 1);

  window_title.set_property<Glib::ustring>("title", "Yet another title");
  g_assert(window_title.get_title() == "Yet another title");
  g_assert(notified == 2);
}

static void test_adw_window_title_subtitle(void) {
  Adw::WindowTitle window_title("", "Some subtitle");

  notified = 0;
  window_title.property_subtitle().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring subtitle = window_title.get_property<Glib::ustring>("subtitle");
  g_assert(subtitle == "Some subtitle");

  window_title.set_subtitle("Some subtitle");
  g_assert(notified == 0);

  window_title.set_subtitle("Another subtitle");
  g_assert(window_title.get_subtitle() == "Another subtitle");
  g_assert(notified == 1);

  window_title.set_property<Glib::ustring>("subtitle", "Yet another subtitle");
  g_assert(window_title.get_subtitle() == "Yet another subtitle");
  g_assert(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/WindowTitle/title", test_adw_window_title_title);
  g_test_add_func("/Adwaita/WindowTitle/subtitle",
                  test_adw_window_title_subtitle);

  return g_test_run();
}
