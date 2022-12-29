/*
 * Copyright (C) 2020 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init
#include <array>

int notified;

static void notify_cb(void) { notified++; }

template <size_t N>
static void add_pages(Adw::TabView &view,
                      std::array<Glib::RefPtr<Adw::TabPage>, N> &pages, int n,
                      int n_pinned) {
  for (int i = 0; i < n_pinned; i++)
    pages[i] = view.append_pinned(Gtk::make_managed<Gtk::Button>());

  for (int i = n_pinned; i < n; i++)
    pages[i] = view.append(Gtk::make_managed<Gtk::Button>());
}

template <size_t N>
static void
assert_page_positions(Adw::TabView &view,
                      std::array<Glib::RefPtr<Adw::TabPage>, N> &pages, int n,
                      int n_pinned, std::initializer_list<int> indices) {

  g_assert_true(view.get_n_pages() == n);
  g_assert_true(view.get_n_pinned_pages() == n_pinned);

  g_assert_true(indices.end() - indices.begin() == n);
  std::initializer_list<int>::iterator it = indices.begin();
  for (int i = 0; i < n; i++, it++) {
    int index = *it;

    if (index >= 0)
      g_assert_true(view.get_page_position(pages[index]) == i);
  }
}

static bool close_noop(const Glib::RefPtr<Adw::TabPage> &) {
  return GDK_EVENT_STOP;
}

template <typename ...T>
static void check_selection_non_null(T...,
                                     Adw::TabView &view) {
  g_assert_true(view.get_selected_page() != nullptr);
}

template <typename ...T>
static void check_selection_null(T...,
                                 Adw::TabView &view) {
  g_assert_true(view.get_selected_page() == nullptr);
}

static void test_adw_tab_view_n_pages(void) {
  Adw::TabView view;

  notified = 0;
  view.property_n_pages().signal_changed().connect(sigc::ptr_fun(notify_cb));

  int n_pages = view.get_property<int>("n-pages");
  g_assert_true(n_pages == 0);

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());
  n_pages = view.get_property<int>("n-pages");
  g_assert_true(n_pages == 1);
  g_assert_true(view.get_n_pages() == 1);
  g_assert_true(notified == 1);

  view.append(Gtk::make_managed<Gtk::Button>());
  g_assert_true(view.get_n_pages() == 2);
  g_assert_true(notified == 2);

  view.append_pinned(Gtk::make_managed<Gtk::Button>());
  g_assert_true(view.get_n_pages() == 3);
  g_assert_true(notified == 3);

  view.reorder_forward(page);
  g_assert_true(view.get_n_pages() == 3);
  g_assert_true(notified == 3);

  view.close_page(page);
  g_assert_true(view.get_n_pages() == 2);
  g_assert_true(notified == 4);
}

static void test_adw_tab_view_n_pinned_pages(void) {
  Adw::TabView view;

  notified = 0;
  view.property_n_pinned_pages().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  int n_pages = view.get_property<int>("n-pinned-pages");
  g_assert_true(n_pages == 0);

  view.append_pinned(Gtk::make_managed<Gtk::Button>());
  n_pages = view.get_property<int>("n-pinned-pages");
  g_assert_true(n_pages == 1);
  g_assert_true(view.get_n_pinned_pages() == 1);
  g_assert_true(notified == 1);

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());
  g_assert_true(view.get_n_pinned_pages() == 1);
  g_assert_true(notified == 1);

  view.set_page_pinned(page, true);
  g_assert_true(view.get_n_pinned_pages() == 2);
  g_assert_true(notified == 2);

  view.reorder_backward(page);
  g_assert_true(view.get_n_pinned_pages() == 2);
  g_assert_true(notified == 2);

  view.set_page_pinned(page, false);
  g_assert_true(view.get_n_pinned_pages() == 1);
  g_assert_true(notified == 3);
}

static void test_adw_tab_view_default_icon(void) {
  Adw::TabView view;
  Glib::RefPtr<Gio::Icon> icon1 =
      Gio::ThemedIcon::create("go-previous-symbolic");
  Glib::RefPtr<Gio::Icon> icon2 = Gio::ThemedIcon::create("go-next-symbolic");

  notified = 0;
  view.property_default_icon().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring icon_str = view.get_default_icon()->to_string();
  g_assert_true(icon_str == "adw-tab-icon-missing-symbolic");
  g_assert_true(notified == 0);

  view.set_default_icon(icon1);
  g_assert_true(view.get_default_icon() == icon1);
  g_assert_true(notified == 1);

  view.set_property<Glib::RefPtr<Gio::Icon>>("default-icon", icon2);
  g_assert_true(view.get_default_icon() == icon2);
  g_assert_true(notified == 2);
}

static void test_adw_tab_view_menu_model(void) {
  Adw::TabView view;

  Glib::RefPtr<Gio::MenuModel> model1 = Gio::Menu::create();
  Glib::RefPtr<Gio::MenuModel> model2 = Gio::Menu::create();

  notified = 0;
  view.property_menu_model().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gio::MenuModel> model =
      view.get_property<Glib::RefPtr<Gio::MenuModel>>("menu-model");
  g_assert_true(model == nullptr);
  g_assert_true(notified == 0);

  view.set_menu_model(model1);
  g_assert_true(view.get_menu_model() == model1);
  g_assert_true(notified == 1);

  view.set_property<Glib::RefPtr<Gio::MenuModel>>("menu-model", model2);
  g_assert_true(view.get_menu_model() == model2);
  g_assert_true(notified == 2);
}

static void test_adw_tab_view_shortcuts(void) {
  Adw::TabView view;

  notified = 0;
  view.property_shortcuts().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::TabViewShortcuts shortcuts =
      view.get_property<Adw::TabViewShortcuts>("shortcuts");
  g_assert_true(shortcuts == Adw::TabViewShortcuts::ALL_SHORTCUTS);
  g_assert_true(notified == 0);

  view.set_shortcuts(Adw::TabViewShortcuts::CONTROL_PAGE_UP);
  g_assert_true(view.get_shortcuts() == Adw::TabViewShortcuts::CONTROL_PAGE_UP);
  g_assert_true(notified == 1);

  view.set_property<Adw::TabViewShortcuts>(
      "shortcuts", Adw::TabViewShortcuts::CONTROL_PAGE_DOWN);
  g_assert_true(view.get_shortcuts() == Adw::TabViewShortcuts::CONTROL_PAGE_DOWN);
  g_assert_true(notified == 2);

  view.add_shortcuts(Adw::TabViewShortcuts::CONTROL_HOME);
  g_assert_true(view.get_shortcuts() == (Adw::TabViewShortcuts::CONTROL_PAGE_DOWN |
                                    Adw::TabViewShortcuts::CONTROL_HOME));
  g_assert_true(notified == 3);

  view.remove_shortcuts(Adw::TabViewShortcuts::CONTROL_PAGE_DOWN);
  g_assert_true(view.get_shortcuts() == Adw::TabViewShortcuts::CONTROL_HOME);
  g_assert_true(notified == 4);
}

static void test_adw_tab_view_get_page(void) {
  Adw::TabView view;

  Gtk::Widget *child1 = Gtk::make_managed<Gtk::Button>();
  Gtk::Widget *child2 = Gtk::make_managed<Gtk::Button>();
  Gtk::Widget *child3 = Gtk::make_managed<Gtk::Button>();

  Glib::RefPtr<Adw::TabPage> page1 = view.append_pinned(child1);
  Glib::RefPtr<Adw::TabPage> page2 = view.append(child2);
  Glib::RefPtr<Adw::TabPage> page3 = view.append(child3);

  g_assert_true(view.get_nth_page(0) == page1);
  g_assert_true(view.get_nth_page(1) == page2);
  g_assert_true(view.get_nth_page(2) == page3);

  g_assert_true(view.get_page(child1) == page1);
  g_assert_true(view.get_page(child2) == page2);
  g_assert_true(view.get_page(child3) == page3);

  g_assert_true(view.get_page_position(page1) == 0);
  g_assert_true(view.get_page_position(page2) == 1);
  g_assert_true(view.get_page_position(page3) == 2);

  g_assert_true(page1->get_child() == child1);
  g_assert_true(page2->get_child() == child2);
  g_assert_true(page3->get_child() == child3);
}

static void test_adw_tab_view_select(void) {
  Adw::TabView view;

  notified = 0;
  view.property_selected_page().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Adw::TabPage> selected_page =
      view.get_property<Glib::RefPtr<Adw::TabPage>>("selected-page");
  g_assert_true(selected_page == nullptr);

  Glib::RefPtr<Adw::TabPage> page1 =
      view.append(Gtk::make_managed<Gtk::Button>());
  g_assert_true(view.get_selected_page() == page1);
  g_assert_true(page1->get_selected());
  g_assert_true(notified == 1);

  Glib::RefPtr<Adw::TabPage> page2 =
      view.append(Gtk::make_managed<Gtk::Button>());
  g_assert_true(view.get_selected_page() == page1);
  g_assert_true(page1->get_selected());
  g_assert_false(page2->get_selected());
  g_assert_true(notified == 1);

  view.set_selected_page(page2);
  g_assert_true(view.get_selected_page() == page2);
  g_assert_true(notified == 2);

  view.set_property<Glib::RefPtr<Adw::TabPage>>("selected-page", page1);
  g_assert_true(view.get_selected_page() == page1);
  g_assert_true(notified == 3);

  bool ret = view.select_previous_page();
  g_assert_true(view.get_selected_page() == page1);
  g_assert_false(ret);
  g_assert_true(notified == 3);

  ret = view.select_next_page();
  g_assert_true(view.get_selected_page() == page2);
  g_assert_true(ret);
  g_assert_true(notified == 4);

  ret = view.select_next_page();
  g_assert_true(view.get_selected_page() == page2);
  g_assert_false(ret);
  g_assert_true(notified == 4);

  ret = view.select_previous_page();
  g_assert_true(view.get_selected_page() == page1);
  g_assert_true(ret);
  g_assert_true(notified == 5);
}

static void test_adw_tab_view_add_basic(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 6> pages;

  pages[0] = view.append(Gtk::make_managed<Gtk::Button>());
  assert_page_positions(view, pages, 1, 0, {0});

  pages[1] = view.prepend(Gtk::make_managed<Gtk::Button>());
  assert_page_positions(view, pages, 2, 0, {1, 0});

  pages[2] = view.insert(Gtk::make_managed<Gtk::Button>(), 1);
  assert_page_positions(view, pages, 3, 0, {1, 2, 0});

  pages[3] = view.prepend_pinned(Gtk::make_managed<Gtk::Button>());
  assert_page_positions(view, pages, 4, 1, {3, 1, 2, 0});

  pages[4] = view.append_pinned(Gtk::make_managed<Gtk::Button>());
  assert_page_positions(view, pages, 5, 2, {3, 4, 1, 2, 0});

  pages[5] = view.insert_pinned(Gtk::make_managed<Gtk::Button>(), 1);
  assert_page_positions(view, pages, 6, 3, {3, 5, 4, 1, 2, 0});
}

static void test_adw_tab_view_add_auto(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 17> pages;

  add_pages(view, pages, 3, 3);
  assert_page_positions(view, pages, 3, 3, {0, 1, 2});

  /* No parent */

  pages[3] = view.add_page(Gtk::make_managed<Gtk::Button>());
  g_assert_true(pages[3]->get_parent() == nullptr);
  assert_page_positions(view, pages, 4, 3, {0, 1, 2, 3});

  pages[4] = view.add_page(Gtk::make_managed<Gtk::Button>());
  g_assert_true(pages[4]->get_parent() == nullptr);
  assert_page_positions(view, pages, 5, 3, {0, 1, 2, 3, 4});

  pages[5] = view.add_page(Gtk::make_managed<Gtk::Button>());
  g_assert_true(pages[5]->get_parent() == nullptr);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  /* Parent is a regular page */

  pages[6] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[4]);
  g_assert_true(pages[6]->get_parent() == pages[4]);
  assert_page_positions(view, pages, 7, 3, {0, 1, 2, 3, 4, 6, 5});

  pages[7] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[4]);
  g_assert_true(pages[7]->get_parent() == pages[4]);
  assert_page_positions(view, pages, 8, 3, {0, 1, 2, 3, 4, 6, 7, 5});

  pages[8] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[6]);
  g_assert_true(pages[8]->get_parent() == pages[6]);
  assert_page_positions(view, pages, 9, 3, {0, 1, 2, 3, 4, 6, 8, 7, 5});

  pages[9] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[6]);
  g_assert_true(pages[9]->get_parent() == pages[6]);
  assert_page_positions(view, pages, 10, 3, {0, 1, 2, 3, 4, 6, 8, 9, 7, 5});

  pages[10] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[4]);
  g_assert_true(pages[10]->get_parent() == pages[4]);
  assert_page_positions(view, pages, 11, 3, {0, 1, 2, 3, 4, 6, 8, 9, 7, 10, 5});

  /* Parent is a pinned page */

  pages[11] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[1]);
  g_assert_true(pages[11]->get_parent() == pages[1]);
  assert_page_positions(view, pages, 12, 3,
                        {0, 1, 2, 11, 3, 4, 6, 8, 9, 7, 10, 5});

  pages[12] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[11]);
  g_assert_true(pages[12]->get_parent() == pages[11]);
  assert_page_positions(view, pages, 13, 3,
                        {0, 1, 2, 11, 12, 3, 4, 6, 8, 9, 7, 10, 5});

  pages[13] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[1]);
  g_assert_true(pages[13]->get_parent() == pages[1]);
  assert_page_positions(view, pages, 14, 3,
                        {0, 1, 2, 11, 12, 13, 3, 4, 6, 8, 9, 7, 10, 5});

  pages[14] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[0]);
  g_assert_true(pages[14]->get_parent() == pages[0]);
  assert_page_positions(view, pages, 15, 3,
                        {0, 1, 2, 14, 11, 12, 13, 3, 4, 6, 8, 9, 7, 10, 5});

  pages[15] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[1]);
  g_assert_true(pages[15]->get_parent() == pages[1]);
  assert_page_positions(view, pages, 16, 3,
                        {0, 1, 2, 15, 14, 11, 12, 13, 3, 4, 6, 8, 9, 7, 10, 5});

  /* Parent is the last page */
  pages[16] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[5]);
  g_assert_true(pages[16]->get_parent() == pages[5]);
  assert_page_positions(
      view, pages, 17, 3,
      {0, 1, 2, 15, 14, 11, 12, 13, 3, 4, 6, 8, 9, 7, 10, 5, 16});
}

