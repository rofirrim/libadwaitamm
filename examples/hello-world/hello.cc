/* Copyright (c) 2019  Roger Ferrer <rofirrim@gmail.com>
 *
 * This file is part of libadwaitamm.
 *
 * libadwaitamm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * libadwaitamm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h>

static void activate_cb(const Glib::RefPtr<Gtk::Application> &app) {
  auto window = new Gtk::ApplicationWindow(app);
  auto label = new Gtk::Label("Hello World");

  window->set_title("Hello");
  window->set_default_size(200, 200);
  window->set_child(*label);
  window->present();
}

int main(int argc, char **argv) {
  Adw::init();

  auto app = Adw::Application::create("org.example.Hello",
                                      Gio::Application::Flags::NONE);

  app->signal_activate().connect(sigc::bind(sigc::ptr_fun(&activate_cb), app));

  app->run(argc, argv);

  return 0;
}
