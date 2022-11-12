#include "adw-demo-page-about.h"
#include <libadwaitamm/private/bin_p.h>

#include <glib/gi18n.h>

namespace Adw {

/////////////////////////
// DemoPageAbout_Class //
/////////////////////////

const Glib::Class &DemoPageAbout_Class::init() {
  if (!gtype_) {
    class_init_func_ = class_init_function;
    register_derived_type(adw_bin_get_type(), "AdwDemoPageAbout",
                          &DemoPageAbout::instance_init_function);
    Glib::init();
    Glib::wrap_register(gtype_, &DemoPageAbout_Class::wrap_new);
  }
  return *this;
}

void DemoPageAbout_Class::class_init_function(void *g_class, void *class_data) {
  Adw::Bin_Class::class_init_function(g_class, class_data);

  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(g_class);

  gtk_widget_class_set_template_from_resource(
      widget_class,
      "/org/gnome/Adwaitamm1/Demo/ui/pages/about/adw-demo-page-about.ui");

  gtk_widget_class_install_action(
      widget_class, "demo.run", NULL,
      (GtkWidgetActionActivateFunc)DemoPageAbout::demo_run_cb);
}

Glib::ObjectBase *DemoPageAbout_Class::wrap_new(GObject *obj) {
  return manage(new DemoPageAbout(
      G_TYPE_CHECK_INSTANCE_CAST(obj, DemoPageAbout::get_type(), GtkWidget)));
}

///////////////////
// DemoPageAbout //
///////////////////

DemoPageAbout::DemoPageAbout(GtkWidget *obj) : Adw::Bin(ADW_BIN(obj)) {}

DemoPageAbout::~DemoPageAbout() {
  gtk_widget_dispose_template(GTK_WIDGET(gobj()), G_OBJECT_TYPE(gobj()));
  destroy_();
}

void DemoPageAbout::instance_init_function(GTypeInstance *instance,
                                           void *g_class) {
  gtk_widget_init_template(GTK_WIDGET(instance));
}

GType DemoPageAbout::get_type() {
  return demo_page_about_class_.init().get_type();
}

DemoPageAbout *DemoPageAbout::wrap(GObject *obj) {
  return dynamic_cast<DemoPageAbout *>(Glib::wrap_auto(
      G_TYPE_CHECK_INSTANCE_CAST(obj, DemoPageAbout::get_type(), GObject)));
}

void DemoPageAbout::demo_run_cb(GObject *self) {
  DemoPageAbout *d = DemoPageAbout::wrap(self);
  d->demo_run();
}

void DemoPageAbout::demo_run() {
  Gtk::Root *root = get_root();

  std::vector<Glib::ustring> developers{"Angela Avery <angela@example.org>"};

  std::vector<Glib::ustring> artists{"GNOME Design Team"};

  std::vector<Glib::ustring> special_thanks{"My cat"};

  const char *release_notes = "\
<p>\
  This release adds the following features:\
</p>\
<ul>\
  <li>Added a way to export fonts.</li>\
  <li>Better support for <code>monospace</code> fonts.</li>\
  <li>Added a way to preview <em>italic</em> text.</li>\
  <li>Bug fixes and performance improvements.</li>\
  <li>Translation updates.</li>\
</ul>\
  ";

  auto about = new Adw::AboutWindow();
  about->set_transient_for(*dynamic_cast<Gtk::Window *>(root));
  about->set_application_icon("org.example.Typeset");
  about->set_application_name(_("Typeset"));
  about->set_developer_name(_("Angela Avery"));
  about->set_version("1.2.3");
  about->set_release_notes_version("1.2.0");
  about->set_release_notes(release_notes);
  about->set_comments(_("Typeset is an app that doesn’t exist and is used as "
                        "an example content for this about window."));
  about->set_website("https://example.org");
  about->set_issue_url("https://example.org");
  about->set_support_url("https://example.org");
  about->set_copyright("© 2022 Angela Avery");
  about->set_license_type(Gtk::License::LGPL_2_1);
  about->set_developers(developers);
  about->set_artists(artists);
  about->set_translator_credits(_("translator-credits"));

  about->add_link(_("Documentation"),
                  "https://gnome.pages.gitlab.gnome.org/libadwaita/doc/main/"
                  "class.AboutWindow.html");

  about->add_legal_section(_("Fonts"), "", Gtk::License::CUSTOM,
                           "This application uses font data from <a "
                           "href='https://example.org'>somewhere</a>.");

  about->add_acknowledgement_section(_("Special thanks to"), special_thanks);

  about->present();
}

DemoPageAbout_Class DemoPageAbout::demo_page_about_class_;

} // namespace Adw