static void test_adw_tab_view_reorder(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 6> pages;

  add_pages(view, pages, 6, 3);

  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  bool ret = view.reorder_page(pages[1], 1);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  ret = view.reorder_page(pages[1], 0);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {1, 0, 2, 3, 4, 5});

  ret = view.reorder_page(pages[1], 1);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  ret = view.reorder_page(pages[5], 5);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  ret = view.reorder_page(pages[5], 4);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 5, 4});

  ret = view.reorder_page(pages[5], 5);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});
}

static void test_adw_tab_view_reorder_first_last(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 6> pages;

  add_pages(view, pages, 6, 3);

  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  bool ret = view.reorder_first(pages[0]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  ret = view.reorder_last(pages[0]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {1, 2, 0, 3, 4, 5});

  ret = view.reorder_last(pages[0]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {1, 2, 0, 3, 4, 5});

  ret = view.reorder_first(pages[0]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  ret = view.reorder_first(pages[3]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  ret = view.reorder_last(pages[3]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 4, 5, 3});

  ret = view.reorder_last(pages[3]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 4, 5, 3});

  ret = view.reorder_first(pages[3]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});
}

static void test_adw_tab_view_reorder_forward_backward(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 6> pages;

  add_pages(view, pages, 6, 3);

  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  bool ret = view.reorder_backward(pages[0]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  ret = view.reorder_forward(pages[0]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {1, 0, 2, 3, 4, 5});

  ret = view.reorder_forward(pages[2]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {1, 0, 2, 3, 4, 5});

  ret = view.reorder_backward(pages[2]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {1, 2, 0, 3, 4, 5});

  ret = view.reorder_backward(pages[3]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {1, 2, 0, 3, 4, 5});

  ret = view.reorder_forward(pages[3]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {1, 2, 0, 4, 3, 5});

  ret = view.reorder_forward(pages[5]);
  g_assert_false(ret);
  assert_page_positions(view, pages, 6, 3, {1, 2, 0, 4, 3, 5});

  ret = view.reorder_backward(pages[5]);
  g_assert_true(ret);
  assert_page_positions(view, pages, 6, 3, {1, 2, 0, 4, 5, 3});
}

static void test_adw_tab_view_pin(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 4> pages;

  /* Test specifically pinning with only 1 page */
  pages[0] = view.append(Gtk::make_managed<Gtk::Button>());
  g_assert_false(pages[0]->get_pinned());
  assert_page_positions(view, pages, 1, 0, {0});

  view.set_page_pinned(pages[0], true);
  g_assert_true(pages[0]->get_pinned());
  assert_page_positions(view, pages, 1, 1, {0});

  view.set_page_pinned(pages[0], false);
  g_assert_false(pages[0]->get_pinned());
  assert_page_positions(view, pages, 1, 0, {0});

  pages[1] = view.append(Gtk::make_managed<Gtk::Button>());
  pages[2] = view.append(Gtk::make_managed<Gtk::Button>());
  pages[3] = view.append(Gtk::make_managed<Gtk::Button>());
  assert_page_positions(view, pages, 4, 0, {0, 1, 2, 3});

  view.set_page_pinned(pages[2], true);
  assert_page_positions(view, pages, 4, 1, {2, 0, 1, 3});

  view.set_page_pinned(pages[1], true);
  assert_page_positions(view, pages, 4, 2, {2, 1, 0, 3});

  view.set_page_pinned(pages[0], true);
  assert_page_positions(view, pages, 4, 3, {2, 1, 0, 3});

  view.set_page_pinned(pages[1], false);
  assert_page_positions(view, pages, 4, 2, {2, 0, 1, 3});
}

static void test_adw_tab_view_close(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 3> pages;

  add_pages(view, pages, 3, 0);

  view.set_selected_page(pages[1]);

  assert_page_positions(view, pages, 3, 0, {0, 1, 2});

  view.close_page(pages[1]);
  assert_page_positions(view, pages, 2, 0, {0, 2});
  g_assert_true(view.get_selected_page() == pages[2]);

  view.close_page(pages[2]);
  assert_page_positions(view, pages, 1, 0, {0});
  g_assert_true(view.get_selected_page() == pages[0]);

  view.close_page(pages[0]);
  assert_page_positions(view, pages, 0, 0, {});
  g_assert_true(view.get_selected_page() == nullptr);
}

static void test_adw_tab_view_close_other(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 6> pages;

  add_pages(view, pages, 6, 3);

  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 3, 4, 5});

  view.close_other_pages(pages[4]);
  assert_page_positions(view, pages, 4, 3, {0, 1, 2, 4});

  view.close_other_pages(pages[2]);
  assert_page_positions(view, pages, 3, 3, {0, 1, 2});
}

