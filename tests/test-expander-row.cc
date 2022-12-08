/*
 * Copyright (C) 2018 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_expander_row_add_remove(void) {
  Adw::ExpanderRow row;
  Gtk::ListBoxRow child;

  row.add_row(&child);
  row.remove(&child);
}

static void test_adw_expander_row_subtitle(void) {
  Adw::ExpanderRow row;
  g_assert(row.get_subtitle() == "");

  row.set_subtitle("Dummy subtitle");
  g_assert(row.get_subtitle() == "Dummy subtitle");

  row.set_use_markup(false);
  row.set_subtitle("Invalid <b>markup");
  g_assert(row.get_subtitle() == "Invalid <b>markup");
}

static void test_adw_expander_row_icon_name(void) {
  Adw::ExpanderRow row;
  g_assert(row.get_icon_name() == "");

  row.set_icon_name("dummy-icon-name");
  g_assert(row.get_icon_name() == "dummy-icon-name");
}

static void test_adw_expander_row_expanded(void) {
  Adw::ExpanderRow row;
  g_assert_false(row.get_expanded());

  row.set_expanded(true);
  g_assert_true(row.get_expanded());

  row.set_expanded(false);
  g_assert_false(row.get_expanded());
}

static void test_adw_expander_row_enable_expansion(void) {
  Adw::ExpanderRow row;

  g_assert_true(row.get_enable_expansion());
  g_assert_false(row.get_expanded());

  row.set_expanded(true);
  g_assert_true(row.get_expanded());

  row.set_enable_expansion(false);
  g_assert_false(row.get_enable_expansion());
  g_assert_false(row.get_expanded());

  row.set_expanded(true);
  g_assert_false(row.get_expanded());

  row.set_enable_expansion(true);
  g_assert_true(row.get_enable_expansion());
  g_assert_true(row.get_expanded());
}

static void test_adw_expander_row_show_enable_switch(void) {
  Adw::ExpanderRow row;

  g_assert_false(row.get_show_enable_switch());

  row.set_show_enable_switch(true);
  g_assert_true(row.get_show_enable_switch());

  row.set_show_enable_switch(false);
  g_assert_false(row.get_show_enable_switch());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ExpanderRow/add_remove",
                  test_adw_expander_row_add_remove);
  g_test_add_func("/Adwaita/ExpanderRow/subtitle",
                  test_adw_expander_row_subtitle);
  g_test_add_func("/Adwaita/ExpanderRow/icon_name",
                  test_adw_expander_row_icon_name);
  g_test_add_func("/Adwaita/ExpanderRow/expanded",
                  test_adw_expander_row_expanded);
  g_test_add_func("/Adwaita/ExpanderRow/enable_expansion",
                  test_adw_expander_row_enable_expansion);
  g_test_add_func("/Adwaita/ExpanderRow/show_enable_switch",
                  test_adw_expander_row_show_enable_switch);

  return g_test_run();
}
