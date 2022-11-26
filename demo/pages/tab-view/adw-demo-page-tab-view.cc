#include "adw-demo-page-tab-view.h"

#include <glib/gi18n.h>

#include "adw-tab-view-demo-window.h"

namespace Adw {

const char DemoPageTabView::class_name[] = "AdwDemoPageTabView";

void DemoPageTabView::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/tab-view/adw-demo-page-tab-view.ui");

  s.install_action("demo.run",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageTabView::demo_run_cb>());
}

void DemoPageTabView::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

void DemoPageTabView::demo_run_cb() {
  Gtk::Window *root = dynamic_cast<Gtk::Window*>(get_root());
  TabViewDemoWindow *window = TabViewDemoWindow::create();
  window->prepopulate();
  window->set_transient_for(*root);
  window->present();
}

} // namespace Adw