static void test_adw_tab_view_close_before_after(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 10> pages;

  add_pages(view, pages, 10, 3);
  assert_page_positions(view, pages, 10, 3, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  view.close_pages_before(pages[3]);
  assert_page_positions(view, pages, 10, 3, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  view.close_pages_before(pages[5]);
  assert_page_positions(view, pages, 8, 3, {0, 1, 2, 5, 6, 7, 8, 9});

  view.close_pages_after(pages[7]);
  assert_page_positions(view, pages, 6, 3, {0, 1, 2, 5, 6, 7});

  view.close_pages_after(pages[0]);
  assert_page_positions(view, pages, 3, 3, {0, 1, 2});
}

static bool close_page_position_cb(const Glib::RefPtr<Adw::TabPage> &page,
                                   Adw::TabView &view) {
  int position = view.get_page_position(page);

  view.close_page_finish(page, (position % 2) > 0);

  return GDK_EVENT_STOP;
}

static void test_adw_tab_view_close_signal(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 10> pages;

  /* Allow closing pages with odd positions, including pinned */
  sigc::connection conn = view.signal_close_page().connect(
      sigc::bind(sigc::ptr_fun(close_page_position_cb), std::ref(view)),
      /* after */ false);

  add_pages(view, pages, 10, 3);

  assert_page_positions(view, pages, 10, 3, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  view.close_other_pages(pages[5]);
  assert_page_positions(view, pages, 6, 2, {0, 2, 4, 5, 6, 8});

  conn.disconnect();

  /* Defer closing */
  conn = view.signal_close_page().connect(sigc::ptr_fun(close_noop),
                                          /* after */ false);

  view.close_page(pages[0]);
  assert_page_positions(view, pages, 6, 2, {0, 2, 4, 5, 6, 8});

  view.close_page_finish(pages[0], false);
  assert_page_positions(view, pages, 6, 2, {0, 2, 4, 5, 6, 8});

  view.close_page(pages[0]);
  assert_page_positions(view, pages, 6, 2, {0, 2, 4, 5, 6, 8});

  view.close_page_finish(pages[0], true);
  assert_page_positions(view, pages, 5, 1, {2, 4, 5, 6, 8});
}

static void test_adw_tab_view_close_select(void) {
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 14> pages;

  add_pages(view, pages, 9, 3);
  pages[9] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[4]);
  pages[10] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[4]);
  pages[11] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[9]);
  pages[12] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[1]);
  pages[13] = view.add_page(Gtk::make_managed<Gtk::Button>(), pages[1]);

  assert_page_positions(view, pages, 14, 3,
                        {0, 1, 2, 12, 13, 3, 4, 9, 11, 10, 5, 6, 7, 8});

  /* Nothing happens when closing unselected pages */

  view.set_selected_page(pages[0]);

  view.close_page(pages[8]);
  g_assert_true(view.get_selected_page() == pages[0]);

  /* No parent */
  assert_page_positions(view, pages, 13, 3,
                        {0, 1, 2, 12, 13, 3, 4, 9, 11, 10, 5, 6, 7});

  view.set_selected_page(pages[6]);

  view.close_page(pages[6]);
  g_assert_true(view.get_selected_page() == pages[7]);

  view.close_page(pages[7]);
  g_assert_true(view.get_selected_page() == pages[5]);

  /* Regular parent */

  assert_page_positions(view, pages, 11, 3,
                        {0, 1, 2, 12, 13, 3, 4, 9, 11, 10, 5});

  view.set_selected_page(pages[10]);

  view.close_page(pages[10]);
  g_assert_true(view.get_selected_page() == pages[11]);

  view.close_page(pages[11]);
  g_assert_true(view.get_selected_page() == pages[9]);

  view.close_page(pages[9]);
  g_assert_true(view.get_selected_page() == pages[4]);

  view.close_page(pages[4]);
  g_assert_true(view.get_selected_page() == pages[5]);

  /* Pinned parent */

  assert_page_positions(view, pages, 7, 3, {0, 1, 2, 12, 13, 3, 5});

  view.set_selected_page(pages[13]);

  view.close_page(pages[13]);
  g_assert_true(view.get_selected_page() == pages[12]);

  view.close_page(pages[12]);
  g_assert_true(view.get_selected_page() == pages[1]);
}

