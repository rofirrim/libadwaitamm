/*
 * Copyright (C) 2020 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_flap_flap(void) {
    Adw::Flap flap;
    Gtk::Widget *widget = nullptr;

    notified = 0;
    flap.property_flap().signal_changed().connect(sigc::ptr_fun(notify_cb));

    widget = flap.get_property<Gtk::Widget*>("flap");
    g_assert_null(widget);

    flap.set_flap(nullptr);
    g_assert_true(notified == 0);

    widget = Gtk::make_managed<Gtk::Button>();
    flap.set_flap(widget);
    g_assert_true(flap.get_flap()->gobj() == widget->gobj());
    g_assert_true(notified == 1);

    flap.set_property<Gtk::Widget*>("flap", nullptr);
    g_assert_null(flap.get_flap());
    g_assert_true(notified == 2);
}

static void test_adw_flap_separator(void) {
  Adw::Flap flap;
  Gtk::Widget *widget = nullptr;

  notified = 0;
  flap.property_separator().signal_changed().connect(sigc::ptr_fun(notify_cb));

  widget = flap.get_property<Gtk::Widget *>("separator");
  g_assert_null(widget);

  flap.set_separator(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  flap.set_separator(widget);
  g_assert_true(flap.get_separator()->gobj() == widget->gobj());
  g_assert_true(notified == 1);

  flap.set_property<Gtk::Widget *>("separator", nullptr);
  g_assert_null(flap.get_separator());
  g_assert_true(notified == 2);
}

static void test_adw_flap_flap_position(void) {
  Adw::Flap flap;

  notified = 0;
  flap.property_flap_position().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Gtk::PackType position = flap.get_property<Gtk::PackType>("flap-position");
  g_assert_true(position == Gtk::PackType::START);

  flap.set_flap_position(Gtk::PackType::START);
  g_assert_true(notified == 0);

  flap.set_flap_position(Gtk::PackType::END);
  g_assert_true(flap.get_flap_position() == Gtk::PackType::END);
  g_assert_true(notified == 1);

  flap.set_property<Gtk::PackType>("flap-position", Gtk::PackType::START);
  g_assert_true(flap.get_flap_position() == Gtk::PackType::START);
  g_assert_true(notified == 2);
}

static void test_adw_flap_reveal_flap(void) {
  Adw::Flap flap;

  flap.set_flap(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  flap.property_reveal_flap().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool reveal = flap.get_property<bool>("reveal-flap");
  g_assert_true(reveal);

  flap.set_reveal_flap(true);
  g_assert_true(notified == 0);

  flap.set_reveal_flap(false);
  g_assert_false(flap.get_reveal_flap());
  g_assert_true(notified == 1);

  flap.set_property<bool>("reveal-flap", true);
  g_assert_true(flap.get_reveal_flap());
  g_assert_true(notified == 2);
}

static void test_adw_flap_reveal_progress(void) {
  Adw::Flap flap;

  notified = 0;
  flap.property_reveal_progress().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  double progress = flap.get_property<double>("reveal-progress");
  g_assert_true(progress == 1.0);

  flap.set_reveal_flap(false);
  g_assert_true(flap.get_reveal_progress() == 0.0);
  g_assert_true(notified == 1);

  flap.set_reveal_flap(true);
  g_assert_true(flap.get_reveal_progress() == 1.0);
  g_assert_true(notified == 2);
}

static void test_adw_flap_fold_policy(void) {
  Adw::Flap flap;

  notified = 0;
  flap.property_fold_policy().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Adw::FlapFoldPolicy policy =
      flap.get_property<Adw::FlapFoldPolicy>("fold-policy");
  g_assert_true(policy == Adw::FlapFoldPolicy::AUTO);

  flap.set_fold_policy(Adw::FlapFoldPolicy::AUTO);
  g_assert_true(notified == 0);

  flap.set_fold_policy(Adw::FlapFoldPolicy::NEVER);
  g_assert_true(flap.get_fold_policy() == Adw::FlapFoldPolicy::NEVER);
  g_assert_true(notified == 1);

  flap.set_property<Adw::FlapFoldPolicy>("fold-policy",
                                         Adw::FlapFoldPolicy::ALWAYS);
  g_assert_true(flap.get_fold_policy() == Adw::FlapFoldPolicy::ALWAYS);
  g_assert_true(notified == 2);
}

static void test_adw_flap_fold_duration(void) {
  Adw::Flap flap;

  notified = 0;
  flap.property_fold_duration().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  guint duration = flap.get_property<guint>("fold-duration");
  g_assert_true(duration == 250);

  flap.set_fold_duration(250);
  g_assert_true(notified == 0);

  flap.set_fold_duration(500);
  g_assert_true(flap.get_fold_duration() == 500);
  g_assert_true(notified == 1);

  flap.set_property<guint>("fold-duration", 100);
  g_assert_true(flap.get_fold_duration() == 100);
  g_assert_true(notified == 2);
}

static void test_adw_flap_folded(void) {
  Adw::Flap flap;

  flap.set_flap(Gtk::make_managed<Gtk::Button>());
  flap.set_fold_policy(Adw::FlapFoldPolicy::NEVER);

  notified = 0;
  flap.property_folded().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool folded = flap.get_property<bool>("folded");
  g_assert_false(folded);

  flap.set_fold_policy(Adw::FlapFoldPolicy::ALWAYS);
  g_assert_true(flap.get_folded());
  g_assert_true(notified == 1);
}

static void test_adw_flap_locked(void) {
  Adw::Flap flap;

  flap.set_flap(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  flap.property_locked().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool locked = flap.get_property<bool>("locked");
  g_assert_false(locked);

  flap.set_locked(false);
  g_assert_true(notified == 0);

  flap.set_locked(true);
  g_assert_true(flap.get_locked());
  g_assert_true(notified == 1);

  flap.set_property<bool>("locked", false);
  g_assert_false(flap.get_locked());
  g_assert_true(notified == 2);
}

static void test_adw_flap_transition_type(void) {
  Adw::Flap flap;

  notified = 0;
  flap.property_transition_type().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Adw::FlapTransitionType policy =
      flap.get_property<Adw::FlapTransitionType>("transition-type");
  g_assert_true(policy == Adw::FlapTransitionType::OVER);

  flap.set_transition_type(Adw::FlapTransitionType::OVER);
  g_assert_true(notified == 0);

  flap.set_transition_type(Adw::FlapTransitionType::SLIDE);
  g_assert_true(flap.get_transition_type() == Adw::FlapTransitionType::SLIDE);
  g_assert_true(notified == 1);

  flap.set_property<Adw::FlapTransitionType>("transition-type",
                                             Adw::FlapTransitionType::UNDER);
  g_assert_true(flap.get_transition_type() == Adw::FlapTransitionType::UNDER);
  g_assert_true(notified == 2);
}

static void test_adw_flap_modal(void) {
  Adw::Flap flap;

  flap.set_flap(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  flap.property_modal().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool modal = flap.get_property<bool>("modal");
  g_assert_true(modal);

  flap.set_modal(true);
  g_assert_true(notified == 0);

  flap.set_modal(false);
  g_assert_false(flap.get_modal());
  g_assert_true(notified == 1);

  flap.set_property<bool>("modal", true);
  g_assert_true(flap.get_modal());
  g_assert_true(notified == 2);
}

static void test_adw_flap_swipe_to_open(void) {
  Adw::Flap flap;

  flap.set_flap(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  flap.property_swipe_to_open().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool swipe_to_open = flap.get_property<bool>("swipe-to-open");
  g_assert_true(swipe_to_open);

  flap.set_swipe_to_open(true);
  g_assert_true(notified == 0);

  flap.set_swipe_to_open(false);
  g_assert_false(flap.get_swipe_to_open());
  g_assert_true(notified == 1);

  flap.set_property<bool>("swipe-to-open", true);
  g_assert_true(flap.get_swipe_to_open());
  g_assert_true(notified == 2);
}

static void test_adw_flap_swipe_to_close(void) {
  Adw::Flap flap;

  flap.set_flap(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  flap.property_swipe_to_close().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool swipe_to_close = flap.get_property<bool>("swipe-to-close");
  g_assert_true(swipe_to_close);

  flap.set_swipe_to_close(true);
  g_assert_true(notified == 0);

  flap.set_swipe_to_close(false);
  g_assert_false(flap.get_swipe_to_close());
  g_assert_true(notified == 1);

  flap.set_property<bool>("swipe-to-close", true);
  g_assert_true(flap.get_swipe_to_close());
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Flap/flap", test_adw_flap_flap);
  g_test_add_func("/Adwaita/Flap/separator", test_adw_flap_separator);
  g_test_add_func("/Adwaita/Flap/flap_position", test_adw_flap_flap_position);
  g_test_add_func("/Adwaita/Flap/reveal_flap", test_adw_flap_reveal_flap);
  g_test_add_func("/Adwaita/Flap/reveal_progress",
                  test_adw_flap_reveal_progress);
  g_test_add_func("/Adwaita/Flap/fold_policy", test_adw_flap_fold_policy);
  g_test_add_func("/Adwaita/Flap/fold_duration", test_adw_flap_fold_duration);
  g_test_add_func("/Adwaita/Flap/folded", test_adw_flap_folded);
  g_test_add_func("/Adwaita/Flap/locked", test_adw_flap_locked);
  g_test_add_func("/Adwaita/Flap/transition_type",
                  test_adw_flap_transition_type);
  g_test_add_func("/Adwaita/Flap/modal", test_adw_flap_modal);
  g_test_add_func("/Adwaita/Flap/swipe_to_open", test_adw_flap_swipe_to_open);
  g_test_add_func("/Adwaita/Flap/swipe_to_close", test_adw_flap_swipe_to_close);

  return g_test_run();
}
