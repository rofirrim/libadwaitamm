# libadwaita

# Input: install_prefix, install_datadir, install_includedir,
#        libadwaitamm_pcname, libadwaitamm_api_version, libadwaitamm_requires,
#        install_pkgconfigdir, install_libdir, build_deprecated_api,
#        libadwaitamm_major_version, libadwaitamm_minor_version, libadwaitamm_micro_version
# Output: pkg_conf_data, install_includeconfigdir

pkg_conf_data = configuration_data()
pkg_conf_data.set('prefix', install_prefix)
pkg_conf_data.set('exec_prefix', '${prefix}')
pkg_conf_data.set('libdir', '${exec_prefix}' / install_libdir)
pkg_conf_data.set('datarootdir', '${prefix}' / install_datadir)
pkg_conf_data.set('datadir', '${datarootdir}')
pkg_conf_data.set('includedir', '${prefix}' / install_includedir)
pkg_conf_data.set('srcdir', meson.current_source_dir())
pkg_conf_data.set('PACKAGE_TARNAME', meson.project_name())
pkg_conf_data.set('PACKAGE_VERSION', meson.project_version())
pkg_conf_data.set('LIBADWAITAMM_MODULE_NAME', libadwaitamm_pcname)
pkg_conf_data.set('LIBADWAITAMM_API_VERSION', libadwaitamm_api_version)
pkg_conf_data.set('LIBADWAITAMM_MODULES', libadwaitamm_requires)

if not build_deprecated_api
  pkg_conf_data.set('LIBADWAITAMM_DISABLE_DEPRECATED', true)
endif
pkg_conf_data.set('LIBADWAITAMM_MAJOR_VERSION', libadwaitamm_major_version)
pkg_conf_data.set('LIBADWAITAMM_MINOR_VERSION', libadwaitamm_minor_version)
pkg_conf_data.set('LIBADWAITAMM_MICRO_VERSION', libadwaitamm_micro_version)

configure_file(
  input: 'libadwaitamm.pc.in',
  output: libadwaitamm_pcname + '.pc',
  configuration: pkg_conf_data,
  install_dir: install_pkgconfigdir,
)

configure_file(
  input: 'libadwaitamm-uninstalled.pc.in',
  output: libadwaitamm_pcname + '-uninstalled.pc',
  configuration: pkg_conf_data,
)

install_includeconfigdir = install_libdir / libadwaitamm_pcname / 'include'
configure_file(
  input: 'libadwaitammconfig.h.in',
  output: '@BASENAME@',
  configuration: pkg_conf_data,
  install_dir: install_includeconfigdir,
)

subdir('libadwaitamm')
