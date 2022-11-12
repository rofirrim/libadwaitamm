#include <glib/gi18n.h>
#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

#include "adw-demo-debug-info.h"
#include "adw-demo-preferences-window.h"
#include "adw-demo-window.h"

static void show_inspector() { gtk_window_set_interactive_debugging(TRUE); }

static void show_preferences(const Glib::RefPtr<Gtk::Application> &app) {

  Gtk::Window *window = app->get_active_window();

  auto preferences = new Adw::DemoPreferencesWindow();
  preferences->set_transient_for(*window);
  preferences->present();
}

static void show_about(const Glib::RefPtr<Gtk::Application> &app) {
  std::vector<Glib::ustring> developers{
      "Adrien Plazas", "Alexander Mikhaylenko", "Andrei Lișiță",
      "Guido Günther", "Julian Sparber",        "Manuel Genovés",
      "Zander Brown",
  };

  std::vector<Glib::ustring> designers{
      "GNOME Design Team",
  };

  Gtk::Window *window = app->get_active_window();

  char *debug_info;
  debug_info = adw_demo_generate_debug_info ();

  auto about = new Adw::AboutWindow();

  about->set_transient_for(*window);
  about->set_application_icon("org.gnome.Adwaitamm1.Demo");
  about->set_application_name(_("Adwaitamm Demo"));
  about->set_developer_name("Roger Ferrer Ibáñez");
  about->set_version(ADW_VERSION_S);
  about->set_debug_info(debug_info);
  about->set_copyright("© 2017–2022 Purism SPC, © 2022 Roger Ferrer Ibáñez");
  about->set_license_type(Gtk::License::LGPL_2_1);
  about->set_developers(developers);
  about->set_designers(designers);
  about->set_artists(designers);
  about->set_translator_credits(_("translator-credits"));

  about->present();
  g_free (debug_info);
}

static void show_window(const Glib::RefPtr<Gtk::Application> &app) {
  auto window = Adw::DemoWindow::create(app);
  window->present();
}

int main(int argc, char **argv) {
  Adw::init();

  auto app = Adw::Application::create("org.gnome.Adwaitamm1.Demo",
                                      Gio::Application::Flags::NON_UNIQUE);

  app->add_action("inspector", sigc::ptr_fun(show_inspector));
  app->add_action("preferences",
                  sigc::bind(sigc::ptr_fun(show_preferences), app));
  app->add_action("about", sigc::bind(sigc::ptr_fun(show_about), app));

  app->signal_activate().connect(sigc::bind(sigc::ptr_fun(show_window), app));

  int status = app->run(argc, argv);

  return status;
}
