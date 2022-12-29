/*
 * Copyright (C) 2021 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static double last_value;
static int done_count;

static void value_cb(double value) { last_value = value; }

static void done_cb(void) { done_count++; }

static void test_adw_animation_general(void) {
  Gtk::Button widget;
  auto target = Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  auto target2 = Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));

  auto animation = Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  last_value = 0;
  done_count = 0;

  animation->signal_done().connect(sigc::ptr_fun(done_cb));

  g_assert_true(animation->get_widget() == &widget);
  g_assert_true(animation->get_target()->gobj() ==
           (AdwAnimationTarget *)target->gobj());

  g_assert_true(animation->get_state() == Adw::AnimationState::IDLE);
  g_assert_true(animation->get_value() == 10);
  g_assert_true(last_value == 0);
  g_assert_true(done_count == 0);

  animation->play();

  /* Since the widget is not mapped, the animation will immediately finish */
  g_assert_true(animation->get_state() == Adw::AnimationState::FINISHED);
  g_assert_true(animation->get_value() == 20);
  g_assert_true(last_value == 20);
  g_assert_true(done_count == 1);

  animation->reset();

  g_assert_true(animation->get_value() == 10);
  g_assert_true(last_value == 10);
  g_assert_true(done_count == 1);

  animation->skip();

  g_assert_true(animation->get_state() == Adw::AnimationState::FINISHED);
  g_assert_true(animation->get_value() == 20);
  g_assert_true(last_value == 20);
  g_assert_true(done_count == 2);

  animation->set_target(target2);
  g_assert_true(animation->get_target()->gobj() ==
           (AdwAnimationTarget *)target2->gobj());

  g_assert_true(last_value == 20);
  g_assert_true(done_count == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Animation/general", test_adw_animation_general);

  return g_test_run();
}