static void test_adw_tab_view_transfer(void) {
  Adw::TabView view1;
  Adw::TabView view2;
  std::array<Glib::RefPtr<Adw::TabPage>, 4> pages1;
  std::array<Glib::RefPtr<Adw::TabPage>, 4> pages2;

  add_pages(view1, pages1, 4, 2);
  assert_page_positions(view1, pages1, 4, 2, {0, 1, 2, 3});

  add_pages(view2, pages2, 4, 2);
  assert_page_positions(view2, pages2, 4, 2, {0, 1, 2, 3});

  view1.transfer_page(pages1[1], &view2, 1);
  assert_page_positions(view1, pages1, 3, 1, {0, 2, 3});
  assert_page_positions(view2, pages2, 5, 3, {0, -1, 1, 2, 3});
  g_assert_true(view2.get_nth_page(1) == pages1[1]);

  view2.transfer_page(pages2[3], &view1, 2);
  assert_page_positions(view1, pages1, 4, 1, {0, 2, -1, 3});
  assert_page_positions(view2, pages2, 4, 3, {0, -1, 1, 2});
  g_assert_true(view1.get_nth_page(2) == pages2[3]);
}

static void
test_adw_tab_view_pages (void)
{
  Adw::TabView view;
  std::array<Glib::RefPtr<Adw::TabPage>, 2> pages;

  Glib::RefPtr<Gtk::SelectionModel> pages_model = view.get_pages();
  g_assert_true(pages_model != nullptr);

  Glib::RefPtr<Gio::ListModel> list_model =
      std::dynamic_pointer_cast<Gio::ListModel>(pages_model);
  g_assert_true(list_model != nullptr);

  sigc::connection conn1 = list_model->signal_items_changed().connect(
      sigc::bind(sigc::ptr_fun(check_selection_non_null<guint, guint, guint>), std::ref(view)));
  sigc::connection conn2 = pages_model->signal_selection_changed().connect(
      sigc::bind(sigc::ptr_fun(check_selection_non_null<guint, guint>), std::ref(view)));

  pages[0] = view.add_page(Gtk::make_managed<Gtk::Button>());
  pages[1] = view.add_page(Gtk::make_managed<Gtk::Button>());

  view.close_page(pages[0]);

  conn1.disconnect();
  conn2.disconnect();

  list_model->signal_items_changed().connect(
       sigc::bind(sigc::ptr_fun(check_selection_null<guint, guint, guint>), std::ref(view)));
  pages_model->signal_selection_changed().connect(
      sigc::bind(sigc::ptr_fun(check_selection_null<guint, guint>), std::ref(view)));

  view.close_page(pages[1]);
}

