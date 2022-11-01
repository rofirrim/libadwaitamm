#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define ADW_TYPE_DEMO_PAGE_ABOUT (adw_demo_page_about_get_type())

G_DECLARE_FINAL_TYPE (AdwDemoPageAbout, adw_demo_page_about, ADW, DEMO_PAGE_ABOUT, AdwBin)

G_END_DECLS

#include <libadwaitamm.h>
#include "templatebuilder.h"

namespace Adw
{

class DemoPageAbout : public Gtk::TemplateBuilder<DemoPageAbout>,
                      public Adw::Bin {
public:
  DemoPageAbout();
  ~DemoPageAbout();
};
}
