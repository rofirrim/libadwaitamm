/*
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_preferences_group_add_remove(void) {
  Adw::PreferencesGroup group;

  Adw::PreferencesRow *row = Gtk::make_managed<Adw::PreferencesRow>();
  group.add(row);

  Gtk::Switch *widget = Gtk::make_managed<Gtk::Switch>();
  group.add(widget);

  g_assert_true(G_TYPE_CHECK_INSTANCE_TYPE(
      gtk_widget_get_parent(GTK_WIDGET(row->gobj())), GTK_TYPE_LIST_BOX));
  g_assert_true(G_TYPE_CHECK_INSTANCE_TYPE(
      gtk_widget_get_parent(GTK_WIDGET(widget->gobj())), GTK_TYPE_BOX));

  group.remove(row);
  group.remove(widget);
}

static void test_adw_preferences_group_title(void) {
  Adw::PreferencesGroup group;

  g_assert_true(group.get_title() == "");

  group.set_title("Dummy title");
  g_assert_true(group.get_title() == "Dummy title");

  group.set_title("");
  g_assert_true(group.get_title() == "");
}

static void test_adw_preferences_group_description(void) {
  Adw::PreferencesGroup group;

  g_assert_true(group.get_description() == "");

  group.set_description("Dummy description");
  g_assert_true(group.get_description() == "Dummy description");

  group.set_description("");
  g_assert_true(group.get_description() == "");
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/PreferencesGroup/add_remove",
                  test_adw_preferences_group_add_remove);
  g_test_add_func("/Adwaita/PreferencesGroup/title",
                  test_adw_preferences_group_title);
  g_test_add_func("/Adwaita/PreferencesGroup/description",
                  test_adw_preferences_group_description);

  return g_test_run();
}
