#pragma once

// #include <adwaita.h>
// 
// G_BEGIN_DECLS
// 
// #define ADW_TYPE_DEMO_PAGE_ANIMATIONS (adw_demo_page_animations_get_type())
// 
// G_DECLARE_FINAL_TYPE (AdwDemoPageAnimations, adw_demo_page_animations, ADW, DEMO_PAGE_ANIMATIONS, AdwBin)
// 
// G_END_DECLS

#include <libadwaitamm.h>
#include <libadwaitamm/private/bin_p.h>
#include "templatewidget.h"

namespace Adw {

class DemoPageAnimations
    : public Gtk::TemplateWidget<DemoPageAnimations, Adw::Bin> {
  friend CppClassType;

protected:
  explicit DemoPageAnimations(GtkWidget *obj) : TemplateWidgetBase(obj) {}

private:
  static const char class_name[];
  static void setup_template(Gtk::TemplateWidgetSetup &s);
  void init_widget(Gtk::TemplateWidgetInit &i);

  std::unique_ptr<Glib::Property<Glib::RefPtr<Adw::Animation>>>
      p_timed_animation;
  std::unique_ptr<Glib::Property<Glib::RefPtr<Adw::Animation>>>
      p_spring_animation;

  void timed_animation_reset();
  void timed_animation_play_pause();
  void timed_animation_skip();
  void notify_spring_params_change();

  Glib::RefPtr<Adw::Animation> get_current_animation();

  static char *animations_easing_name(AdwEnumListItem *value,
                                      gpointer user_data);
  static void timed_animation_measure(GtkWidget *widget,
                                      GtkOrientation orientation, int for_size,
                                      int *minimum, int *natural,
                                      int *minimum_baseline,
                                      int *natural_baseline);
  static void timed_animation_allocate(GtkWidget *widget_, int width,
                                       int height, int baseline);
  static char *get_play_pause_icon_name(gpointer user_data,
                                        AdwAnimationState timed_state,
                                        AdwAnimationState spring_state);
  static gboolean timed_animation_can_reset(gpointer user_data,
                                            AdwAnimationState timed_state,
                                            AdwAnimationState spring_state);
  static gboolean timed_animation_can_skip(gpointer user_data,
                                           AdwAnimationState timed_state,
                                           AdwAnimationState spring_state);
  static void timed_animation_cb(double value, GtkWidget *self);

  Gtk::Stack *animation_preferences_stack;
  Glib::RefPtr<Adw::TimedAnimation> timed_animation;
  Gtk::Widget *timed_animation_sample;
  Gtk::Widget *timed_animation_button_box;
  Gtk::SpinButton *timed_animation_repeat_count;
  Gtk::Switch *timed_animation_reverse;
  Gtk::Switch *timed_animation_alternate;
  Gtk::SpinButton *timed_animation_duration;
  Adw::ComboRow *timed_animation_easing;
  Glib::RefPtr<Adw::SpringAnimation> spring_animation;
  Gtk::SpinButton *spring_animation_velocity;
  Gtk::SpinButton *spring_animation_damping;
  Gtk::SpinButton *spring_animation_mass;
  Gtk::SpinButton *spring_animation_stiffness;
  Gtk::SpinButton *spring_animation_epsilon;
  Gtk::Switch *spring_animation_clamp_switch;
};

} // namespace Adw
