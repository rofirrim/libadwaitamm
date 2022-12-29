/*
 * Copyright (C) 2020 Alexander Mikhaylenko <alexm@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

static void test_adw_carousel_indicator_lines_carousel(void) {
    Adw::CarouselIndicatorLines lines;
    Adw::Carousel carousel;

  notified = 0;
  lines.property_carousel().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_null(lines.get_carousel());
  g_assert_true(notified == 0);

  lines.set_carousel(&carousel);
  g_assert_true(lines.get_carousel()->gobj() == carousel.gobj());
  g_assert_true(notified == 1);

  lines.set_carousel(nullptr);
  g_assert_null(lines.get_carousel());
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/CarouselInidicatorLines/carousel",
                  test_adw_carousel_indicator_lines_carousel);
  return g_test_run();
}
