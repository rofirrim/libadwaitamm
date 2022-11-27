#pragma once

#include <gtkmm.h>

#if 0

// This defines Gtk::TemplateWidget which is a helper class to ease in the best
// way possible the use of template widget with gtkmm. Unfortunately, this has
// not been wrapped in gtkmm which forces us to rely on C GTK APIs. It is
// possible, though, by constraining the way we can create gtkmm objects to
// make widget templates a bit less cumbersome.
//
// This approach is not perfect but covers the most common use cases when
// using gtk templates. One downside is that you may need, due to headers
// in templates add additional "#include private/widget_p.h".
//
// Assume you're inheriting from (C++) FirstWidget and you plan
// to populate it using a resource. Then you can define a gtkmm wrapper
// SecondWidget like this:

// -------------------------
// -- file secondwidget.h --
// -------------------------

class SecondWidget : public Gtk::TemplateWidget<SecondWidget, FirstWidget> {
  // Due to the CRTP used, we need to access SecondWidget from
  // Gtk::TemplateWidget. A typedef 'CppClassType' has been setup to help with
  // this.
  friend CppClassType;

public:
  // Constructing via a regular constructor will not work due to the machinery
  // of gtkmm that would be invoked. Use a factory instead that returns a raw
  // pointer. Wrap it into a Glib::RefPtr as needed but be mindful of the
  // reference counter. Note: this factory is only needed if you plan to
  // instantiate the widget programmatically (i.e. not just by loading the .ui
  // file)
  static SecondWidget *create(...);

  // This constructor will be used to wrap C instances in C++ instances. It is
  // internally called when linking a C object with its C++ counterpart.
  // Does nothing but chain up the wrapped object. TemplateWidgetBase is
  // a convenience typedef.
  // We use a GtkWidget* in lack of a better option here.
protected:
  explicit SecondWidget(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  // If your widget is a top-level window that does not have to be managed
  // define also this, otherwise no need to define it as it will default to
  // true.
  // static bool is_managed() { return false; }

  // Rest of the class goes here. This is an example.
  Gtk::Label *label_1;
  Gtk::Button *button_1;

  void button_1_on_click();
};

// --------------------------
// -- file secondwidget.cc --
// --------------------------
//
#include <nnn/private/firstwidget_p.h>

// This is the name you will use in the '.ui' files
const char SecondWidget::class_name[] = "SecondWidget";

void SecondWidget::setup_template(Gtk::TemplateWidgetSetup &s) {
  // Define your resource file
  s.set_resource("/com/example/ui/secondwidget.ui");

  // Claim the widgets mentioned in secondwidget.ui
  // This won't bind them to the instance yet. This happens in init_widget.
  s.bind_widget("label_1");
  s.bind_widget("button_1");

  // Bind the callbacks to their functions.
  s.bind_callback("button_1_on_click",
                  Gtk::ptr_fun_to_mem_fun<&DemoWindow::button_1_on_click>());
}

void SecondWidget::init_widget(Gtk::TemplateWidgetInit &i) {
  // Initialize the template.
  i.init_template();

  // Now link the widgets we claimed in setup_template.
  i.bind_widget(label_1, "label_1");
  i.bind_widget(button_1, "button_1");

  // Extra instance initalizations must happen here, like in a regular gtkmm
  // constructor.
  // ...
}

// If you have to expose a constructor, you can do something like this.

SecondWidget *SecondWidget::create() {
  // Pass any construct time property you may need.
  GObject *obj = g_object_new_with_properties(
      SecondWidget::get_type(), 0, nullptr, nullptr);
  return SecondWidget::wrap(obj);
}

#endif

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

  void install_action(const Glib::ustring &name, GCallback callback) {
    gtk_widget_class_install_action(widget_class, name.c_str(), NULL,
                                    (GtkWidgetActionActivateFunc)callback);
  }

  GType get_class_type() const { return G_TYPE_FROM_CLASS(widget_class); }

  GtkWidgetClass *get_class() const { return widget_class; }
};

class TemplateWidgetInit {
private:
  GTypeInstance *instance;

public:
  explicit TemplateWidgetInit(GTypeInstance *insn) : instance(insn) {}

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
    if (this->gobj())
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

  // Reintroduce this method so it returns false in classes that
  // are not meant to be managed.
  static bool is_managed() { return true; }

  using TemplateWidgetBase = TemplateWidget;
  using CppClassType = TemplateWidget::TemplateWidgetClass;

  // This is a convenience method that pairs with install_action.
  void action_set_enabled(const Glib::ustring& name, bool enabled) {
      gtk_widget_action_set_enabled(GTK_WIDGET(this->gobj()), name.c_str(), enabled);
  }
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
    auto *result = new CppObjectClass(
        G_TYPE_CHECK_INSTANCE_CAST(obj, CppObjectClass::get_type(), GtkWidget));
    if (CppObjectClass::is_managed())
        result = manage(result);
    return result;
  }

  static void class_init_function(void *g_class, void *class_data) {
    GObjectClass *const gobject_class = static_cast<GObjectClass *>(g_class);
    gobject_class->get_property = &Glib::custom_get_property_callback;
    gobject_class->set_property = &Glib::custom_set_property_callback;

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
