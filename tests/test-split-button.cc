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

static void notify_cb() { notified++; }

static void test_adw_split_button_icon_name(void) {
  Adw::SplitButton button;

  notified = 0;
  button.property_icon_name().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring icon_name = button.get_property<Glib::ustring>("icon-name");
  g_assert_true(icon_name == "");

  button.set_icon_name("document-open-symbolic");
  g_assert_true(notified == 1);

  button.set_icon_name("document-open-symbolic");
  g_assert_true(button.get_icon_name() == "document-open-symbolic");
  g_assert_true(notified == 1);

  button.set_property<Glib::ustring>("icon-name", "edit-find-symbolic");
  g_assert_true(button.get_icon_name() == "edit-find-symbolic");
  g_assert_true(notified == 2);

  button.set_label("Open");
  g_assert_true(button.get_icon_name() == "");
  g_assert_true(notified == 3);

  button.set_icon_name("document-open-symbolic");
  g_assert_true(button.get_icon_name() == "document-open-symbolic");
  g_assert_true(notified == 4);

  button.set_child(Gtk::make_managed<Gtk::Button>());
  g_assert_true(button.get_icon_name() == "");
  g_assert_true(notified == 5);
}

static void test_adw_split_button_label(void) {
  Adw::SplitButton button;

  notified = 0;
  button.property_label().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring label = button.get_property<Glib::ustring>("label");
  g_assert_true(label == "");

  button.set_label("Open");
  g_assert_true(notified == 1);

  button.set_label("Open");
  g_assert_true(button.get_label() == "Open");
  g_assert_true(notified == 1);

  button.set_property<Glib::ustring>("label", "Find");
  g_assert_true(button.get_label() == "Find");
  g_assert_true(notified == 2);

  button.set_icon_name("document-open-symbolic");
  g_assert_true(button.get_label() == "");
  g_assert_true(notified == 3);

  button.set_label("Open");
  g_assert_true(button.get_label() == "Open");
  g_assert_true(notified == 4);

  button.set_child(Gtk::make_managed<Gtk::Button>());
  g_assert_true(button.get_label() == "");
  g_assert_true(notified == 5);
}

static void test_adw_split_button_use_underline(void) {
  Adw::SplitButton button;

  notified = 0;
  button.property_use_underline().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool use_underline = button.get_property<bool>("use-underline");
  g_assert_false(use_underline);

  button.set_use_underline(false);
  g_assert_true(notified == 0);

  button.set_use_underline(true);
  g_assert_true(button.get_use_underline());
  g_assert_true(notified == 1);

  button.set_property<bool>("use-underline", false);
  g_assert_false(button.get_use_underline());
  g_assert_true(notified == 2);
}

static void test_adw_split_button_child(void) {
  Adw::SplitButton button;

  Gtk::Button *child1 = new Gtk::Button();
  Gtk::Button *child2 = new Gtk::Button();
  Gtk::Button *child3 = new Gtk::Button();

  notified = 0;
  button.property_child().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget *child = button.get_property<Gtk::Widget *>("child");
  g_assert_true(child == nullptr);

  button.set_child(nullptr);
  g_assert_true(notified == 0);

  button.set_child(child1);
  g_assert_true(button.get_child() == child1);
  g_assert_true(notified == 1);

  button.set_property<Gtk::Widget *>("child", child2);
  g_assert_true(button.get_child() == child2);
  g_assert_true(notified == 2);

  button.set_label("Open");
  g_assert_false(button.get_child() == child2);
  g_assert_true(notified == 3);

  button.set_child(child3);
  g_assert_true(button.get_child() == child3);
  g_assert_true(notified == 4);

  button.set_icon_name("document-open-symbolic");
  g_assert_false(button.get_child() == child3);
  g_assert_true(notified == 5);

  delete child3;
  delete child2;
  delete child1;
}

