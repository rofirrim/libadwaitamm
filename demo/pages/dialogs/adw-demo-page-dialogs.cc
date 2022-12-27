#include "adw-demo-page-dialogs.h"

#include <glib/gi18n.h>

namespace Adw {

const char DemoPageDialogs::class_name[] = "AdwDemoPageDialogs";
unsigned int DemoPageDialogs::signal_add_toast;

void DemoPageDialogs::setup_template(Gtk::TemplateWidgetSetup &s) {
  // Creation of signals should be somehow wrapped, although this hasn't
  // typically needed.
  signal_add_toast =
      g_signal_new("add-toast", s.get_class_type(), G_SIGNAL_RUN_FIRST, 0, NULL,
                   NULL, NULL, G_TYPE_NONE, 1, ADW_TYPE_TOAST);

  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/dialogs/adw-demo-page-dialogs.ui");
  s.install_action(
      "demo.message-dialog",
      Gtk::ptr_fun_to_mem_fun<&DemoPageDialogs::demo_message_dialog_cb>());
}

void DemoPageDialogs::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
}

void DemoPageDialogs::demo_message_dialog_cb()
{
  Gtk::Window *parent = dynamic_cast<Gtk::Window*>(get_root());

  Adw::MessageDialog *dialog = new Adw::MessageDialog(
      parent, _("Save Changes?"),
      _("Open document contains unsaved changes. Changes which are not saved "
        "will be permanently lost."));

  dialog->add_response("cancel",  _("_Cancel"));
  dialog->set_close_response("cancel");

  dialog->add_response("discard", _("_Discard"));
  dialog->set_response_appearance("discard",
                                  Adw::ResponseAppearance::DESTRUCTIVE);

  dialog->add_response("save",    _("_Save"));
  dialog->set_response_appearance("save", Adw::ResponseAppearance::SUGGESTED);
  dialog->set_default_response("save");

  dialog->signal_response().connect([this](const Glib::ustring &response) {
    auto toast = new Adw::Toast(
        Glib::ustring::sprintf(_("Dialog response: %s"), response));

    // FIXME: Wrap this.
    g_signal_emit(gobj(), signal_add_toast, 0, toast->gobj());
  });

  dialog->present();
}

}
