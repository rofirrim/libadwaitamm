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

static void notify_cb(void) { notified++; }

static void test_adw_bin_child(void) {
  Adw::Bin bin;

  notified = 0;
  bin.property_child().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget *widget = bin.get_property<Gtk::Widget *>("child");
  g_assert_null(widget);

  bin.set_child(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  bin.set_child(widget);
  g_assert_true(bin.get_child() == widget);
  g_assert_true(notified == 1);

  bin.set_property<Gtk::Widget *>("child", nullptr);
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Bin/child", test_adw_bin_child);

  return g_test_run();
}
