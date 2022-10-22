#include <glib/gi18n.h>
#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

#include "adw-demo-debug-info.h"
#include "adw-demo-preferences-window.h"
#include "adw-demo-window.h"

#if 0

static void
show_inspector (GSimpleAction *action,
                GVariant      *state,
                gpointer       user_data)
{
  gtk_window_set_interactive_debugging (TRUE);
}

static void
show_preferences (GSimpleAction *action,
                  GVariant      *state,
                  gpointer       user_data)
{
  GtkApplication *app = GTK_APPLICATION (user_data);
  GtkWindow *window = gtk_application_get_active_window (app);
  AdwDemoPreferencesWindow *preferences = adw_demo_preferences_window_new ();

  gtk_window_set_transient_for (GTK_WINDOW (preferences), window);
  gtk_window_present (GTK_WINDOW (preferences));
}

static void
show_about (GSimpleAction *action,
            GVariant      *state,
            gpointer       user_data)
{
  const char *developers[] = {
    "Adrien Plazas",
    "Alexander Mikhaylenko",
    "Andrei Lișiță",
    "Guido Günther",
    "Julian Sparber",
    "Manuel Genovés",
    "Zander Brown",
    NULL
  };

  const char *designers[] = {
    "GNOME Design Team",
    NULL
 };

  GtkApplication *app = GTK_APPLICATION (user_data);
  GtkWindow *window = gtk_application_get_active_window (app);
  char *debug_info;
  GtkWidget *about;

  debug_info = adw_demo_generate_debug_info ();

  about = GTK_WIDGET(
    g_object_new (ADW_TYPE_ABOUT_WINDOW,
                  "transient-for", window,
                  "application-icon", "org.gnome.Adwaitamm1.Demo",
                  "application-name", _("Adwaitamm Demo"),
                  "developer-name", _("Roger Ferrer Ibáñez"),
                  "version", ADW_VERSION_S,
                  "debug-info", debug_info,
                  "copyright", "© 2017–2022 Purism SPC, © 2022 Roger Ferrer Ibáñez",
                  "license-type", GTK_LICENSE_LGPL_2_1,
                  "developers", developers,
                  "designers", designers,
                  "artists", designers,
                  "translator-credits", _("translator-credits"),
                  NULL) );

  gtk_window_present (GTK_WINDOW (about));

  g_free (debug_info);
}

static void
show_window (GtkApplication *app)
{
  AdwDemoWindow *window;

  window = adw_demo_window_new (app);

  gtk_window_present (GTK_WINDOW (window));
}
#endif

static void show_inspector() {}
static void show_preferences() {}

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

  auto about = Adw::AboutWindow::create();
  // This is lame
  about->reference();

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
  AdwDemoWindow *window;

  window = adw_demo_window_new(app->gobj());

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
  // AdwApplication *app;
  // int status;
  // static GActionEntry app_entries[] = {
  //   { "inspector", show_inspector, NULL, NULL, NULL },
  //   { "preferences", show_preferences, NULL, NULL, NULL },
  //   { "about", show_about, NULL, NULL, NULL },
  // };

  // app = adw_application_new ("org.gnome.Adwaitamm1.Demo",
  // G_APPLICATION_NON_UNIQUE); g_action_map_add_action_entries (G_ACTION_MAP
  // (app),
  //                                  app_entries, G_N_ELEMENTS (app_entries),
  //                                  app);
  // g_signal_connect (app, "activate", G_CALLBACK (show_window), NULL);
  // status = g_application_run (G_APPLICATION (app), argc, argv);
  // g_object_unref (app);
  
  Adw::init();

  auto app = Adw::Application::create("org.gnome.Adwaitamm1.Demo",
                                      Gio::Application::Flags::NON_UNIQUE);

  app->add_action("inspector", sigc::ptr_fun(show_inspector));
  app->add_action("preferences", sigc::ptr_fun(show_preferences));
  app->add_action("about", sigc::bind(sigc::ptr_fun(show_about), app));

  app->signal_activate().connect(sigc::bind(sigc::ptr_fun(show_window), app));

  int status = app->run(argc, argv);

  return status;
}
