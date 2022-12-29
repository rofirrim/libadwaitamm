/*
 * Copyright (C) 2021 Maximiliano Sandoval <msandova@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_toast_overlay_child(void) {
  Adw::ToastOverlay toast_overlay;

  notified = 0;
  toast_overlay.property_child().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Gtk::Widget *widget = toast_overlay.get_property<Gtk::Widget *>("child");
  g_assert_true(widget == nullptr);

  toast_overlay.set_child(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  toast_overlay.set_child(widget);
  g_assert_true(toast_overlay.get_child() == widget);
  g_assert_true(notified == 1);

  toast_overlay.set_property<Gtk::Widget*>("child", nullptr);
  g_assert_true(toast_overlay.get_child() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_toast_overlay_add_toast(void) {
  Adw::ToastOverlay toast_overlay;
  std::unique_ptr<Adw::Toast> toast =
      std::make_unique<Adw::Toast>("Test Notification");

  toast->reference();
  toast_overlay.add_toast(toast.get());
  toast->reference();
  toast_overlay.add_toast(toast.get());
  toast->reference();
  toast_overlay.add_toast(toast.get());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ToastOverlay/child", test_adw_toast_overlay_child);
  g_test_add_func("/Adwaita/ToastOverlay/add_toast",
                  test_adw_toast_overlay_add_toast);

  return g_test_run();
}
