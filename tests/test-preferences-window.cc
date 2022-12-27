/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_preferences_window_add_remove(void) {
  Adw::PreferencesWindow window;
  Adw::PreferencesPage page;

  window.add(&page);
  window.remove(&page);
}

static void test_adw_preferences_window_add_toast(void) {
  Adw::PreferencesWindow window;
  Adw::Toast *toast = new Adw::Toast("Test notification");
  window.add_toast(toast);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/PreferencesWindow/add_remove",
                  test_adw_preferences_window_add_remove);
  g_test_add_func("/Adwaita/PreferencesWindow/add_toast",
                  test_adw_preferences_window_add_toast);

  return g_test_run();
}
