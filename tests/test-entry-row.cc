/*
 * Copyright (C) 2022 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_entry_row_add_remove(void) {
  Adw::EntryRow row;

  Gtk::Button prefix, suffix;

  row.add_prefix(&prefix);
  row.add_suffix(&suffix);

  row.remove(&prefix);
  row.remove(&suffix);
}

static void test_adw_entry_row_show_apply_button(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_show_apply_button().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool show_apply_button = row.get_property<bool>("show-apply-button");
  g_assert_false(show_apply_button);

  row.set_show_apply_button(false);
  g_assert_true(notified == 0);

  row.set_show_apply_button(true);
  g_assert_true(row.get_show_apply_button());
  g_assert_true(notified == 1);

  row.set_property<bool>("show-apply-button", false);
  g_assert_false(row.get_show_apply_button());
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/EntryRow/add_remove",
                  test_adw_entry_row_add_remove);
  g_test_add_func("/Adwaita/EntryRow/show_apply_button",
                  test_adw_entry_row_show_apply_button);

  return g_test_run();
}
