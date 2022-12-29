/*
 * Copyright (C) 2022 George Barrett <bob@bob131.so>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_property_animation_target_construct(void) {
  auto widget = Glib::RefPtr<Gtk::Button>(new Gtk::Button());

  auto named_target = Adw::PropertyAnimationTarget::create(widget, "opacity");

  GParamSpec *target_pspec = named_target->get_pspec();
  g_assert_true(target_pspec);
  g_assert_true(Glib::ustring(target_pspec->name) == "opacity");

  auto pspec_target = Adw::PropertyAnimationTarget::create(widget, target_pspec);
  g_assert_true(pspec_target->get_pspec() == target_pspec);

  target_pspec = pspec_target->get_pspec();
  g_assert_true(target_pspec);
  g_assert_true(Glib::ustring(target_pspec->name) == "opacity");
}

static void test_adw_property_animation_target_basic(void) {
  auto widget = Glib::RefPtr<Gtk::Button>(new Gtk::Button());

  auto target = Adw::PropertyAnimationTarget::create(widget, "opacity");
  auto animation = Adw::TimedAnimation::create(widget.get(), 1, 0, 0, target);

  g_assert_true(widget->get_opacity() == 1);

  animation->play();
  g_assert_true(widget->get_opacity() == 0);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/PropertyAnimationTarget/construct",
                  test_adw_property_animation_target_construct);
  g_test_add_func("/Adwaita/PropertyAnimationTarget/basic",
                  test_adw_property_animation_target_basic);

  return g_test_run();
}
