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

static void test_adw_style_manager_color_scheme(void) {
  Glib::RefPtr<Adw::StyleManager> manager = Adw::StyleManager::get_default();
  Adw::ColorScheme color_scheme;

  notified = 0;
  sigc::connection conn =
      manager->property_color_scheme().signal_changed().connect(
          sigc::ptr_fun(notify_cb));

  color_scheme = manager->get_property<Adw::ColorScheme>("color-scheme");
  g_assert_true(color_scheme == Adw::ColorScheme::DEFAULT);
  g_assert_true(notified == 0);

  manager->set_color_scheme(Adw::ColorScheme::DEFAULT);
  g_assert_true(notified == 0);

  manager->set_color_scheme(Adw::ColorScheme::PREFER_DARK);
  color_scheme = manager->get_property<Adw::ColorScheme>("color-scheme");
  g_assert_true(color_scheme == Adw::ColorScheme::PREFER_DARK);
  g_assert_true(notified == 1);

  manager->set_property<Adw::ColorScheme>("color-scheme",
                                          Adw::ColorScheme::PREFER_LIGHT);
  g_assert_true(manager->get_color_scheme() == Adw::ColorScheme::PREFER_LIGHT);
  g_assert_true(notified == 2);

  conn.disconnect();
  manager->set_color_scheme(Adw::ColorScheme::DEFAULT);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/StyleManager/color_scheme",
                  test_adw_style_manager_color_scheme);

  return g_test_run();
}
