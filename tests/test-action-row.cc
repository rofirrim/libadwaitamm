/*
 * Copyright (C) 2018 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_action_row_add_remove(void) {
  Adw::ActionRow row;

  auto *prefix = Gtk::make_managed<Gtk::CheckButton>();
  auto *suffix = Gtk::make_managed<Gtk::CheckButton>();

  row.add_prefix(*prefix);
  row.add_suffix(*suffix);

  row.remove(*prefix);
  row.remove(*suffix);
}

static void test_adw_action_row_subtitle(void) {
  Adw::ActionRow row;

  g_assert_true(row.get_subtitle() == "");

  row.set_subtitle("Dummy subtitle");
  g_assert_true(row.get_subtitle() == "Dummy subtitle");

  row.set_use_markup(false);
  row.set_subtitle("Invalid <b>markup");
  g_assert_true(row.get_subtitle() == "Invalid <b>markup");
}

static void test_adw_action_row_icon_name(void) {
  Adw::ActionRow row;

  g_assert_true(row.get_icon_name() == "");

  row.set_icon_name("dummy-icon-name");
  g_assert_true(row.get_icon_name() == "dummy-icon-name");
}

static void test_adw_action_row_title_lines(void) {
  Adw::ActionRow row;
  g_assert_true(row.get_title_lines() == 0);

  g_test_expect_message(
      ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
      "adw_action_row_set_title_lines: assertion 'title_lines >= 0' failed");
  row.set_title_lines(-1);
  g_test_assert_expected_messages();

  g_assert_true(row.get_title_lines() == 0);

  row.set_title_lines(1);
  g_assert_true(row.get_title_lines() == 1);
}

static void test_adw_action_row_subtitle_lines(void) {
  Adw::ActionRow row;

  g_assert_true(row.get_subtitle_lines() == 0);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "adw_action_row_set_subtitle_lines: assertion "
                        "'subtitle_lines >= 0' failed");
  row.set_subtitle_lines(-1);
  g_test_assert_expected_messages();

  g_assert_true(row.get_subtitle_lines() == 0);

  row.set_subtitle_lines(1);
  g_assert_true(row.get_subtitle_lines() == 1);
}

static void test_adw_action_row_activate(void) {
  Adw::ActionRow row;

  int activated = 0;
  row.signal_activated().connect([&activated]() { activated++; });

  row.activate();
  g_assert_cmpint(activated, ==, 1);
}

int
main (int   argc,
      char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ActionRow/add_remove", test_adw_action_row_add_remove);
  g_test_add_func("/Adwaita/ActionRow/subtitle", test_adw_action_row_subtitle);
  g_test_add_func("/Adwaita/ActionRow/icon_name", test_adw_action_row_icon_name);
  g_test_add_func("/Adwaita/ActionRow/title_lines", test_adw_action_row_title_lines);
  g_test_add_func("/Adwaita/ActionRow/subtitle_lines", test_adw_action_row_subtitle_lines);
  g_test_add_func("/Adwaita/ActionRow/activate", test_adw_action_row_activate);

  return g_test_run();
}
