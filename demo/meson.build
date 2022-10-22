# Demo

# input: gnome, libadwaitamm_own_dep

if get_option('build-examples')

subdir('data')

demo_config_data = configuration_data()
demo_config_data.set_quoted('ADW_DEMO_VCS_TAG', '@VCS_TAG@')

demo_config_h = vcs_tag(
  input: configure_file(
    output: 'config.h.in',
    configuration: demo_config_data
  ),
  output: 'config.h'
)

adwaita_demo_resources = gnome.compile_resources(
   'adwaitamm-demo-resources',
   'adwaitamm-demo.gresources.xml',

   c_name: 'adw',
)

adwaita_demo_sources = [
  adwaita_demo_resources,

  'pages/about/adw-demo-page-about.cc',
  'pages/animations/adw-demo-page-animations.cc',
  'pages/avatar/adw-demo-page-avatar.cc',
  'pages/buttons/adw-demo-page-buttons.cc',
  'pages/carousel/adw-demo-page-carousel.cc',
  'pages/clamp/adw-demo-page-clamp.cc',
  'pages/dialogs/adw-demo-page-dialogs.cc',
  'pages/flap/adw-demo-page-flap.cc',
  'pages/flap/adw-flap-demo-window.cc',
  'pages/leaflet/adw-demo-page-leaflet.cc',
  'pages/lists/adw-demo-page-lists.cc',
  'pages/styles/adw-demo-page-styles.cc',
  'pages/styles/adw-style-demo-window.cc',
  'pages/tab-view/adw-demo-page-tab-view.cc',
  'pages/tab-view/adw-tab-view-demo-window.cc',
  'pages/toasts/adw-demo-page-toasts.cc',
  'pages/view-switcher/adw-demo-page-view-switcher.cc',
  'pages/view-switcher/adw-view-switcher-demo-window.cc',
  'pages/welcome/adw-demo-page-welcome.cc',

  'adwaitamm-demo.cc',
  'adw-demo-debug-info.cc',
  'adw-demo-preferences-window.cc',
  'adw-demo-window.cc',
  demo_config_h,
]

adwaita_demo = executable('adwaitamm-@0@-demo'.format(libadwaitamm_api_version),
  adwaita_demo_sources,
  dependencies: libadwaitamm_own_dep,
  install: true,
)

endif