static void test_adw_tab_page_title(void) {
  Adw::TabView view;

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_title().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring title = page->get_property<Glib::ustring>("title");
  g_assert_true(title == "");
  g_assert_true(notified == 0);

  page->set_title("Some title");
  g_assert_true(page->get_title() == "Some title");
  g_assert_true(notified == 1);

  page->set_property<Glib::ustring>("title", "Some other title");
  g_assert_true(page->get_title() == "Some other title");
  g_assert_true(notified == 2);
}

static void test_adw_tab_page_tooltip(void) {
  Adw::TabView view;

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_tooltip().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring tooltip = page->get_property<Glib::ustring>("tooltip");
  g_assert_true(tooltip == "");
  g_assert_true(notified == 0);

  page->set_tooltip("Some tooltip");
  g_assert_true(page->get_tooltip() == "Some tooltip");
  g_assert_true(notified == 1);

  page->set_property<Glib::ustring>("tooltip", "Some other tooltip");
  g_assert_true(page->get_tooltip() == "Some other tooltip");
  g_assert_true(notified == 2);
}

static void test_adw_tab_page_icon(void) {
  Adw::TabView view;
  Glib::RefPtr<Gio::Icon> icon1 =
      Gio::ThemedIcon::create("go-previous-symbolic");
  Glib::RefPtr<Gio::Icon> icon2 = Gio::ThemedIcon::create("go-next-symbolic");

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_icon().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gio::Icon> icon =
      page->get_property<Glib::RefPtr<Gio::Icon>>("icon");
  g_assert_true(icon == nullptr);
  g_assert_true(notified == 0);

  page->set_icon(icon1);
  g_assert_true(page->get_icon() == icon1);
  g_assert_true(notified == 1);

  page->set_property<Glib::RefPtr<Gio::Icon>>("icon", icon2);
  g_assert_true(page->get_icon() == icon2);
  g_assert_true(notified == 2);
}

