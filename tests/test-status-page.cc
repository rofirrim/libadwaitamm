/*
 * Copyright (C) 2020 Andrei Lișiță <andreii.lisita@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_status_page_icon_name(void) {
  Adw::StatusPage status_page;

  notified = 0;
  status_page.property_icon_name().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring icon_name =
      status_page.get_property<Glib::ustring>("icon-name");
  g_assert(icon_name == "");

  status_page.set_icon_name("");
  g_assert(notified == 0);

  status_page.set_icon_name("some-icon-symbolic");
  g_assert(status_page.get_icon_name() == "some-icon-symbolic");
  g_assert(notified == 1);

  status_page.set_property<Glib::ustring>("icon-name", "other-icon-symbolic");
  g_assert(status_page.get_icon_name() == "other-icon-symbolic");
  g_assert(notified == 2);
}

static void test_adw_status_page_title(void) {
  Adw::StatusPage status_page;

  notified = 0;
  status_page.property_title().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring title = status_page.get_property<Glib::ustring>("title");
  g_assert(title == "");

  status_page.set_title("");
  g_assert(notified == 0);

  status_page.set_title("Some Title");
  g_assert(status_page.get_title() == "Some Title");
  g_assert(notified == 1);

  status_page.set_property<Glib::ustring>("title", "Other Title");
  g_assert(status_page.get_title() == "Other Title");
  g_assert(notified == 2);
}

static void test_adw_status_page_description(void) {
  Adw::StatusPage status_page;

  notified = 0;
  status_page.property_description().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring description =
      status_page.get_property<Glib::ustring>("description");
  g_assert(description == "");

  status_page.set_description("");
  g_assert(notified == 0);

  status_page.set_description("Some description");
  g_assert(status_page.get_description() == "Some description");
  g_assert(notified == 1);

  status_page.set_property<Glib::ustring>("description", "Other description");
  g_assert(status_page.get_description() == "Other description");
  g_assert(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/StatusPage/icon_name",
                  test_adw_status_page_icon_name);
  g_test_add_func("/Adwaita/StatusPage/title", test_adw_status_page_title);
  g_test_add_func("/Adwaita/StatusPage/description",
                  test_adw_status_page_description);

  return g_test_run();
}
