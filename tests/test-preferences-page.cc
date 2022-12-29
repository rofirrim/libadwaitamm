/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_preferences_page_add_remove(void) {
  Adw::PreferencesPage page;
  Adw::PreferencesGroup group;

  page.add(&group);
  page.remove(&group);
}

static void test_adw_preferences_page_title(void) {
  Adw::PreferencesPage page;

  g_assert_true(page.get_title() == "");

  page.set_title("Dummy title");
  g_assert_true(page.get_title() == "Dummy title");

  page.set_title("");
  g_assert_true(page.get_title() == "");
}

static void test_adw_preferences_page_icon_name(void) {
  Adw::PreferencesPage page;

  g_assert_true(page.get_icon_name() == "");

  page.set_icon_name("dummy-icon-name");
  g_assert_true(page.get_icon_name() == "dummy-icon-name");

  page.set_icon_name("");
  g_assert_true(page.get_icon_name() == "");
}

static void test_adw_preferences_page_use_underline(void) {
  Adw::PreferencesPage page;

  g_assert_false(page.get_use_underline());

  page.set_use_underline(true);
  g_assert_true(page.get_use_underline());

  page.set_use_underline(false);
  g_assert_false(page.get_use_underline());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/PreferencesPage/add_remove",
                  test_adw_preferences_page_add_remove);
  g_test_add_func("/Adwaita/PreferencesPage/title",
                  test_adw_preferences_page_title);
  g_test_add_func("/Adwaita/PreferencesPage/icon_name",
                  test_adw_preferences_page_icon_name);
  g_test_add_func("/Adwaita/PreferencesPage/use_underline",
                  test_adw_preferences_page_use_underline);

  return g_test_run();
}
