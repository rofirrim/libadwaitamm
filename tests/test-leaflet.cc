/*
 * Copyright (C) 2020 Alexander Mikhaylenko <alexm@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void assert_page_position(Glib::RefPtr<Gtk::SelectionModel> pages,
                                 Gtk::Widget *widget, int position) {
  // I'm not sure I understand why SelectionModel does not inherit from
  // Glib::ListModel so we can access the virtual functions.
  Glib::RefPtr<Adw::LeafletPage> page = Glib::wrap(ADW_LEAFLET_PAGE(
      g_list_model_get_item(G_LIST_MODEL(pages->gobj()), position)));

  g_assert_true(widget->gobj() == page->get_child()->gobj());
}

static void test_adw_leaflet_adjacent_child(void) {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget *, 3> children;

  for (int i = 0; i < 3; i++) {
    children[i] = Gtk::make_managed<Gtk::Button>();

    Glib::RefPtr<Adw::LeafletPage> page = leaflet.append(children[i]);

    if (i == 1)
      page->set_navigatable(false);
  }

  leaflet.set_visible_child(children[0]);

  Gtk::Widget *result =
      leaflet.get_adjacent_child(Adw::NavigationDirection::BACK);
  g_assert_null(result);

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::FORWARD);
  g_assert_true(result->gobj() == children[2]->gobj());

  leaflet.set_visible_child(children[1]);

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::BACK);
  g_assert_true(result->gobj() == children[0]->gobj());

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::FORWARD);
  g_assert_true(result->gobj() == children[2]->gobj());

  leaflet.set_visible_child(children[2]);

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::BACK);
  g_assert_true(result->gobj() == children[0]->gobj());

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::FORWARD);
  g_assert_null(result);
}

static void test_adw_leaflet_navigate(void) {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget *, 3> children;

  bool result = leaflet.navigate(Adw::NavigationDirection::BACK);
  g_assert_false(result);

  result = leaflet.navigate(Adw::NavigationDirection::FORWARD);
  g_assert_false(result);

  for (int i = 0; i < 3; i++) {
    children[i] = Gtk::make_managed<Gtk::Label>("");

    Glib::RefPtr<Adw::LeafletPage> page = leaflet.append(children[i]);

    if (i == 1)
      page->set_navigatable(false);
  }

  leaflet.set_visible_child(children[0]);

  result = leaflet.navigate(Adw::NavigationDirection::BACK);
  g_assert_false(result);

  result = leaflet.navigate(Adw::NavigationDirection::FORWARD);
  g_assert_true(result);
  g_assert_true(leaflet.get_visible_child()->gobj() == children[2]->gobj());

  result = leaflet.navigate(Adw::NavigationDirection::FORWARD);
  g_assert_false(result);

  result = leaflet.navigate(Adw::NavigationDirection::BACK);
  g_assert_true(result);
  g_assert_true(leaflet.get_visible_child()->gobj() == children[0]->gobj());
}

static void test_adw_leaflet_prepend(void) {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget *, 2> labels;

  for (int i = 0; i < 2; i++) {
    labels[i] = Gtk::make_managed<Gtk::Label>("");
  }

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();

  leaflet.prepend(labels[1]);
  assert_page_position(pages, labels[1], 0);

  leaflet.prepend(labels[0]);
  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[1], 1);
}

static void test_adw_leaflet_insert_child_after(void) {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget *, 3> labels;

  for (int i = 0; i < 3; i++) {
    labels[i] = Gtk::make_managed<Gtk::Label>("");
  }

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();

  leaflet.append(labels[2]);

  assert_page_position(pages, labels[2], 0);

  leaflet.insert_child_after(labels[0], nullptr);
  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[2], 1);

  leaflet.insert_child_after(labels[1], labels[0]);
  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[1], 1);
  assert_page_position(pages, labels[2], 2);
}

static void test_adw_leaflet_reorder_child_after(void) {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget *, 3> labels;

  for (int i = 0; i < 3; i++) {
    labels[i] = Gtk::make_managed<Gtk::Label>("");

    leaflet.append(labels[i]);
  }

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();

  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[1], 1);
  assert_page_position(pages, labels[2], 2);

  leaflet.reorder_child_after(labels[2], nullptr);
  assert_page_position(pages, labels[2], 0);
  assert_page_position(pages, labels[0], 1);
  assert_page_position(pages, labels[1], 2);

  leaflet.reorder_child_after(labels[0], labels[1]);
  assert_page_position(pages, labels[2], 0);
  assert_page_position(pages, labels[1], 1);
  assert_page_position(pages, labels[0], 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Leaflet/adjacent_child",
                  test_adw_leaflet_adjacent_child);
  g_test_add_func("/Adwaita/Leaflet/navigate", test_adw_leaflet_navigate);
  g_test_add_func("/Adwaita/Leaflet/prepend", test_adw_leaflet_prepend);
  g_test_add_func("/Adwaita/Leaflet/insert_child_after",
                  test_adw_leaflet_insert_child_after);
  g_test_add_func("/Adwaita/Leaflet/reorder_child_after",
                  test_adw_leaflet_reorder_child_after);

  return g_test_run();
}
