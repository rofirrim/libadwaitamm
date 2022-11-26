#include "adw-tab-view-demo-window.h"

#include <glib/gi18n.h>

namespace Adw {

const char TabViewDemoWindow::class_name[] = "AdwTabViewDemoWindow";

TabViewDemoWindow *TabViewDemoWindow::create() {
  GObject *obj = g_object_new_with_properties(TabViewDemoWindow::get_type(), 0,
                                              nullptr, nullptr);
  return TabViewDemoWindow::wrap(obj);
}

void TabViewDemoWindow::setup_template(Gtk::TemplateWidgetSetup &s) {
  s.set_resource("/org/gnome/Adwaitamm1/Demo/ui/pages/tab-view/"
                 "adw-tab-view-demo-window.ui");

  s.bind_widget("view");
  s.bind_widget("tab_bar");

  s.bind_callback(
      "page_detached_cb",
      Gtk::ptr_fun_to_mem_fun<&TabViewDemoWindow::page_detached_cb>());
  s.bind_callback("setup_menu_cb",
                  Gtk::ptr_fun_to_mem_fun<&TabViewDemoWindow::setup_menu_cb>());
  s.bind_callback(
      "create_window_cb",
      Gtk::ptr_fun_to_mem_fun<&TabViewDemoWindow::create_window_cb>());
  s.bind_callback(
      "indicator_activated_cb",
      Gtk::ptr_fun_to_mem_fun<&TabViewDemoWindow::indicator_activated_cb>());
  s.bind_callback(
      "extra_drag_drop_cb",
      Gtk::ptr_fun_to_mem_fun<&TabViewDemoWindow::extra_drag_drop_cb>());

  // FIXME: Ideally we'd like this wrapped as well.
  gtk_widget_class_add_binding_action(s.get_class(), GDK_KEY_t,
                                      GDK_CONTROL_MASK, "win.tab-new", NULL);
  gtk_widget_class_add_binding_action(s.get_class(), GDK_KEY_n,
                                      GDK_CONTROL_MASK, "win.window-new", NULL);
  gtk_widget_class_add_binding_action(s.get_class(), GDK_KEY_w,
                                      GDK_CONTROL_MASK, "tab.close", NULL);
}

Glib::RefPtr<Adw::TabPage> TabViewDemoWindow::get_current_page() {
  if (menu_page)
    return menu_page;
  return view->get_selected_page();
}

void TabViewDemoWindow::init_widget(Gtk::TemplateWidgetInit &i) {
  i.init_template();
  i.bind_widget(view, "view");
  i.bind_widget(tab_bar, "tab_bar");

  Glib::RefPtr<Gio::SimpleActionGroup> simple_action_group =
      Gio::SimpleActionGroup::create();
  simple_action_group->add_action("window-new", [this]() { window_new(); });
  simple_action_group->add_action("tab-new", [this]() { tab_new(); });

  insert_action_group("win", simple_action_group);

  tab_action_group = Gio::SimpleActionGroup::create();
  tab_action_group->add_action(
      "pin", [this]() { view->set_page_pinned(get_current_page(), true); });
  tab_action_group->add_action(
      "unpin", [this]() { view->set_page_pinned(get_current_page(), false); });
  tab_action_group->add_action(
      "close", [this]() { view->close_page(get_current_page()); });
  tab_action_group->add_action(
      "close-other", [this]() { view->close_other_pages(get_current_page()); });
  tab_action_group->add_action("close-before", [this]() {
    view->close_pages_before(get_current_page());
  });
  tab_action_group->add_action(
      "close-after", [this]() { view->close_pages_after(get_current_page()); });
  tab_action_group->add_action("move-to-new-window",
                               [this]() { tab_move_to_new_window(); });
  auto needs_attention_action =
      tab_action_group->add_action_bool("needs-attention");
  needs_attention_action->signal_change_state().connect(
      [this, needs_attention_action](const Glib::VariantBase &base) {
        Glib::Variant<bool> needs_attention =
            Glib::VariantBase::cast_dynamic<Glib::Variant<bool>>(base);
        get_current_page()->set_needs_attention(needs_attention.get());
        needs_attention_action->set_state(needs_attention);
      });

  auto loading_action = tab_action_group->add_action_bool("loading");
  loading_action->signal_change_state().connect(
      [this, loading_action](const Glib::VariantBase &base) {
        Glib::Variant<bool> loading =
            Glib::VariantBase::cast_dynamic<Glib::Variant<bool>>(base);
        get_current_page()->set_loading(loading.get());
        loading_action->set_state(loading);
      });

  auto indicator_action = tab_action_group->add_action_bool("indicator");
  indicator_action->signal_change_state().connect(
      [this, indicator_action](const Glib::VariantBase &base) {
        Glib::Variant<bool> indicator =
            Glib::VariantBase::cast_dynamic<Glib::Variant<bool>>(base);
        tab_change_indicator(indicator_action, indicator.get());
      });

  auto icon_action = tab_action_group->add_action_bool("icon");
  icon_action->signal_change_state().connect(
      [this, icon_action](const Glib::VariantBase &base) {
        Glib::Variant<bool> enable_icon =
            Glib::VariantBase::cast_dynamic<Glib::Variant<bool>>(base);
        tab_change_icon(icon_action, enable_icon.get());
      });

  tab_action_group->add_action("refresh-icon", [this]() {
    Glib::RefPtr<Gio::Icon> icon = get_random_icon();
    get_current_page()->set_icon(icon);
  });
  tab_action_group->add_action("duplicate", [this]() { tab_duplicate(); });

  insert_action_group("tab", tab_action_group);

  tab_bar->setup_extra_drop_target(Gdk::DragAction::COPY, {G_TYPE_STRING});
}

void TabViewDemoWindow::prepopulate() {
  tab_new();
  tab_new();
  tab_new();
}

std::vector<Glib::ustring> icon_names;

void TabViewDemoWindow::init_icon_names(
    const Glib::RefPtr<Gtk::IconTheme> &theme) {
  if (!icon_names.empty())
    return;
  icon_names = theme->get_icon_names();
}

Glib::RefPtr<Gio::Icon> TabViewDemoWindow::get_random_icon() {
  auto display = get_display();
  auto theme = Gtk::IconTheme::get_for_display(display);

  init_icon_names(theme);

  int index = g_random_int_range(0, icon_names.size());

  return Gio::ThemedIcon::create(icon_names[index]);
}

void TabViewDemoWindow::tab_new() {
  Glib::RefPtr<Gio::Icon> icon = get_random_icon();
  static int next_page = 1;
  Glib::ustring title = Glib::ustring::sprintf(_("Tab %d"), next_page);

  Glib::RefPtr<Adw::TabPage> page = add_page(nullptr, title, icon);
  Gtk::Widget *content = page->get_child();

  view->set_selected_page(page);
  content->grab_focus();

  next_page++;
}

Glib::RefPtr<Adw::TabPage>
TabViewDemoWindow::add_page(const Glib::RefPtr<Adw::TabPage> &parent,
                            const Glib::ustring &title,
                            Glib::RefPtr<Gio::Icon> icon) {
  Gtk::Entry *content = new Gtk::Entry;
  content->set_text(title);
  content->set_halign(Gtk::Align::CENTER);
  content->set_valign(Gtk::Align::CENTER);

  Glib::RefPtr<Adw::TabPage> page = view->add_page(content, parent);

  Glib::Binding::bind_property(content->property_text(), page->property_title(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);

  Glib::Binding::bind_property_value(
      content->property_text(), page->property_tooltip(),
      Glib::Binding::Flags::SYNC_CREATE,
      sigc::mem_fun(*this, &TabViewDemoWindow::text_to_tooltip));

  page->set_icon(icon);
  page->set_indicator_activatable(true);

  return page;
}

Glib::RefPtr<Gio::Icon>
TabViewDemoWindow::get_indicator_icon(const Glib::RefPtr<Adw::TabPage> &page) {
  void *p = page->get_data("adw-tab-view-demo-muted");
  bool muted = (intptr_t)p;

  if (muted)
    return Gio::ThemedIcon::create("tab-audio-muted-symbolic");
  else
    return Gio::ThemedIcon::create("tab-audio-playing-symbolic");
}

Glib::ustring TabViewDemoWindow::get_indicator_tooltip(
    const Glib::RefPtr<Adw::TabPage> &page) {
  void *p = page->get_data("adw-tab-view-demo-muted");
  bool muted = (intptr_t)p;

  if (muted)
    return _("Unmute Tab");
  else
    return _("Mute Tab");
}

void TabViewDemoWindow::tab_change_indicator(
    const Glib::RefPtr<Gio::SimpleAction> &action, bool indicator) {

  Glib::RefPtr<Gio::Icon> icon;
  Glib::ustring tooltip;

  Glib::RefPtr<Adw::TabPage> page = get_current_page();

  if (indicator) {
    icon = get_indicator_icon(page);
    tooltip = get_indicator_tooltip(page);
  }

  page->set_indicator_icon(icon);
  page->set_indicator_tooltip(tooltip);

  action->set_state(Glib::Variant<bool>::create(indicator));
}

void TabViewDemoWindow::tab_change_icon(
    const Glib::RefPtr<Gio::SimpleAction> &action, bool enable_icon) {
  Glib::RefPtr<Adw::TabPage> page = get_current_page();
  if (enable_icon) {
    Glib::RefPtr<Gio::Icon> icon = get_random_icon();
    page->set_icon(icon);
  } else {
    page->set_icon(nullptr);
  }

  action->set_state(Glib::Variant<bool>::create(enable_icon));
}

void TabViewDemoWindow::tab_move_to_new_window() {
  TabViewDemoWindow *window = TabViewDemoWindow::create();

  view->transfer_page(menu_page, window->view, 0);
  window->present();
}

void TabViewDemoWindow::tab_duplicate() {
  Glib::RefPtr<Adw::TabPage> parent = get_current_page();

  Glib::RefPtr<Adw::TabPage> page =
      add_page(parent, parent->get_title(), parent->get_icon());

  page->set_indicator_icon(parent->get_indicator_icon());
  page->set_indicator_tooltip(parent->get_indicator_tooltip());
  page->set_loading(parent->get_loading());
  page->set_needs_attention(parent->get_needs_attention());

  page->set_data("adw-tab-view-demo-muted",
                 parent->get_data("adw-tab-view-demo-muted"));

  view->set_selected_page(page);
}

void TabViewDemoWindow::page_detached_cb(AdwTabPage *) {
  if (!view->get_n_pages())
    close();
}

void TabViewDemoWindow::setup_menu_cb(AdwTabPage *page_, AdwTabView *view_) {
  Glib::RefPtr<Adw::TabPage> page = Glib::wrap(page_);
  if (page) page->reference();

  Adw::TabView *view = Glib::wrap(view_);

  menu_page = page;

  unsigned int n_pages = view->get_n_pages();

  bool can_close_before = true, can_close_after = true;
  bool pinned = false;
  bool has_icon = false;

  if (page) {
    Glib::RefPtr<Adw::TabPage> prev;
    unsigned int pos = view->get_page_position(page);

    if (pos > 0) {
      prev = view->get_nth_page(pos - 1);
    }

    pinned = page->get_pinned();
    bool prev_pinned = prev && prev->get_pinned();

    can_close_before = !pinned && prev && !prev_pinned;
    can_close_after = pos < n_pages - 1;

    has_icon = page->get_icon() != nullptr;
  }

  set_tab_action_enabled("pin", !page || !pinned);
  set_tab_action_enabled("unpin", !page || pinned);
  set_tab_action_enabled("close", !page || !pinned);
  set_tab_action_enabled("close-before", can_close_before);
  set_tab_action_enabled("close-after", can_close_after);
  set_tab_action_enabled("close-other", can_close_before || can_close_after);
  set_tab_action_enabled("move-to-new-window",
                         !page || (!pinned && n_pages > 1));
  set_tab_action_enabled("refresh-icon", has_icon);

  if (page) {
    set_tab_action_state("icon", has_icon);
    set_tab_action_state("loading", page->get_loading());
    set_tab_action_state("needs-attention", page->get_needs_attention());
    set_tab_action_state("indicator", page->get_indicator_icon() != NULL);
  }
}

AdwTabView *TabViewDemoWindow::create_window_cb() {
  Adw::TabViewDemoWindow *window = Adw::TabViewDemoWindow::create();
  window->present();
  return window->view->gobj();
}

void TabViewDemoWindow::indicator_activated_cb(AdwTabPage *page_) {
  Glib::RefPtr<Adw::TabPage> page = Glib::wrap(page_);
  if (page) page->reference();

  bool muted = (intptr_t)page->get_data("adw-tab-view-demo-muted");

  page->set_data("adw-tab-view-demo-muted", (void *)(intptr_t)!muted);

  Glib::RefPtr<Gio::Icon> icon = get_indicator_icon(page);
  Glib::ustring tooltip = get_indicator_tooltip(page);

  page->set_indicator_icon(icon);
  page->set_indicator_tooltip(tooltip);
}

gboolean TabViewDemoWindow::extra_drag_drop_cb(AdwTabPage *page_,
                                               GValue *value) {
  Glib::RefPtr<Adw::TabPage> page = Glib::wrap(page_);
  if (page) page->reference();

  Glib::Value<Glib::ustring> title;
  title.init(value);

  page->set_title(title.get());

  return GDK_EVENT_STOP;
}

void TabViewDemoWindow::set_tab_action_enabled(const Glib::ustring &name,
                                               bool enabled) {
  Glib::RefPtr<Gio::SimpleAction> action =
      std::dynamic_pointer_cast<Gio::SimpleAction>(
          tab_action_group->lookup_action(name));
  action->set_enabled(enabled);
}

void TabViewDemoWindow::set_tab_action_state(const Glib::ustring &name,
                                             bool state) {

  Glib::RefPtr<Gio::SimpleAction> action =
      std::dynamic_pointer_cast<Gio::SimpleAction>(
          tab_action_group->lookup_action(name));
  action->set_state(Glib::Variant<bool>::create(state));
}

void TabViewDemoWindow::window_new() {
  Adw::TabViewDemoWindow *window = Adw::TabViewDemoWindow::create();
  window->prepopulate();
  window->present();
}

bool TabViewDemoWindow::text_to_tooltip(const GValue *input_, GValue *output_) {
  Glib::Value<Glib::ustring> input;
  input.init(input_);

  Glib::ustring title = input.get();

  Glib::ustring tooltip = Glib::Markup::escape_text(
      Glib::ustring::sprintf(_("Elaborate tooltip for <b>%s</b>"), title));

  // This is not very elegant but should work.
  static_assert(std::is_standard_layout_v<Glib::Value<Glib::ustring>>,
                "Glib::ValueBase must be standard layout");
  Glib::Value<Glib::ustring> &output =
      reinterpret_cast<decltype(output) &>(*output_);

  output.set(tooltip);

  return true;
}

} // namespace Adw