static void test_adw_tab_page_loading(void) {
  Adw::TabView view;

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_loading().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool loading = page->get_property<bool>("loading");
  g_assert_false(loading);
  g_assert_true(notified == 0);

  page->set_loading(true);
  loading = page->get_property<bool>("loading");
  g_assert_true(loading);
  g_assert_true(notified == 1);

  page->set_property<bool>("loading", false);
  g_assert_false(page->get_loading());
  g_assert_true(notified == 2);
}

static void test_adw_tab_page_indicator_icon(void) {
  Adw::TabView view;
  Glib::RefPtr<Gio::Icon> icon1 =
      Gio::ThemedIcon::create("go-previous-symbolic");
  Glib::RefPtr<Gio::Icon> icon2 = Gio::ThemedIcon::create("go-next-symbolic");

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_indicator_icon().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gio::Icon> icon =
      page->get_property<Glib::RefPtr<Gio::Icon>>("indicator-icon");
  g_assert_true(icon == nullptr);
  g_assert_true(notified == 0);

  page->set_indicator_icon(icon1);
  g_assert_true(page->get_indicator_icon() == icon1);
  g_assert_true(notified == 1);

  page->set_property<Glib::RefPtr<Gio::Icon>>("indicator-icon", icon2);
  g_assert_true(page->get_indicator_icon() == icon2);
  g_assert_true(notified == 2);
}

