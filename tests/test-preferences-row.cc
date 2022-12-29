/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_preferences_row_title(void) {
  Adw::PreferencesRow row;

  g_assert_true(row.get_title() == "");

  row.set_title("Dummy title");
  g_assert_true(row.get_title() == "Dummy title");

  row.set_title("");
  g_assert_true(row.get_title() == "");

  row.set_use_markup(false);
  row.set_title("Invalid <b>markup");
  g_assert_true(row.get_title() == "Invalid <b>markup");
}

static void test_adw_preferences_row_use_underline(void) {
  Adw::PreferencesRow row;

  g_assert_false(row.get_use_underline());

  row.set_use_underline(true);
  g_assert_true(row.get_use_underline());

  row.set_use_underline(false);
  g_assert_false(row.get_use_underline());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/PreferencesRow/title",
                  test_adw_preferences_row_title);
  g_test_add_func("/Adwaita/PreferencesRow/use_underline",
                  test_adw_preferences_row_use_underline);

  return g_test_run();
}
