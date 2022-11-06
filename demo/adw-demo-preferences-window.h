#pragma once

#include <libadwaitamm.h>
#include "templatebuilder.h"

namespace Adw {

class DemoPreferencesWindow
    : public Gtk::TemplateBuilder<DemoPreferencesWindow>,
      public Adw::PreferencesWindow {
public:
  DemoPreferencesWindow();
  // DemoPreferencesWindow(GTypeInstance *) { g_assert_not_reached(); }
  ~DemoPreferencesWindow();

private:
  Gtk::Widget *subpage1;
  Gtk::Widget *subpage2;

  void subpage1_activated();
  void subpage2_activated();
  void return_to_preferences();
  void toast_show();
};
} // namespace Adw
