#pragma once

#include <gtkmm.h>

namespace Gtk {

template <typename CppTypeClass, typename CppObjectClass,
          GType (*c_object_get_type)()>
class TemplateWidgetClass : public Glib::Class {
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
      class_init_func_ = CppTypeClass::class_init_function;
      register_derived_type(c_object_get_type(), CppTypeClass::class_name,
                            &CppObjectClass::instance_init_function);
      Glib::init();
      Glib::wrap_register(gtype_, &wrap_new);
    }
    return *this;
  }

  static Glib::ObjectBase *wrap_new(GObject *obj) {
    return new CppObjectClass(
        G_TYPE_CHECK_INSTANCE_CAST(obj, CppObjectClass::get_type(), GtkWidget));
  }

  using TemplateWidgetClassBase = TemplateWidgetClass;
};

template <typename CppTypeClass, typename CppObjectClass,
          typename CppBaseObjectClass, typename CBaseObjectClass>
class TemplateWidget : public CppBaseObjectClass {
public:
  TemplateWidget(GtkWidget *obj)
      : CppBaseObjectClass((CBaseObjectClass *)(obj)) {}

  virtual ~TemplateWidget() {
    gtk_widget_dispose_template(GTK_WIDGET(this->gobj()),
                                G_OBJECT_TYPE(this->gobj()));
    this->destroy_();
  }

  friend CppTypeClass;
  static CppTypeClass type_class_;

  static GType get_type() { return type_class_.init().get_type(); }

  static CppObjectClass *wrap(GObject *obj) {
    return dynamic_cast<CppObjectClass *>(Glib::wrap_auto(
        G_TYPE_CHECK_INSTANCE_CAST(obj, CppObjectClass::get_type(), GObject)));
  }

  using TemplateWidgetBase = TemplateWidget;
};

template <typename CppTypeClass, typename CppObjectClass,
          typename CppBaseObjectClass, typename CBaseObjectClass>
CppTypeClass TemplateWidget<CppTypeClass, CppObjectClass, CppBaseObjectClass,
                            CBaseObjectClass>::type_class_;

} // namespace Gtk
