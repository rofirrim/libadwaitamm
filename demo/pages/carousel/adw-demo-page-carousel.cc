#include "adw-demo-page-carousel.h"

#include <glib/gi18n.h>

// FIXME - How to better wrap those?

static char *
get_orientation_name (AdwEnumListItem *item,
                      gpointer         user_data)
{
  switch (adw_enum_list_item_get_value (item)) {
  case GTK_ORIENTATION_HORIZONTAL:
    return g_strdup (_("Horizontal"));
  case GTK_ORIENTATION_VERTICAL:
    return g_strdup (_("Vertical"));
  default:
    return NULL;
  }
}

static char *
get_indicators_name (GtkStringObject *value)
{
  const char *style;

  g_assert (GTK_IS_STRING_OBJECT (value));

  style = gtk_string_object_get_string (value);

  if (!g_strcmp0 (style, "dots"))
    return g_strdup (_("Dots"));

  if (!g_strcmp0 (style, "lines"))
    return g_strdup (_("Lines"));

  return NULL;
}

namespace Adw {

const char DemoPageCarousel::class_name[] = "AdwDemoPageCarousel";

void DemoPageCarousel::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/carousel/adw-demo-page-carousel.ui");

  s.bind_widget("box");
  s.bind_widget("carousel");
  s.bind_widget("indicators_stack");
  s.bind_widget("orientation_row");
  s.bind_widget("indicators_row");

  s.bind_callback("get_orientation_name", GCallback(get_orientation_name));
  s.bind_callback(
      "notify_orientation_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoPageCarousel::notify_orientation_cb>());
  s.bind_callback("get_indicators_name", GCallback(get_indicators_name));
  s.bind_callback(
      "notify_indicators_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoPageCarousel::notify_indicators_cb>());

  s.install_action(
      "carousel.return",
      Gtk::ptr_fun_to_mem_fun<&DemoPageCarousel::carousel_return_cb>());
}

void DemoPageCarousel::init_widget(Gtk::TemplateWidgetInit &i) {
    i.init_template();

    i.bind_widget(box, "box");
    i.bind_widget(carousel, "carousel");
    i.bind_widget(indicators_stack, "indicators_stack");
    i.bind_widget(orientation_row, "orientation_row");
    i.bind_widget(indicators_row, "indicators_row");
}

void DemoPageCarousel::notify_orientation_cb() {
  Gtk::Orientation orientation =
      static_cast<Gtk::Orientation>(orientation_row->get_selected());

  box->set_orientation(static_cast<Gtk::Orientation>(1 - (int)orientation));
  carousel->set_orientation(static_cast<Gtk::Orientation>(orientation));
}

void DemoPageCarousel::notify_indicators_cb() {
  auto obj = std::dynamic_pointer_cast<Gtk::StringObject>(indicators_row->get_selected_item());

  indicators_stack->set_visible_child(obj->get_string());
}

void DemoPageCarousel::carousel_return_cb() {
  carousel->scroll_to(carousel->get_nth_page(0), true);
}

} // namespace Adw
