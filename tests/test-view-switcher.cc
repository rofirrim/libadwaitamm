/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_view_switcher_policy(void) {
  Adw::ViewSwitcher view_switcher;

  g_assert_true(view_switcher.get_policy() == Adw::ViewSwitcherPolicy::NARROW);

  view_switcher.set_policy(Adw::ViewSwitcherPolicy::WIDE);
  g_assert_true(view_switcher.get_policy() == Adw::ViewSwitcherPolicy::WIDE);

  view_switcher.set_policy(Adw::ViewSwitcherPolicy::NARROW);
  g_assert_true(view_switcher.get_policy() == Adw::ViewSwitcherPolicy::NARROW);
}

static void test_adw_view_switcher_stack(void) {
  Adw::ViewSwitcher view_switcher;
  Adw::ViewStack *view_stack = Gtk::make_managed<Adw::ViewStack>();

  g_assert_true(view_switcher.get_stack() == nullptr);

  view_switcher.set_stack(view_stack);
  g_assert_true(view_switcher.get_stack() == view_stack);

  view_switcher.set_stack(nullptr);
  g_assert_true(view_switcher.get_stack() == nullptr);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ViewSwitcher/policy",
                  test_adw_view_switcher_policy);
  g_test_add_func("/Adwaita/ViewSwitcher/stack", test_adw_view_switcher_stack);

  return g_test_run();
}
