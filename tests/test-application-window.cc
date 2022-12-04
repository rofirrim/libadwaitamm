/*
 * Copyright (C) 2020 Alexander Mikhaylenko <alexm@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_application_window_new(void) {
  Glib::RefPtr<Gtk::Application> app; // none
  Adw::ApplicationWindow window(app);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ApplicationWindow/new",
                  test_adw_application_window_new);

  return g_test_run();
}
