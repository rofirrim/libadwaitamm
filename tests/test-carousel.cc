/*
 * Copyright (C) 2019 Alexander Mikhaylenko <exalm7659@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void allocate_carousel(Adw::Carousel &carousel) {
  int width, height;

  int _minimum, _minimum_baseline, _natural_baseline;
  carousel.measure(Gtk::Orientation::HORIZONTAL, -1, _minimum, width,
                   _minimum_baseline, _natural_baseline);

  carousel.measure(Gtk::Orientation::VERTICAL, -1, _minimum, height,
                   _minimum_baseline, _natural_baseline);

  // FIXME: This is not wrapped?
  gtk_widget_allocate(GTK_WIDGET(carousel.gobj()), width, height, 0, NULL);
}

static void test_adw_carousel_add_remove(void) {
  Adw::Carousel carousel;

  Gtk::Label child1;
  Gtk::Label child2;
  Gtk::Label child3;

  notified = 0;
  carousel.property_n_pages().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(carousel.get_n_pages() == 0);

  carousel.append(&child1);
  g_assert_true(carousel.get_n_pages() == 1);
  g_assert_true(notified == 1);

  carousel.prepend(&child2);
  allocate_carousel(carousel);

  g_assert_true(carousel.get_n_pages() == 2);
  g_assert_true(carousel.get_nth_page(0)->gobj() == (GtkWidget*)child2.gobj());
  g_assert_true(carousel.get_nth_page(1)->gobj() == (GtkWidget*)child1.gobj());
  g_assert_true(carousel.get_position() == 1);
  g_assert_true(notified == 2);

  carousel.insert(&child3, 1);
  allocate_carousel(carousel);
  g_assert_true(carousel.get_n_pages() == 3);
  g_assert_true(carousel.get_nth_page(0)->gobj() == (GtkWidget*)child2.gobj());
  g_assert_true(carousel.get_nth_page(1)->gobj() == (GtkWidget*)child3.gobj());
  g_assert_true(carousel.get_nth_page(2)->gobj() == (GtkWidget*)child1.gobj());
  g_assert_true(carousel.get_position() == 2);
  g_assert_true(notified == 3);

  carousel.scroll_to(&child3, false);
  carousel.remove(&child2);
  allocate_carousel(carousel);
  g_assert_true(carousel.get_n_pages() == 2);
  g_assert_true(carousel.get_position() == 0);
  g_assert_true(notified == 4);

  carousel.remove(&child1);
  g_assert_true(carousel.get_n_pages() == 1);
  g_assert_true(notified == 5);
}

static void assert_carousel_positions(Adw::Carousel &carousel, Gtk::Widget &child1,
                                      Gtk::Widget &child2, Gtk::Widget &child3,
                                      Gtk::Widget &child4, double position) {
  allocate_carousel(carousel);
  g_assert_true(carousel.get_nth_page(0)->gobj() == child1.gobj());
  g_assert_true(carousel.get_nth_page(1)->gobj() == child2.gobj());
  g_assert_true(carousel.get_nth_page(2)->gobj() == child3.gobj());
  g_assert_true(carousel.get_nth_page(3)->gobj() == child4.gobj());
  g_assert_true(carousel.get_position() == position);
}

static void test_adw_carousel_reorder(void) {
  Adw::Carousel carousel;
  Gtk::Label child1;
  Gtk::Label child2;
  Gtk::Label child3;
  Gtk::Label child4;

  carousel.append(&child1);
  carousel.append(&child2);
  carousel.append(&child3);
  carousel.append(&child4);
  allocate_carousel(carousel);

  g_assert_true(carousel.get_n_pages() == 4);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);

  /* No-op */
  carousel.reorder(&child1, 0);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);
  carousel.reorder(&child2, 1);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);
  carousel.reorder(&child3, 2);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);
  carousel.reorder(&child4, 3);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);

  carousel.reorder(&child4, 4);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);
  carousel.reorder(&child4, -1);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);

  carousel.scroll_to(&child1, false);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);
  carousel.reorder(&child2, 2);
  assert_carousel_positions(carousel, child1, child3, child2, child4, 0);
  carousel.reorder(&child2, 1);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 0);

  carousel.scroll_to(&child2, false);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 1);
  carousel.reorder(&child2, 2);
  assert_carousel_positions(carousel, child1, child3, child2, child4, 2);
  carousel.reorder(&child2, 1);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 1);

  carousel.scroll_to(&child3, false);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 2);
  carousel.reorder(&child2, 2);
  assert_carousel_positions(carousel, child1, child3, child2, child4, 1);
  carousel.reorder(&child2, 1);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 2);

  carousel.scroll_to(&child4, false);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 3);
  carousel.reorder(&child2, 2);
  assert_carousel_positions(carousel, child1, child3, child2, child4, 3);
  carousel.reorder(&child2, 1);
  assert_carousel_positions(carousel, child1, child2, child3, child4, 3);
}