static void test_adw_split_button_menu_model(void) {
  Adw::SplitButton button;

  Glib::RefPtr<Gio::MenuModel> model1 = Gio::Menu::create();
  Glib::RefPtr<Gio::MenuModel> model2 = Gio::Menu::create();

  notified = 0;
  button.property_menu_model().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gio::MenuModel> model =
      button.get_property<Glib::RefPtr<Gio::MenuModel>>("menu-model");
  g_assert_true(model == nullptr);
  g_assert_true(notified == 0);

  button.set_menu_model(model1);
  g_assert_true(button.get_menu_model() == model1);
  g_assert_true(notified == 1);

  button.set_property<Glib::RefPtr<Gio::MenuModel>>("menu-model", model2);
  g_assert_true(button.get_menu_model() == model2);
  g_assert_true(notified == 2);

  button.set_popover(Gtk::make_managed<Gtk::Popover>());
  g_assert_true(button.get_menu_model() == nullptr);
  g_assert_true(notified == 3);
}

static void test_adw_split_button_popover(void) {
  Adw::SplitButton button;

  Gtk::Popover *popover1 = new Gtk::Popover();
  Gtk::Popover *popover2 = new Gtk::Popover();

  notified = 0;
  button.property_popover().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Popover *popover = button.get_property<Gtk::Popover *>("popover");
  g_assert_true(popover == nullptr);
  g_assert_true(notified == 0);

  button.set_popover(popover1);
  g_assert_true(button.get_popover() == popover1);
  g_assert_true(notified == 1);

  button.set_property<Gtk::Popover *>("popover", popover2);
  g_assert_true(button.get_popover() == popover2);
  g_assert_true(notified == 2);

  Glib::RefPtr<Gio::MenuModel> model = Gio::Menu::create();
  button.set_menu_model(model);
  g_assert_false(button.get_popover() == popover2);
  g_assert_true(notified == 3);

  delete popover2;
  delete popover1;
}

static void test_adw_split_button_direction(void) {
  Adw::SplitButton button;

  notified = 0;
  button.property_direction().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Gtk::ArrowType direction = button.get_property<Gtk::ArrowType>("direction");
  g_assert_true(direction == Gtk::ArrowType::DOWN);

  button.set_direction(Gtk::ArrowType::DOWN);
  g_assert_true(notified == 0);

  button.set_direction(Gtk::ArrowType::UP);
  g_assert_true(button.get_direction() == Gtk::ArrowType::UP);
  g_assert_true(notified == 1);

  button.set_property<Gtk::ArrowType>("direction", Gtk::ArrowType::DOWN);
  g_assert_true(button.get_direction() == Gtk::ArrowType::DOWN);
  g_assert_true(notified == 2);
}

static void test_adw_split_button_dropdown_tooltip(void) {
  Adw::SplitButton button;

  notified = 0;
  button.property_dropdown_tooltip().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring tooltip =
      button.get_property<Glib::ustring>("dropdown-tooltip");
  g_assert_true(tooltip == "");
  g_assert_true(notified == 0);

  button.set_dropdown_tooltip("Some tooltip");
  g_assert_true(button.get_dropdown_tooltip() == "Some tooltip");
  g_assert_true(notified == 1);

  button.set_property<Glib::ustring>("dropdown-tooltip", "Some other tooltip");
  g_assert_true(button.get_dropdown_tooltip() == "Some other tooltip");
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/SplitButton/icon_name",
                  test_adw_split_button_icon_name);
  g_test_add_func("/Adwaita/SplitButton/label", test_adw_split_button_label);
  g_test_add_func("/Adwaita/SplitButton/use_underline",
                  test_adw_split_button_use_underline);
  g_test_add_func("/Adwaita/SplitButton/child", test_adw_split_button_child);
  g_test_add_func("/Adwaita/SplitButton/menu_model",
                  test_adw_split_button_menu_model);
  g_test_add_func("/Adwaita/SplitButton/popover",
                  test_adw_split_button_popover);
  g_test_add_func("/Adwaita/SplitButton/direction",
                  test_adw_split_button_direction);
  g_test_add_func("/Adwaita/SplitButton/dropdown_tooltip",
                  test_adw_split_button_dropdown_tooltip);

  return g_test_run();
}
