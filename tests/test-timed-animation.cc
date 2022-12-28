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

static void value_cb(double) {}

static void notify_cb() { notified++; }

static void test_adw_animation_value_from(void) {
  Gtk::Button widget;
  Glib::RefPtr<Adw::AnimationTarget> target =
      Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  Glib::RefPtr<Adw::TimedAnimation> animation =
      Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  notified = 0;
  animation->property_value_from().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  double value = animation->get_property<double>("value-from");
  g_assert(value == 10);
  g_assert(notified == 0);

  animation->set_value_from(20);
  value = animation->get_property<double>("value-from");
  g_assert(value == 20);
  g_assert(notified == 1);

  animation->set_property<double>("value-from", 30.0);
  g_assert(animation->get_value_from() == 30);
  g_assert(notified == 2);
}

static void test_adw_animation_value_to(void) {
  Gtk::Button widget;
  Glib::RefPtr<Adw::AnimationTarget> target =
      Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  Glib::RefPtr<Adw::TimedAnimation> animation =
      Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  animation->skip();

  notified = 0;
  animation->property_value_to().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  double value = animation->get_property<double>("value-to");
  g_assert(value == 20);
  g_assert(notified == 0);

  animation->set_value_to(10);
  value = animation->get_property<double>("value-to");
  g_assert(value == 10);
  g_assert(notified == 1);

  animation->set_property<double>("value-to", 30.0);
  g_assert(animation->get_value_to() == 30);
  g_assert(notified == 2);
}

static void test_adw_animation_duration(void) {
  Gtk::Button widget;
  Glib::RefPtr<Adw::AnimationTarget> target =
      Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  Glib::RefPtr<Adw::TimedAnimation> animation =
      Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  notified = 0;
  animation->property_duration().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  unsigned int duration = animation->get_property<unsigned int>("duration");
  g_assert(duration == 100);
  g_assert(notified == 0);

  animation->set_duration(200);
  duration = animation->get_property<unsigned int>("duration");
  g_assert(duration == 200);
  g_assert(notified == 1);

  animation->set_property<unsigned int>("duration", 300u);
  g_assert(animation->get_duration() == 300);
  g_assert(notified == 2);
}

static void test_adw_animation_easing(void) {
  Gtk::Button widget;
  Glib::RefPtr<Adw::AnimationTarget> target =
      Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  Glib::RefPtr<Adw::TimedAnimation> animation =
      Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  notified = 0;
  animation->property_easing().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Adw::Easing easing = animation->get_property<Adw::Easing>("easing");
  g_assert(easing == Adw::Easing::EASE_OUT_CUBIC);
  g_assert(notified == 0);

  animation->set_easing(Adw::Easing::EASE_IN_CUBIC);
  easing = animation->get_property<Adw::Easing>("easing");
  g_assert(easing == Adw::Easing::EASE_IN_CUBIC);
  g_assert(notified == 1);

  animation->set_property<Adw::Easing>("easing",
                                       Adw::Easing::EASE_IN_OUT_CUBIC);
  g_assert(animation->get_easing() == Adw::Easing::EASE_IN_OUT_CUBIC);
  g_assert(notified == 2);
}

static void test_adw_animation_repeat_count(void) {
  Gtk::Button widget;
  Glib::RefPtr<Adw::AnimationTarget> target =
      Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  Glib::RefPtr<Adw::TimedAnimation> animation =
      Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  notified = 0;
  animation->property_repeat_count().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  unsigned int repeat_count =
      animation->get_property<unsigned int>("repeat-count");
  g_assert(repeat_count == 1);
  g_assert(notified == 0);

  animation->set_repeat_count(2);
  repeat_count = animation->get_property<unsigned int>("repeat-count");
  g_assert(repeat_count == 2);
  g_assert(notified == 1);

  animation->set_property<unsigned int>("repeat-count", 3u);
  g_assert(animation->get_repeat_count() == 3);
  g_assert(notified == 2);
}

static void test_adw_animation_reverse(void) {
  Gtk::Button widget;
  Glib::RefPtr<Adw::AnimationTarget> target =
      Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  Glib::RefPtr<Adw::TimedAnimation> animation =
      Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  notified = 0;
  animation->property_reverse().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool reverse = animation->get_property<bool>("reverse");
  g_assert_false(reverse);
  g_assert(notified == 0);

  animation->set_reverse(true);
  reverse = animation->get_property<bool>("reverse");
  g_assert_true(reverse);
  g_assert(notified == 1);

  animation->set_property<bool>("reverse", false);
  g_assert_false(animation->get_reverse());
  g_assert(notified == 2);
}

static void test_adw_animation_alternate(void) {
  Gtk::Button widget;
  Glib::RefPtr<Adw::AnimationTarget> target =
      Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  Glib::RefPtr<Adw::TimedAnimation> animation =
      Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  notified = 0;
  animation->property_alternate().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool alternate = animation->get_property<bool>("alternate");
  g_assert_false(alternate);
  g_assert(notified == 0);

  animation->set_alternate(true);
  alternate = animation->get_property<bool>("alternate");
  g_assert_true(alternate);
  g_assert(notified == 1);

  animation->set_property<bool>("alternate", false);
  g_assert_false(animation->get_alternate());
  g_assert(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/TimedAnimation/value_from",
                  test_adw_animation_value_from);
  g_test_add_func("/Adwaita/TimedAnimation/value_to",
                  test_adw_animation_value_to);
  g_test_add_func("/Adwaita/TimedAnimation/duration",
                  test_adw_animation_duration);
  g_test_add_func("/Adwaita/TimedAnimation/easing", test_adw_animation_easing);
  g_test_add_func("/Adwaita/TimedAnimation/repeat_count",
                  test_adw_animation_repeat_count);
  g_test_add_func("/Adwaita/TimedAnimation/reverse",
                  test_adw_animation_reverse);
  g_test_add_func("/Adwaita/TimedAnimation/alternate",
                  test_adw_animation_alternate);

  return g_test_run();
}