static void test_adw_carousel_interactive(void) {
  Adw::Carousel carousel;

  notified = 0;
  carousel.property_interactive().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  /* Accessors */
  g_assert_true(carousel.get_interactive());
  carousel.set_interactive(false);
  g_assert_true(!carousel.get_interactive());
  g_assert_true(notified == 1);

  /* Property */
  carousel.set_property<bool>("interactive", true);
  bool interactive = carousel.get_property<bool>("interactive");
  g_assert_true(interactive);
  g_assert_true(notified == 2);

  /* Setting the same value should not notify */
  carousel.set_interactive(true);
  g_assert_true(notified == 2);
}

static void test_adw_carousel_spacing(void) {
  Adw::Carousel carousel;

  notified = 0;
  carousel.property_spacing().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  /* Accessors */
  g_assert_true(carousel.get_spacing() == 0);
  carousel.set_spacing(12);
  g_assert_true(carousel.get_spacing() == 12);
  g_assert_true(notified == 1);

  /* Property */
  carousel.set_property<guint>("spacing", 6);
  guint spacing = carousel.get_property<guint>("spacing");
  g_assert_true(spacing == 6);
  g_assert_true(notified == 2);

  /* Setting the same value should not notify */
  carousel.set_spacing(6);
  g_assert_true(notified == 2);
}

static void test_adw_carousel_allow_mouse_drag(void) {
  Adw::Carousel carousel;

  notified = 0;
  carousel.property_allow_mouse_drag().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  /* Accessors */
  g_assert_true(carousel.get_allow_mouse_drag());
  carousel.set_allow_mouse_drag(false);
  g_assert_false(carousel.get_allow_mouse_drag());
  g_assert_true(notified == 1);

  /* Property */
  carousel.set_property<bool>("allow-mouse-drag", true);
  bool allow_mouse_drag = carousel.get_property<bool>("allow-mouse-drag");
  g_assert_true(allow_mouse_drag);
  g_assert_true(notified == 2);

  /* Setting the same value should not notify */
  carousel.set_allow_mouse_drag(true);
  g_assert_true(notified == 2);
}

static void test_adw_carousel_allow_long_swipes(void) {
  Adw::Carousel carousel;

  notified = 0;
  carousel.property_allow_long_swipes().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  /* Accessors */
  g_assert_false(carousel.get_allow_long_swipes());
  carousel.set_allow_long_swipes(true);
  g_assert_true(carousel.get_allow_long_swipes());
  g_assert_true(notified == 1);

  /* Property */
  carousel.set_property<bool>("allow-long-swipes", false);
  bool allow_long_swipes = carousel.get_property<bool>("allow-long-swipes");
  g_assert_false(allow_long_swipes);
  g_assert_true(notified == 2);

  /* Setting the same value should not notify */
  carousel.set_allow_long_swipes(false);
  g_assert_true(notified == 2);
}

static void test_adw_carousel_reveal_duration(void) {

  Adw::Carousel carousel;

  notified = 0;
  carousel.property_reveal_duration().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  /* Accessors */
  g_assert_true(carousel.get_reveal_duration() == 0);
  carousel.set_reveal_duration(200);
  g_assert_true(carousel.get_reveal_duration() == 200);
  g_assert_true(notified == 1);

  /* Property */
  carousel.set_property<guint>("reveal-duration", 500);
  guint duration =
      carousel.get_property<guint>("reveal-duration");
  g_assert_true(duration == 500);
  g_assert_true(notified == 2);

  /* Setting the same value should not notify */
  carousel.set_reveal_duration(500);
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Carousel/add_remove", test_adw_carousel_add_remove);
  g_test_add_func("/Adwaita/Carousel/reorder", test_adw_carousel_reorder);
  g_test_add_func("/Adwaita/Carousel/interactive",
                  test_adw_carousel_interactive);
  g_test_add_func("/Adwaita/Carousel/spacing", test_adw_carousel_spacing);
  g_test_add_func("/Adwaita/Carousel/allow_mouse_drag",
                  test_adw_carousel_allow_mouse_drag);
  g_test_add_func("/Adwaita/Carousel/allow_long_swipes",
                  test_adw_carousel_allow_long_swipes);
  g_test_add_func("/Adwaita/Carousel/reveal_duration",
                  test_adw_carousel_reveal_duration);
  return g_test_run();
}
