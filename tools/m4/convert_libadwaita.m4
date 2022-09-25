dnl Copyright (c) 2019  Roger Ferrer <rofirrim@gmail.com>
dnl This file is part of libadwaitamm.

_CONVERSION(`AdwWindow*',`Glib::RefPtr<Window>',__RP2P)
_CONVERSION(`AdwAnimationTarget*',`Glib::RefPtr<AnimationTarget>',__RP2P)
_CONVERSION(`Glib::RefPtr<AnimationTarget>',`AdwAnimationTarget*',__FP2P)
_CONVERSION(`AdwStyleManager*',`Glib::RefPtr<StyleManager>',__RP2P)
_CONVERSION(`const Glib::RefPtr<Gtk::Application>&',`GtkApplication*',__CONVERT_REFPTR_TO_P($3))


_CONV_ENUM(`Adw',`AnimationState')
_CONV_ENUM(`Adw',`ColorScheme')
