#include "adw-demo-page-about.h"

#include <glib/gi18n.h>

namespace Adw {

///////////////////
// DemoPageAbout //
///////////////////

const char DemoPageAbout::class_name[] = "AdwDemoPageAbout";

void DemoPageAbout::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/about/adw-demo-page-about.ui");

  s.install_action("demo.run",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageAbout::demo_run>());
}

DemoPageAbout::DemoPageAbout(GtkWidget *obj) : TemplateWidgetBase(obj) {}

void DemoPageAbout::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
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

} // namespace Adw
