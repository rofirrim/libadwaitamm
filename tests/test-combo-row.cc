/*
 * Copyright (C) 2018 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_combo_row_set_for_enum(void) {
  Adw::ComboRow row;
  Glib::RefPtr<Gtk::Expression<Glib::ustring>> expr;
  Glib::RefPtr<Gio::ListModel> model;
  Glib::RefPtr<Adw::EnumListItem> item;

  g_assert(row.get_model() == nullptr);

  expr = Gtk::PropertyExpression<Glib::ustring>::create(
      Adw::EnumListItem::get_type(), "nick");
  row.set_expression(expr);

  // Enums don't have a way to retrieve its GType in gtkmm.
  model = Adw::EnumListModel::create(GTK_TYPE_ORIENTATION);
  row.set_model(model);

  model = row.get_model();
  g_assert(model != nullptr);

  g_assert(model->get_n_items() == 2);

  item = std::dynamic_pointer_cast<Adw::EnumListItem>(model->get_object(0));
  g_assert(item != nullptr);
  g_assert(item->get_nick() == "horizontal");

  item = std::dynamic_pointer_cast<Adw::EnumListItem>(model->get_object(1));
  g_assert(item != nullptr);
  g_assert(item->get_nick() == "vertical");
}

static void test_adw_combo_row_selected(void) {
  Adw::ComboRow row;
  Glib::RefPtr<Gio::ListModel> model;
  int selected = 0;

  notified = 0;
  row.property_selected().signal_changed().connect(sigc::ptr_fun(notify_cb));

  selected = row.get_property<int>("selected");
  g_assert(selected == -1);

  row.set_selected(-1);
  g_assert(notified == 0);

  // Enums don't have a way to retrieve its GType in gtkmm.
  model = Adw::EnumListModel::create(GTK_TYPE_SELECTION_MODE);
  row.set_model(model);

  g_assert(row.get_selected() == 0);
  g_assert(notified == 1);

  row.set_selected(3);
  g_assert(row.get_selected() == 3);
  g_assert(notified == 2);

  row.set_property<int>("selected", 1);
  g_assert(row.get_selected() == 1);
  g_assert(notified == 3);
}

static void test_adw_combo_row_use_subtitle(void) {
  Adw::ComboRow row;

  notified = 0;
  row.property_use_subtitle().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_false(row.get_use_subtitle());

  row.set_use_subtitle(false);
  g_assert(notified == 0);

  row.set_use_subtitle(true);
  g_assert_true(row.get_use_subtitle());
  g_assert(notified == 1);

  row.set_property<bool>("use-subtitle", false);
  bool use_subtitle = row.get_property<bool>("use-subtitle");
  g_assert_false(use_subtitle);
  g_assert(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ComboRow/set_for_enum",
                  test_adw_combo_row_set_for_enum);
  g_test_add_func("/Adwaita/ComboRow/selected", test_adw_combo_row_selected);
  g_test_add_func("/Adwaita/ComboRow/use_subtitle",
                  test_adw_combo_row_use_subtitle);

  return g_test_run();
}
