#include "adw-demo-page-toasts.h"

#include <glib/gi18n.h>

namespace Adw {

const char DemoPageToasts::class_name[] = "AdwDemoPageToasts";
unsigned int DemoPageToasts::signal_add_toast;

void DemoPageToasts::setup_template(Gtk::TemplateWidgetSetup &s) {

  signal_add_toast =
      g_signal_new("add-toast", s.get_class_type(), G_SIGNAL_RUN_FIRST, 0, NULL,
                   NULL, NULL, G_TYPE_NONE, 1, ADW_TYPE_TOAST);

  s.set_resource(
      "/org/gnome/Adwaitamm1/Demo/ui/pages/toasts/adw-demo-page-toasts.ui");

  s.install_action("toast.add",
                   Gtk::ptr_fun_to_mem_fun<&DemoPageToasts::toast_add_cb>());
  s.install_action(
      "toast.add-with-button",
      Gtk::ptr_fun_to_mem_fun<&DemoPageToasts::toast_add_with_button_cb>());
  s.install_action(
      "toast.add-with-long-title",
      Gtk::ptr_fun_to_mem_fun<&DemoPageToasts::toast_add_with_long_title_cb>());
  s.install_action(
      "toast.dismiss",
      Gtk::ptr_fun_to_mem_fun<&DemoPageToasts::toast_dismiss_cb>());
}

void DemoPageToasts::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
  action_set_enabled("toast.dismiss", false);
}

void DemoPageToasts::toast_add_cb() {
  Glib::RefPtr<Adw::Toast> toast = Adw::Toast::create(_("Simple Toast"));
  toast->reference();

  add_toast(toast);
}

void DemoPageToasts::toast_add_with_button_cb() {
  toast_undo_items++;

  if (undo_toast) {
    Glib::ustring title = Glib::ustring::sprintf(
        ngettext("<span font_features='tnum=1'>%d</span> item deleted",
                 "<span font_features='tnum=1'>%d</span> items deleted",
                 toast_undo_items),
        toast_undo_items);

    undo_toast->set_title(title);

    /* Bump the toast timeout */
    undo_toast->reference();
    add_toast(undo_toast);
  } else {
    undo_toast = Adw::Toast::create(
        Glib::ustring::sprintf(_("‘%s’ deleted"), "Lorem Ipsum"));
    undo_toast->reference();

    undo_toast->set_priority(Adw::ToastPriority::HIGH);
    undo_toast->set_button_label(_("_Undo"));
    undo_toast->set_action_name("toast.undo");

    undo_toast->signal_dismissed().connect([this]() {
      undo_toast.reset();
      toast_undo_items = 0;
      action_set_enabled("toast.dismiss", false);
    });

    add_toast(undo_toast);

    action_set_enabled("toast.dismiss", true);
  }
}

void DemoPageToasts::toast_add_with_long_title_cb() {
  Glib::RefPtr<Adw::Toast> toast =
      Adw::Toast::create(_("Lorem ipsum dolor sit amet, "
                           "consectetur adipiscing elit, "
                           "sed do eiusmod tempor incididunt "
                           "ut labore et dolore magnam aliquam "
                           "quaerat voluptatem."));
  toast->reference();

  add_toast(toast);
}

void DemoPageToasts::add_toast(const Glib::RefPtr<Adw::Toast> &toast) {
  g_signal_emit(gobj(), signal_add_toast, 0, toast->gobj());
}

void DemoPageToasts::toast_dismiss_cb() {
  if (undo_toast)
    undo_toast->dismiss();
}

void DemoPageToasts::undo() {
  Glib::ustring title = Glib::ustring::sprintf(
      ngettext("Undoing deleting <span font_features='tnum=1'>%d</span> item…",
               "Undoing deleting <span font_features='tnum=1'>%d</span> items…",
               toast_undo_items),
      toast_undo_items);

  Glib::RefPtr<Adw::Toast> toast = Adw::Toast::create(title);
  toast->reference();

  toast->set_priority(Adw::ToastPriority::HIGH);

  add_toast(toast);
}

} // namespace Adw
