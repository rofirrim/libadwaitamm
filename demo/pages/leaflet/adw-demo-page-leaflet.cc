#include "adw-demo-page-leaflet.h"

#include <glib/gi18n.h>

static char *get_transition_name(AdwEnumListItem *item,
                                 gpointer /*user_data*/) {
  switch (adw_enum_list_item_get_value(item)) {
  case ADW_LEAFLET_TRANSITION_TYPE_OVER:
    return g_strdup(_("Over"));
  case ADW_LEAFLET_TRANSITION_TYPE_UNDER:
    return g_strdup(_("Under"));
  case ADW_LEAFLET_TRANSITION_TYPE_SLIDE:
    return g_strdup(_("Slide"));
  default:
    return NULL;
  }
}

namespace Adw {

const char DemoPageLeaflet::class_name[] = "AdwDemoPageLeaflet";
unsigned int DemoPageLeaflet::signal_next_page;

void DemoPageLeaflet::setup_template(Gtk::TemplateWidgetSetup &s) {
  signal_next_page =
      g_signal_new("next-page", s.get_class_type(), G_SIGNAL_RUN_FIRST, 0, NULL,
                   NULL, NULL, G_TYPE_NONE, 0);

  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/leaflet/adw-demo-page-leaflet.ui");
  s.bind_callback("get_transition_name", GCallback(get_transition_name));
  s.bind_callback(
      "next_row_activated_cb",
      Gtk::ptr_fun_to_mem_fun<&DemoPageLeaflet::next_row_activated_cb>());
}

void DemoPageLeaflet::init_widget(Gtk::TemplateWidgetInit &i) {
  p_transition_type =
      std::make_unique<Glib::Property<Adw::LeafletTransitionType>>(
          *this, "transition-type", Adw::LeafletTransitionType::OVER);

  i.init_template();
}

void DemoPageLeaflet ::next_row_activated_cb() {
  g_signal_emit(gobj(), signal_next_page, 0);
}

} // namespace Adw
