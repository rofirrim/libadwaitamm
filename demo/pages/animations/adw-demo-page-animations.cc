#include "adw-demo-page-animations.h"

#include <glib/gi18n.h>

namespace Adw {

const char DemoPageAnimations::class_name[] = "AdwDemoPageAnimations";

void DemoPageAnimations::setup_template(Gtk::TemplateWidgetSetup &s) {

  s.set_resource("/org/gnome/Adwaitamm1/Demo/ui/pages/animations/"
                 "adw-demo-page-animations.ui");

  s.bind_widget("animation_preferences_stack");
  s.bind_widget("timed_animation_sample");
  s.bind_widget("timed_animation_button_box");
  s.bind_widget("timed_animation_repeat_count");
  s.bind_widget("timed_animation_reverse");
  s.bind_widget("timed_animation_alternate");
  s.bind_widget("timed_animation_duration");
  s.bind_widget("timed_animation_easing");
  s.bind_widget("spring_animation_velocity");
  s.bind_widget("spring_animation_damping");
  s.bind_widget("spring_animation_mass");
  s.bind_widget("spring_animation_stiffness");
  s.bind_widget("spring_animation_epsilon");
  s.bind_widget("spring_animation_clamp_switch");

  s.bind_callback("animations_easing_name", GCallback(animations_easing_name));
  s.bind_callback(
      "timed_animation_reset",
      Gtk::ptr_fun_to_mem_fun<&DemoPageAnimations::timed_animation_reset>());
  s.bind_callback("timed_animation_play_pause",
                  Gtk::ptr_fun_to_mem_fun<
                      &DemoPageAnimations::timed_animation_play_pause>());
  s.bind_callback(
      "timed_animation_skip",
      Gtk::ptr_fun_to_mem_fun<&DemoPageAnimations::timed_animation_skip>());
  s.bind_callback("get_play_pause_icon_name",
                  GCallback(get_play_pause_icon_name));
  s.bind_callback("timed_animation_can_reset",
                  GCallback(timed_animation_can_reset));
  s.bind_callback("timed_animation_can_skip",
                  GCallback(timed_animation_can_skip));
  s.bind_callback("notify_spring_params_change",
                  Gtk::ptr_fun_to_mem_fun<
                      &DemoPageAnimations::notify_spring_params_change>());
}

DemoPageAnimations::DemoPageAnimations(GtkWidget *obj)
    : TemplateWidgetBase(obj) {}

void DemoPageAnimations::init_widget(Gtk::TemplateWidgetInit &i) {
  // Register properties.
  p_timed_animation =
      std::make_unique<Glib::Property<Glib::RefPtr<Adw::Animation>>>(
          *this, "timed-animation");
  p_spring_animation =
      std::make_unique<Glib::Property<Glib::RefPtr<Adw::Animation>>>(
          *this, "spring-animation");

  i.init_template();

  i.bind_widget(animation_preferences_stack, "animation_preferences_stack");
  i.bind_widget(timed_animation_sample, "timed_animation_sample");
  i.bind_widget(timed_animation_button_box, "timed_animation_button_box");
  i.bind_widget(timed_animation_repeat_count, "timed_animation_repeat_count");
  i.bind_widget(timed_animation_reverse, "timed_animation_reverse");
  i.bind_widget(timed_animation_alternate, "timed_animation_alternate");
  i.bind_widget(timed_animation_duration, "timed_animation_duration");
  i.bind_widget(timed_animation_easing, "timed_animation_easing");
  i.bind_widget(spring_animation_velocity, "spring_animation_velocity");
  i.bind_widget(spring_animation_damping, "spring_animation_damping");
  i.bind_widget(spring_animation_mass, "spring_animation_mass");
  i.bind_widget(spring_animation_stiffness, "spring_animation_stiffness");
  i.bind_widget(spring_animation_epsilon, "spring_animation_epsilon");
  i.bind_widget(spring_animation_clamp_switch, "spring_animation_clamp_switch");

  auto target = std::make_shared<Adw::CallbackAnimationTarget>(sigc::bind(
      sigc::ptr_fun(timed_animation_cb), Glib::unwrap(timed_animation_sample)));
  target->reference();

  timed_animation =
      Adw::TimedAnimation::create(timed_animation_sample, 0, 1, 100, target);

  spring_animation = Adw::SpringAnimation::create(
      timed_animation_sample, 0, 1, SpringParams::create(Damping{10}, 1, 100),
      target);

  notify_spring_params_change();

  Glib::Binding::bind_property(timed_animation_repeat_count->property_value(),
                               timed_animation->property_repeat_count(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);
  Glib::Binding::bind_property(timed_animation_reverse->property_state(),
                               timed_animation->property_reverse(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);

  Glib::Binding::bind_property(timed_animation_alternate->property_state(),
                               timed_animation->property_alternate(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);
  Glib::Binding::bind_property(timed_animation_duration->property_value(),
                               timed_animation->property_duration(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);
  Glib::Binding::bind_property(timed_animation_easing->property_selected(),
                               timed_animation->property_easing(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);

  Glib::Binding::bind_property(spring_animation_velocity->property_value(),
                               spring_animation->property_initial_velocity(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);
  Glib::Binding::bind_property(spring_animation_epsilon->property_value(),
                               spring_animation->property_epsilon(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);
  Glib::Binding::bind_property(spring_animation_clamp_switch->property_active(),
                               spring_animation->property_clamp(),
                               Glib::Binding::Flags::SYNC_CREATE |
                                   Glib::Binding::Flags::BIDIRECTIONAL);

  timed_animation->set_easing(Adw::Easing::EASE_IN_OUT_CUBIC);

  p_timed_animation->set_value(timed_animation);
  p_spring_animation->set_value(spring_animation);

  Glib::RefPtr<Gtk::LayoutManager> manager = Glib::wrap(gtk_custom_layout_new(
      NULL, timed_animation_measure, timed_animation_allocate));

  timed_animation_sample->set_layout_manager(manager);
  timed_animation_button_box->set_direction(Gtk::TextDirection::LTR);
}

void DemoPageAnimations::timed_animation_reset() {
  timed_animation->reset();
  spring_animation->reset();
}

Glib::RefPtr<Adw::Animation> DemoPageAnimations::get_current_animation() {
  auto current_animation =
      animation_preferences_stack->get_visible_child_name();

  if (current_animation == "Timed") {
    return timed_animation;
  } else if (current_animation == "Spring") {
    return spring_animation;
  } else {
    g_assert_not_reached();
  }
}

void DemoPageAnimations::timed_animation_play_pause() {
  Glib::RefPtr<Adw::Animation> animation = get_current_animation();

  switch (animation->get_state()) {
  case Adw::AnimationState::IDLE:
  case Adw::AnimationState::FINISHED:
    animation->play();
    break;
  case Adw::AnimationState::PAUSED:
    animation->resume();
    break;
  case Adw::AnimationState::PLAYING:
    animation->pause();
    break;
  default:
    g_assert_not_reached();
  }
}

void DemoPageAnimations::timed_animation_skip() {
  timed_animation->skip();
  spring_animation->skip();
}

void DemoPageAnimations::notify_spring_params_change() {
  Glib::RefPtr<Adw::SpringParams> spring_params =
      Adw::SpringParams::create(Damping{spring_animation_damping->get_value()},
                                spring_animation_mass->get_value(),
                                spring_animation_stiffness->get_value());

  spring_animation->set_spring_params(spring_params);
}

char *DemoPageAnimations::animations_easing_name(AdwEnumListItem *value,
                                                 gpointer user_data) {
  switch (adw_enum_list_item_get_value(value)) {
  case ADW_LINEAR:
    return g_strdup(_("Linear"));
  case ADW_EASE_IN_QUAD:
    return g_strdup(_("Ease-in (Quadratic)"));
  case ADW_EASE_OUT_QUAD:
    return g_strdup(_("Ease-out (Quadratic)"));
  case ADW_EASE_IN_OUT_QUAD:
    return g_strdup(_("Ease-in-out (Quadratic)"));
  case ADW_EASE_IN_CUBIC:
    return g_strdup(_("Ease-in (Cubic)"));
  case ADW_EASE_OUT_CUBIC:
    return g_strdup(_("Ease-out (Cubic)"));
  case ADW_EASE_IN_OUT_CUBIC:
    return g_strdup(_("Ease-in-out (Cubic)"));
  case ADW_EASE_IN_QUART:
    return g_strdup(_("Ease-in (Quartic)"));
  case ADW_EASE_OUT_QUART:
    return g_strdup(_("Ease-out (Quartic)"));
  case ADW_EASE_IN_OUT_QUART:
    return g_strdup(_("Ease-in-out (Quartic)"));
  case ADW_EASE_IN_QUINT:
    return g_strdup(_("Ease-in (Quintic)"));
  case ADW_EASE_OUT_QUINT:
    return g_strdup(_("Ease-out (Quintic)"));
  case ADW_EASE_IN_OUT_QUINT:
    return g_strdup(_("Ease-in-out (Quintic)"));
  case ADW_EASE_IN_SINE:
    return g_strdup(_("Ease-in (Sine)"));
  case ADW_EASE_OUT_SINE:
    return g_strdup(_("Ease-out (Sine)"));
  case ADW_EASE_IN_OUT_SINE:
    return g_strdup(_("Ease-in-out (Sine)"));
  case ADW_EASE_IN_EXPO:
    return g_strdup(_("Ease-in (Exponential)"));
  case ADW_EASE_OUT_EXPO:
    return g_strdup(_("Ease-out (Exponential)"));
  case ADW_EASE_IN_OUT_EXPO:
    return g_strdup(_("Ease-in-out (Exponential)"));
  case ADW_EASE_IN_CIRC:
    return g_strdup(_("Ease-in (Circular)"));
  case ADW_EASE_OUT_CIRC:
    return g_strdup(_("Ease-out (Circular)"));
  case ADW_EASE_IN_OUT_CIRC:
    return g_strdup(_("Ease-in-out (Circular)"));
  case ADW_EASE_IN_ELASTIC:
    return g_strdup(_("Ease-in (Elastic)"));
  case ADW_EASE_OUT_ELASTIC:
    return g_strdup(_("Ease-out (Elastic)"));
  case ADW_EASE_IN_OUT_ELASTIC:
    return g_strdup(_("Ease-in-out (Elastic)"));
  case ADW_EASE_IN_BACK:
    return g_strdup(_("Ease-in (Back)"));
  case ADW_EASE_OUT_BACK:
    return g_strdup(_("Ease-out (Back)"));
  case ADW_EASE_IN_OUT_BACK:
    return g_strdup(_("Ease-in-out (Back)"));
  case ADW_EASE_IN_BOUNCE:
    return g_strdup(_("Ease-in (Bounce)"));
  case ADW_EASE_OUT_BOUNCE:
    return g_strdup(_("Ease-out (Bounce)"));
  case ADW_EASE_IN_OUT_BOUNCE:
    return g_strdup(_("Ease-in-out (Bounce)"));
  default:
    return NULL;
  }
}

void DemoPageAnimations::timed_animation_measure(
    GtkWidget *widget_, GtkOrientation orientation, int for_size, int *minimum,
    int *natural, int *minimum_baseline, int *natural_baseline) {
  Gtk::Widget *widget = Glib::wrap(widget_);

  Gtk::Widget *child = widget->get_first_child();

  if (!child)
    return;

  child->measure(Gtk::Orientation(orientation), for_size, *minimum, *natural, *minimum_baseline, *natural_baseline);
  // gtk_widget_measure(child, orientation, for_size, minimum, natural,
  //                    minimum_baseline, natural_baseline);
}

void DemoPageAnimations::timed_animation_allocate(GtkWidget *widget_, int width,
                                                  int height, int baseline) {
  Gtk::Widget *widget = Glib::wrap(widget_);
  Adw::DemoPageAnimations *self = dynamic_cast<Adw::DemoPageAnimations *>(
      widget->get_ancestor(Adw::DemoPageAnimations::get_type()));

  Gtk::Widget *child = widget->get_first_child();

  if (!child)
    return;

  Glib::RefPtr<Adw::Animation> animation = self->get_current_animation();

  double progress = animation->get_value();

  int child_width;
  // unused
  int natural;
  int minimum_baseline;
  int natural_baseline;

  child->measure(Gtk::Orientation::HORIZONTAL, -1, child_width, natural,
                 minimum_baseline, natural_baseline);

  int offset = (int)((width - child_width) * (progress - 0.5));

  graphene_point_t p = GRAPHENE_POINT_INIT((float)offset, 0);

  gtk_widget_allocate(Glib::unwrap(child), width, height, baseline,
                      gsk_transform_translate(NULL, &p));
}

char *
DemoPageAnimations::get_play_pause_icon_name(gpointer user_data,
                                             AdwAnimationState timed_state,
                                             AdwAnimationState spring_state) {
  gboolean playing = timed_state == ADW_ANIMATION_PLAYING ||
                     spring_state == ADW_ANIMATION_PLAYING;

  return g_strdup(playing ? "media-playback-pause-symbolic"
                          : "media-playback-start-symbolic");
}

gboolean
DemoPageAnimations::timed_animation_can_reset(gpointer user_data,
                                              AdwAnimationState timed_state,
                                              AdwAnimationState spring_state) {
  return timed_state != ADW_ANIMATION_IDLE ||
         spring_state != ADW_ANIMATION_IDLE;
}

gboolean
DemoPageAnimations::timed_animation_can_skip(gpointer user_data,
                                             AdwAnimationState timed_state,
                                             AdwAnimationState spring_state) {
  return timed_state != ADW_ANIMATION_FINISHED &&
         spring_state != ADW_ANIMATION_FINISHED;
}

void DemoPageAnimations::timed_animation_cb(double value, GtkWidget *self) {
  gtk_widget_queue_allocate(self);
}

} // namespace Adw
