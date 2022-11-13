#pragma once

#include <gtkmm.h>

namespace Gtk {

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

class TemplateWidgetSetup {
private:
  GtkWidgetClass *widget_class;

public:
  explicit TemplateWidgetSetup(GtkWidgetClass *widget_class)
      : widget_class(widget_class) {}

  void set_resource(const Glib::ustring &resource_name) {
    gtk_widget_class_set_template_from_resource(widget_class,
                                                resource_name.c_str());
  }

  void bind_widget(const Glib::ustring &widget_name) {
    gtk_widget_class_bind_template_child_full(widget_class, widget_name.c_str(),
                                              false, 0);
  }

  void bind_callback(const Glib::ustring &name, GCallback callback) {
    gtk_widget_class_bind_template_callback_full(widget_class, name.c_str(),
                                                 callback);
  }
};

class TemplateWidgetInit {
private:
  GTypeInstance *instance;

public:
  explicit TemplateWidgetInit(GTypeInstance *instance) : instance(instance) {}

  void init_template() { gtk_widget_init_template(GTK_WIDGET(instance)); }

  template <typename Widget>
  void bind_widget(Widget *&widget, const Glib::ustring &name) {
    widget = dynamic_cast<Widget *>(
        Glib::wrap(GTK_WIDGET(gtk_widget_get_template_child(
            GTK_WIDGET(instance), G_OBJECT_TYPE(instance), name.c_str()))));
  }
};

template <typename CppObjectClass, typename CppBaseObjectClass>
class TemplateWidget : public CppBaseObjectClass {
public:
  struct TemplateWidgetClass;

  TemplateWidget(GtkWidget *obj)
      : CppBaseObjectClass(
            (typename CppBaseObjectClass::BaseObjectType *)(obj)) {}

  virtual ~TemplateWidget() {
    gtk_widget_dispose_template(GTK_WIDGET(this->gobj()),
                                G_OBJECT_TYPE(this->gobj()));
    this->destroy_();
  }

  static TemplateWidgetClass type_class_;

  static GType get_type() { return type_class_.init().get_type(); }

  static CppObjectClass *wrap(GObject *obj) {
    return dynamic_cast<CppObjectClass *>(Glib::wrap_auto(
        G_TYPE_CHECK_INSTANCE_CAST(obj, CppObjectClass::get_type(), GObject)));
  }

  using TemplateWidgetBase = TemplateWidget;
  using CppClassType = TemplateWidget::TemplateWidgetClass;
};

template <typename CppObjectClass, typename CppBaseObjectClass>
class TemplateWidget<CppObjectClass, CppBaseObjectClass>::TemplateWidgetClass
    : public Glib::Class {
public:
  using Class::Class;

  // Do not hide the parent functions.
  using Class::register_derived_type;
  void register_derived_type(GType base_type,
                             /* static lifetime */ const char *derived_name,
                             GInstanceInitFunc instance_init) {
    return register_derived_type(base_type, derived_name, instance_init,
                                 nullptr);
  }

  void register_derived_type(GType base_type, const char *derived_name,
                             GInstanceInitFunc instance_init,
                             GTypeModule *module) {
    if (gtype_)
      return; // already initialized

    if (base_type == 0) {
      g_critical("TemplateWidgetClass::register_derived_type(): base type does "
                 "not exist?");
      return;
    }

#if GLIB_CHECK_VERSION(2, 70, 0)
    if (G_TYPE_IS_FINAL(base_type)) {
      g_critical(
          "TemplateWidgetClass::register_derived_type(): cannot derive from a "
          "final type!");
      return;
    }
#endif

    GTypeQuery base_query = {
        0,
        nullptr,
        0,
        0,
    };
    g_type_query(base_type, &base_query);

    // GTypeQuery::class_size is guint but GTypeInfo::class_size is guint16.
    const guint16 class_size = (guint16)base_query.class_size;

    // GTypeQuery::instance_size is guint but GTypeInfo::instance_size is
    // guint16.
    const guint16 instance_size = (guint16)base_query.instance_size;

    const GTypeInfo derived_info = {
        class_size,
        nullptr,          // base_init
        nullptr,          // base_finalize
        class_init_func_, // Set by the caller ( *_Class::init() ).
        nullptr,          // class_finalize
        nullptr,          // class_data
        instance_size,
        0,             // n_preallocs
        instance_init, // instance_init
        nullptr,       // value_table
    };

    if (!derived_name) {
      g_critical("TemplateWidgetClass::register_derived_type(): derived_name "
                 "is NULL.");
      return;
    }

    if (module)
      gtype_ = g_type_module_register_type(module, base_type, derived_name,
                                           &derived_info, GTypeFlags(0));
    else
      gtype_ = g_type_register_static(base_type, derived_name, &derived_info,
                                      GTypeFlags(0));
  }

  const Class &init() {
    if (!gtype_) {
      class_init_func_ = TemplateWidgetClass::class_init_function;
      register_derived_type(CppBaseObjectClass::get_base_type(),
                            CppObjectClass::class_name,
                            &TemplateWidgetClass::instance_init_function);
      Glib::init();
      Glib::wrap_register(gtype_, &wrap_new);
    }
    return *this;
  }

  static Glib::ObjectBase *wrap_new(GObject *obj) {
    return new CppObjectClass(
        G_TYPE_CHECK_INSTANCE_CAST(obj, CppObjectClass::get_type(), GtkWidget));
  }

  static void class_init_function(void *g_class, void *class_data) {
    CppBaseObjectClass::CppClassType::class_init_function(g_class, class_data);
    TemplateWidgetSetup s(GTK_WIDGET_CLASS(g_class));
    CppObjectClass::setup_template(s);
  }

  static void instance_init_function(GTypeInstance *instance, void *g_class) {
    TemplateWidgetInit i(instance);
    CppObjectClass *self = CppObjectClass::wrap(G_OBJECT(instance));
    self->init_widget(i);
  }
};

template <typename CppObjectClass, typename CppBaseObjectClass>
typename TemplateWidget<CppObjectClass, CppBaseObjectClass>::TemplateWidgetClass
    TemplateWidget<CppObjectClass, CppBaseObjectClass>::type_class_;

} // namespace Gtk
