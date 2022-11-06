#pragma once

#include <functional>
#include <type_traits>
#include <glibmm/extraclassinit.h>
#include <gtkmm.h>

/*
 * This is a convenience header to simplify as much as possible
 * the handling of GTK widget templates and forwarding them to member functions
 * of the C++ instance.
 *
 * Inherit from TemplateBuilder and fill the different parameters of the
 * constructor:
 *   - this
 *   - the resource path to the .ui file
 *   - the widgets you want to bind from the .ui file (they must be pointers in
 *     the C++ class)
 *   - the callbacks you want to bind. You can bind them to (non-static) member
 *     functions of the C++ class using a call ptr_to_mem_fun<&Class:mem_fun>().
 *     Otherwise you can pass a regular function (possibly a static member)
 *     casted to GCallback
 *
 * Then in the constructor invoke "bind_widgets()" to bind everything.
 */

namespace Gtk {

using BindWidgets = std::vector<std::pair<Glib::ustring, void *>>;
using BindCallbacks = std::vector<std::pair<Glib::ustring, GCallback>>;

template <typename CppClass> struct TemplateBuilderInfo {
  Glib::ustring resource;
  BindWidgets widgets_to_bind;
  BindCallbacks callbacks_to_bind;

  TemplateBuilderInfo(const Glib::ustring &resource,
                      const BindWidgets &widgets_to_bind,
                      const BindCallbacks &callbacks_to_bind)
      : resource(resource), widgets_to_bind(widgets_to_bind),
        callbacks_to_bind(callbacks_to_bind) {}
};

template <auto P, typename T = decltype(P)> struct PtrFunToMemFun;

template <auto P, typename CppClass, typename Ret, typename... Args>
struct PtrFunToMemFun<P, Ret (CppClass::*)(Args...)> {
  static Ret callback(GTypeInstance *instance, Args... args) {
    CppClass *w = dynamic_cast<CppClass *>(Glib::wrap(GTK_WIDGET(instance)));
    return (w->*P)(args...);
  }
};

template <auto P> GCallback ptr_fun_to_mem_fun() {
  return GCallback(PtrFunToMemFun<P>::callback);
}

template <typename CppClass>
struct TemplateBuilder : private std::unique_ptr<TemplateBuilderInfo<CppClass>>,
                         public Glib::ExtraClassInit {
private:
  using BuilderInfo = TemplateBuilderInfo<CppClass>;
  using UniquePtrBuilderInfo = std::unique_ptr<TemplateBuilderInfo<CppClass>>;

public:
  TemplateBuilder(GTypeInstance *cobject, CppClass *this_,
                  const Glib::ustring &res, const BindWidgets &widgets_to_bind,
                  const BindCallbacks &callbacks_to_bind)
      : UniquePtrBuilderInfo(std::make_unique<BuilderInfo>(res, widgets_to_bind,
                                                           callbacks_to_bind)),
        Glib::ExtraClassInit(class_init, this->UniquePtrBuilderInfo::get(),
                             instance_init),
        cobject(cobject), this_(this_) {}

  TemplateBuilder(CppClass *this_, const Glib::ustring &res,
                  const BindWidgets &widgets_to_bind,
                  const BindCallbacks &callbacks_to_bind)
      : TemplateBuilder(nullptr, this_, res, widgets_to_bind,
                        callbacks_to_bind) {}

  TemplateBuilder() : Glib::ExtraClassInit(nullptr, nullptr, nullptr) {
    g_assert_not_reached();
  }

  void init_widget_template() {
    if (!this_->gobj())
      return;
    gtk_widget_init_template(GTK_WIDGET(this_->gobj()));

    auto &base = (UniquePtrBuilderInfo &)*this;
    for (const auto &widget : base->widgets_to_bind) {
      *static_cast<Gtk::Widget **>(widget.second) =
          Glib::wrap(GTK_WIDGET(gtk_widget_get_template_child(
              GTK_WIDGET(this_->gobj()), G_OBJECT_TYPE(this_->gobj()),
              widget.first.c_str())));
    }
  }

  void dispose_widget_template() {
    if (!this_->gobj())
      return;
    gtk_widget_dispose_template(GTK_WIDGET(this_->gobj()),
                                G_OBJECT_TYPE(this_->gobj()));
  }

private:
  static void class_init(void *g_class, void *class_data);
  static void instance_init(GTypeInstance* instance, void* g_class);

  GTypeInstance* cobject;
  CppClass *this_;
};

template <typename CppClass>
void TemplateBuilder<CppClass>::class_init(void *g_class, void *class_data) {
  BuilderInfo *builder_info = static_cast<BuilderInfo *>(class_data);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(g_class);
  gtk_widget_class_set_template_from_resource(widget_class,
                                              builder_info->resource.c_str());

  for (const auto &widget : builder_info->widgets_to_bind) {
    gtk_widget_class_bind_template_child_full(widget_class,
                                              widget.first.c_str(), false, 0);
  }

  for (const auto &cb : builder_info->callbacks_to_bind) {
    gtk_widget_class_bind_template_callback_full(widget_class, cb.first.c_str(),
                                                 cb.second);
  }
}

template <typename CppClass>
void TemplateBuilder<CppClass>::instance_init(GTypeInstance *instance,
                                              void *g_class) {
  // // g_print("(1) %s\n", __PRETTY_FUNCTION__);

  // if constexpr (std::is_constructible_v<CppClass, GTypeInstance *>) {
  //   g_print("(1) %s\n", __PRETTY_FUNCTION__);
  //   manage(new CppClass(nullptr));
  // }
}

} // namespace Gtk
