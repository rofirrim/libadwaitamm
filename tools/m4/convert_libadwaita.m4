dnl Copyright (c) 2019  Roger Ferrer <rofirrim@gmail.com>
dnl This file is part of libadwaitamm.

_CONVERSION(`AdwWindow*',`Glib::RefPtr<Window>',__RP2P)

_CONVERSION(`AdwAnimationTarget*',`Glib::RefPtr<AnimationTarget>',__RP2P)
_CONVERSION(`Glib::RefPtr<AnimationTarget>',`AdwAnimationTarget*',__FP2P)

_CONVERSION(`AdwStyleManager*',`Glib::RefPtr<StyleManager>',__RP2P)

_CONVERSION(`const Glib::RefPtr<Gtk::Application>&',`GtkApplication*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwSpringParams*',`Glib::RefPtr<SpringParams>',__RP2P)
_CONVERSION(`const Glib::RefPtr<SpringParams>&',`AdwSpringParams*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwCarousel*',`Adw::Carousel*',__RP2P)
_CONVERSION(`Adw::Carousel*',`AdwCarousel*',__FP2P)

_CONVERSION(`const Glib::RefPtr<Gtk::Expression<Glib::ustring>>&',`GtkExpression*',`(($3) ? ($3)->gobj() : nullptr)')
_CONVERSION(`GtkExpression*',`Glib::RefPtr<Gtk::Expression<Glib::ustring>>',`Glib::wrap<Glib::ustring>($3)')

_CONVERSION(`const Glib::RefPtr<Gtk::ListItemFactory>&', `GtkListItemFactory*', __CONVERT_REFPTR_TO_P) 
_CONVERSION(`GtkListItemFactory*',`Glib::RefPtr<Gtk::ListItemFactory>',`Glib::wrap($3)') 

_CONVERSION(`AdwLeafletPage*',`Glib::RefPtr<LeafletPage>',__RP2P)
_CONVERSION(`const Glib::RefPtr<LeafletPage>&',`AdwLeafletPage*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`GtkSelectionModel*',`Glib::RefPtr<Gtk::SelectionModel>',__RP2P)
_CONVERSION(`const Glib::RefPtr<Gtk::SelectionModel>&',`GtkSelectionModel*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`GtkWindow*',`Gtk::Window*',__RP2P)
_CONVERSION(`Gtk::Window*',`GtkWindow*',__FP2P)

_CONVERSION(`AdwPreferencesGroup*',`PreferencesGroup*',__RP2P)
_CONVERSION(`PreferencesGroup*',`AdwPreferencesGroup*',__FP2P)

_CONVERSION(`AdwPreferencesPage*',`PreferencesPage*',__RP2P)
_CONVERSION(`PreferencesPage*',`AdwPreferencesPage*',__FP2P)

_CONVERSION(`AdwToast*',`Toast*',__RP2P)
_CONVERSION(`Toast*',`AdwToast*',__FP2P)
_CONVERSION(`const Glib::RefPtr<Toast>&',`AdwToast*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`GtkPopover*',`Gtk::Popover*',__RP2P)
_CONVERSION(`Gtk::Popover*',`GtkPopover*',__FP2P)

_CONVERSION(`AdwSpringParams*',`Glib::RefPtr<SpringParams>',__RP2P)
_CONVERSION(`const Glib::RefPtr<SpringParams>&',`AdwSpringParams*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwAnimationTarget*',`Glib::RefPtr<AnimationTarget>',__RP2P)
_CONVERSION(`const Glib::RefPtr<AnimationTarget>&',`AdwAnimationTarget*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwSqueezerPage*',`Glib::RefPtr<SqueezerPage>',__RP2P)
_CONVERSION(`const Glib::RefPtr<SqueezerPage>&',`AdwSqueezerPage*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwSwipeable*',`Glib::RefPtr<Swipeable>',__RP2P)
_CONVERSION(`const Glib::RefPtr<Swipeable>&',`AdwSwipeable*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwTabPage*',`Glib::RefPtr<TabPage>',__RP2P)
_CONVERSION(`AdwTabPage*',`const Glib::RefPtr<TabPage>&',__RP2P)
_CONVERSION(`const Glib::RefPtr<TabPage>&',`AdwTabPage*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwTabView*',`TabView*',__RP2P)
_CONVERSION(`TabView*',`AdwTabView*',__FP2P)
_CONVERSION(`const TabView*',`AdwTabView*',__FP2P)

_CONVERSION(`AdwViewStackPage*',`Glib::RefPtr<ViewStackPage>',__RP2P)
_CONVERSION(`const Glib::RefPtr<ViewStackPage>&',`AdwViewStackPage*',__CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwViewStack*',`ViewStack*',__RP2P)
_CONVERSION(`ViewStack*',`AdwViewStack*',__FP2P)

_CONVERSION(`AdwViewStack*',`Glib::RefPtr<ViewStack>',__RP2P)
_CONVERSION(`const Glib::RefPtr<ViewStack>&',`AdwViewStack*',__CONVERT_REFPTR_TO_P($3))

_CONV_ENUM(`Adw',`AnimationState')
_CONV_ENUM(`Adw',`ColorScheme')
_CONV_ENUM(`Adw',`FlapFoldPolicy')
_CONV_ENUM(`Adw',`FlapTransitionType')
_CONV_ENUM(`Adw',`FoldThresholdPolicy')
_CONV_ENUM(`Adw',`CenteringPolicy')
_CONV_ENUM(`Adw',`NavigationDirection')
_CONV_ENUM(`Adw',`LeafletTransitionType')
_CONV_ENUM(`Adw',`ResponseAppearance')
_CONV_ENUM(`Adw',`ToastPriority')
_CONV_ENUM(`Adw',`SqueezerTransitionType')
_CONV_ENUM(`Adw',`TabViewShortcuts')
_CONV_ENUM(`Adw',`Easing')
_CONV_ENUM(`Adw',`ViewSwitcherPolicy')
