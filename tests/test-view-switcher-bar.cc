/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_view_switcher_bar_stack(void) {
  Adw::ViewSwitcherBar bar;
  Adw::ViewStack *stack = Gtk::make_managed<Adw::ViewStack>();

  bar.set_stack(stack);
  g_assert(bar.get_stack() == stack);

  bar.set_stack(nullptr);
  g_assert(bar.get_stack() == nullptr);
}

static void test_adw_view_switcher_bar_reveal(void) {
  Adw::ViewSwitcherBar bar;

  g_assert_false(bar.get_reveal());

  bar.set_reveal(true);
  g_assert_true(bar.get_reveal());

  bar.set_reveal(false);
  g_assert_false(bar.get_reveal());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ViewSwitcherBar/stack",
                  test_adw_view_switcher_bar_stack);
  g_test_add_func("/Adwaita/ViewSwitcherBar/reveal",
                  test_adw_view_switcher_bar_reveal);

  return g_test_run();
}
