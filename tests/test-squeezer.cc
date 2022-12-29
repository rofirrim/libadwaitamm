/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_squeezer_homogeneous(void) {
  Adw::Squeezer squeezer;

  g_assert_true(squeezer.get_homogeneous());

  squeezer.set_homogeneous(false);
  g_assert_false(squeezer.get_homogeneous());

  squeezer.set_homogeneous(true);
  g_assert_true(squeezer.get_homogeneous());
}

static void test_adw_squeezer_allow_none(void) {
  Adw::Squeezer squeezer;

  g_assert_false(squeezer.get_allow_none());

  squeezer.set_allow_none(true);
  g_assert_true(squeezer.get_allow_none());

  squeezer.set_allow_none(false);
  g_assert_false(squeezer.get_allow_none());
}

static void test_adw_squeezer_transition_duration(void) {
  Adw::Squeezer squeezer;

  g_assert_true(squeezer.get_transition_duration() == 200);

  squeezer.set_transition_duration(400);
  g_assert_true(squeezer.get_transition_duration() == 400);

  squeezer.set_transition_duration(-1);
  g_assert_true(squeezer.get_transition_duration() == G_MAXUINT);
}

static void test_adw_squeezer_transition_type(void) {
  Adw::Squeezer squeezer;

  g_assert_true(squeezer.get_transition_type() == Adw::SqueezerTransitionType::NONE);

  squeezer.set_transition_type(Adw::SqueezerTransitionType::CROSSFADE);
  g_assert_true(squeezer.get_transition_type() ==
           Adw::SqueezerTransitionType::CROSSFADE);

  squeezer.set_transition_type(Adw::SqueezerTransitionType::NONE);
  g_assert_true(squeezer.get_transition_type() == Adw::SqueezerTransitionType::NONE);
}

static void test_adw_squeezer_transition_running(void) {
  Adw::Squeezer squeezer;

  g_assert_false(squeezer.get_transition_running());
}

static void test_adw_squeezer_show_hide_child(void) {
  Adw::Squeezer squeezer;

  g_assert_true(squeezer.get_visible_child() == nullptr);

  Gtk::Label *child = Gtk::make_managed<Gtk::Label>("");

  squeezer.add(child);
  g_assert_true(squeezer.get_visible_child() == child);

  child->hide();
  g_assert_true(squeezer.get_visible_child() == nullptr);

  child->show();
  g_assert_true(squeezer.get_visible_child() == child);

  squeezer.remove(child);
  g_assert_true(squeezer.get_visible_child() == nullptr);
}

static void test_adw_squeezer_interpolate_size(void) {
  Adw::Squeezer squeezer;

  g_assert_false(squeezer.get_interpolate_size());

  squeezer.set_interpolate_size(true);
  g_assert_true(squeezer.get_interpolate_size());

  squeezer.set_interpolate_size(false);
  g_assert_false(squeezer.get_interpolate_size());
}

static void test_adw_squeezer_page_enabled(void) {
  Adw::Squeezer squeezer;

  Gtk::Label *child = Gtk::make_managed<Gtk::Label>();

  Glib::RefPtr<Adw::SqueezerPage> page = squeezer.add(child);
  g_assert_true(page->get_enabled());

  page->set_enabled(false);
  g_assert_false(page->get_enabled());

  page->set_enabled(true);
  g_assert_true(page->get_enabled());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ViewSwitcher/homogeneous",
                  test_adw_squeezer_homogeneous);
  g_test_add_func("/Adwaita/ViewSwitcher/allow_none",
                  test_adw_squeezer_allow_none);
  g_test_add_func("/Adwaita/ViewSwitcher/transition_duration",
                  test_adw_squeezer_transition_duration);
  g_test_add_func("/Adwaita/ViewSwitcher/transition_type",
                  test_adw_squeezer_transition_type);
  g_test_add_func("/Adwaita/ViewSwitcher/transition_running",
                  test_adw_squeezer_transition_running);
  g_test_add_func("/Adwaita/ViewSwitcher/show_hide_child",
                  test_adw_squeezer_show_hide_child);
  g_test_add_func("/Adwaita/ViewSwitcher/interpolate_size",
                  test_adw_squeezer_interpolate_size);
  g_test_add_func("/Adwaita/ViewSwitcher/page_enabled",
                  test_adw_squeezer_page_enabled);

  return g_test_run();
}