static void test_adw_tab_page_indicator_tooltip(void) {
  Adw::TabView view;

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_indicator_tooltip().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring tooltip =
      page->get_property<Glib::ustring>("indicator-tooltip");
  g_assert_true(tooltip == "");
  g_assert_true(notified == 0);

  page->set_indicator_tooltip("Some tooltip");
  g_assert_true(page->get_indicator_tooltip() == "Some tooltip");
  g_assert_true(notified == 1);

  page->set_property<Glib::ustring>("indicator-tooltip", "Some other tooltip");
  g_assert_true(page->get_indicator_tooltip() == "Some other tooltip");
  g_assert_true(notified == 2);
}

static void test_adw_tab_page_indicator_activatable(void) {
  Adw::TabView view;

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_indicator_activatable().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool indicator_activatable =
      page->get_property<bool>("indicator-activatable");
  g_assert_false(indicator_activatable);
  g_assert_true(notified == 0);

  page->set_indicator_activatable(true);
  indicator_activatable = page->get_property<bool>("indicator-activatable");
  g_assert_true(indicator_activatable);
  g_assert_true(notified == 1);

  page->set_property<bool>("indicator-activatable", false);
  g_assert_false(page->get_indicator_activatable());
  g_assert_true(notified == 2);
}

static void test_adw_tab_page_needs_attention(void) {
  Adw::TabView view;

  Glib::RefPtr<Adw::TabPage> page =
      view.append(Gtk::make_managed<Gtk::Button>());

  notified = 0;
  page->property_needs_attention().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool needs_attention = page->get_property<bool>("needs-attention");
  g_assert_false(needs_attention);
  g_assert_true(notified == 0);

  page->set_needs_attention(true);
  needs_attention = page->get_property<bool>("needs-attention");
  g_assert_true(needs_attention);
  g_assert_true(notified == 1);

  page->set_property<bool>("needs-attention", false);
  g_assert_false(page->get_needs_attention());
  g_assert_true(notified == 2);
}

static void test_adw_tab_view_pages_to_list_view_setup(
    const Glib::RefPtr<Gtk::ListItem> &list_item) {
  list_item->set_child(*Gtk::make_managed<Gtk::Label>(""));
}

static void test_adw_tab_view_pages_to_list_view_bind(
    const Glib::RefPtr<Gtk::ListItem> &list_item) {
  Glib::RefPtr<Gtk::Label> item =
      std::dynamic_pointer_cast<Gtk::Label>(list_item->get_item());
  g_assert_true(item != nullptr);

  Gtk::Label *row = dynamic_cast<Gtk::Label *>(list_item->get_child());
  g_assert_true(row != nullptr);

  Glib::RefPtr<Glib::Binding> binding = Glib::Binding::bind_property(
      item->property_label(), row->property_label(),
      Glib::Binding::Flags::SYNC_CREATE);

  binding->reference();
  list_item->set_data("BINDING", binding.get());
}

