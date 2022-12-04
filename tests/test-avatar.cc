/*
 * Copyright (C) 2020 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

#define TEST_ICON_NAME "avatar-default-symbolic"
#define TEST_STRING "Mario Rossi"
#define TEST_SIZE 128

static void test_adw_avatar_icon_name(void) {
  Adw::Avatar avatar(128, "", true);
  g_assert(avatar.get_icon_name() == "");

  avatar.set_icon_name(TEST_ICON_NAME);
  g_assert(avatar.get_icon_name() == TEST_ICON_NAME);
}

static void test_adw_avatar_text(void) {
  Adw::Avatar avatar(128, "", true);
  g_assert(avatar.get_text() == "");

  avatar.set_text(TEST_STRING);
  g_assert(avatar.get_text() == TEST_STRING);
}

static void test_adw_avatar_size(void) {
  Adw::Avatar avatar(TEST_SIZE, "", true);

  g_assert(avatar.get_size() == TEST_SIZE);
  avatar.set_size(TEST_SIZE / 2);
  g_assert(avatar.get_size() == TEST_SIZE / 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Avatar/icon_name", test_adw_avatar_icon_name);
  g_test_add_func("/Adwaita/Avatar/text", test_adw_avatar_text);
  g_test_add_func("/Adwaita/Avatar/size", test_adw_avatar_size);

  return g_test_run();
}