static void test_adw_tab_view_pages_to_list_view_unbind(
    const Glib::RefPtr<Gtk::ListItem> &list_item) {
  Glib::Binding *binding =
      reinterpret_cast<Glib::Binding *>(list_item->get_data("BINDING"));

  binding->unbind();
  binding->unreference();
}

static void test_adw_tab_view_pages_to_list_view(void) {
  Adw::TabView view;
  Gtk::ListView list_view;

  Glib::RefPtr<Gtk::SelectionModel> pages = view.get_pages();
  g_assert_true(pages != nullptr);

  Glib::RefPtr<Gtk::SignalListItemFactory> factory =
      Gtk::SignalListItemFactory::create();

  factory->signal_setup().connect(
      sigc::ptr_fun(test_adw_tab_view_pages_to_list_view_setup));
  factory->signal_bind().connect(
      sigc::ptr_fun(test_adw_tab_view_pages_to_list_view_bind));
  factory->signal_unbind().connect(
      sigc::ptr_fun(test_adw_tab_view_pages_to_list_view_unbind));

  list_view.set_factory(factory);
  list_view.set_model(pages);

  view.append(Gtk::make_managed<Gtk::Label>("test label"));
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/TabView/n_pages", test_adw_tab_view_n_pages);
  g_test_add_func("/Adwaita/TabView/n_pinned_pages",
                  test_adw_tab_view_n_pinned_pages);
  g_test_add_func("/Adwaita/TabView/default_icon",
                  test_adw_tab_view_default_icon);
  g_test_add_func("/Adwaita/TabView/menu_model", test_adw_tab_view_menu_model);
  g_test_add_func("/Adwaita/TabView/shortcuts", test_adw_tab_view_shortcuts);
  g_test_add_func("/Adwaita/TabView/get_page", test_adw_tab_view_get_page);
  g_test_add_func("/Adwaita/TabView/select", test_adw_tab_view_select);
  g_test_add_func("/Adwaita/TabView/add_basic", test_adw_tab_view_add_basic);
  g_test_add_func("/Adwaita/TabView/add_auto", test_adw_tab_view_add_auto);
  g_test_add_func("/Adwaita/TabView/reorder", test_adw_tab_view_reorder);
  g_test_add_func("/Adwaita/TabView/reorder_first_last",
                  test_adw_tab_view_reorder_first_last);
  g_test_add_func("/Adwaita/TabView/reorder_forward_backward",
                  test_adw_tab_view_reorder_forward_backward);
  g_test_add_func("/Adwaita/TabView/pin", test_adw_tab_view_pin);
  g_test_add_func("/Adwaita/TabView/close", test_adw_tab_view_close);
  g_test_add_func("/Adwaita/TabView/close_other",
                  test_adw_tab_view_close_other);
  g_test_add_func("/Adwaita/TabView/close_before_after",
                  test_adw_tab_view_close_before_after);
  g_test_add_func("/Adwaita/TabView/close_signal",
                  test_adw_tab_view_close_signal);
  g_test_add_func("/Adwaita/TabView/close_select",
                  test_adw_tab_view_close_select);
  g_test_add_func("/Adwaita/TabView/transfer", test_adw_tab_view_transfer);
  g_test_add_func("/Adwaita/TabView/pages", test_adw_tab_view_pages);
  g_test_add_func("/Adwaita/TabView/pages_to_list_view",
                  test_adw_tab_view_pages_to_list_view);
  g_test_add_func("/Adwaita/TabPage/title", test_adw_tab_page_title);
  g_test_add_func("/Adwaita/TabPage/tooltip", test_adw_tab_page_tooltip);
  g_test_add_func("/Adwaita/TabPage/icon", test_adw_tab_page_icon);
  g_test_add_func("/Adwaita/TabPage/loading", test_adw_tab_page_loading);
  g_test_add_func("/Adwaita/TabPage/indicator_icon",
                  test_adw_tab_page_indicator_icon);
  g_test_add_func("/Adwaita/TabPage/indicator_tooltip",
                  test_adw_tab_page_indicator_tooltip);
  g_test_add_func("/Adwaita/TabPage/indicator_activatable",
                  test_adw_tab_page_indicator_activatable);
  g_test_add_func("/Adwaita/TabPage/needs_attention",
                  test_adw_tab_page_needs_attention);

  return g_test_run();
}
