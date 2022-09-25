# libadwaita/libadwaitamm

# Input: libadwaitamm_build_dep, libadwaitamm_pcname, maintainer_mode, project_source_root,
#        generate_binding, m4_files, libadwaitamm_libversion, install_includedir,
#        macos_darwin_versions, python3, can_add_dist_script, gmmproc_extra_m4_dirs
# Output: hg_ccg_basenames, extra_h_files, built_h_file_targets, built_files_root,
#         libadwaitamm_own_dep

defs_basefiles = [
  'libadwaita.defs',
  'libadwaita_methods.defs',
  'libadwaita_enums.defs',
  'libadwaita_signals.defs',
  'libadwaita_vfuncs.defs',
  'libadwaita_docs.xml',
]

defs_files = []
foreach file : defs_basefiles
  defs_files += '..' / 'src' / file
endforeach

hg_ccg_basenames = [
  'window',
  'windowtitle',
  'aboutwindow',
  'preferencesrow',
  'actionrow',
  'animation',
  'animationtarget',
  'application',
  'stylemanager',
  'applicationwindow',
]

extra_cc_files = [
]

extra_h_files = [
  'wrap_init.h',
]

install_headers('..' / 'libadwaitamm.h', subdir: libadwaitamm_pcname)
install_headers(extra_h_files, subdir: libadwaitamm_pcname / 'libadwaitamm')

untracked_libadwaitamm = 'untracked' / 'libadwaita' / 'libadwaitamm'
rel_untracked_libadwaitamm = '..' / '..' / untracked_libadwaitamm
src_untracked_libadwaitamm = project_source_root / untracked_libadwaitamm

if maintainer_mode

  # Maintainer mode. Generate .h and .cc files from .hg and .ccg files in ../src.

  # doc/reference/meson.build needs this.
  built_files_root = project_build_root
  built_h_file_targets = []

  # Force meson+ninja to generate source files before anything is compiled.
  # Compilation must depend on these targets.
  built_cc_file_targets = []

  hg_files = []
  foreach file : hg_ccg_basenames
    hg_files += '..' / 'src' / file + '.hg'
  endforeach

  # Create wrap_init.cc in project_build_root/libadwaita/libadwaitamm.
  built_cc_file_targets += custom_target('wrap_init.cc',
    input: hg_files,
    output: 'wrap_init.cc',
    command: [
      python3, generate_binding, 'generate_wrap_init',
      gmmproc_dir,
      '@OUTPUT@',
      'Adw', # namespace
      '@INPUT@',
    ],
    build_by_default: maintainer_mode,
    install: false,
  )

  # Create .h/_p.h/.cc files from .hg/.ccg files in project_build_root/libadwaita/libadwaitamm.
  foreach file : hg_ccg_basenames
    hg_file = '..' / 'src' / file + '.hg'
    ccg_file = '..' / 'src' / file + '.ccg'
    built_file_target = custom_target(file + '.cc',
      input: [hg_file, ccg_file],
      output: [file + '.stamp', file + '.cc', file + '.h'],
      command: [
        python3, generate_binding, 'gmmproc',
        gmmproc_dir,
        '@OUTPUT0@',
        file,
        meson.current_source_dir() / '..' / 'src',
        project_source_root / 'tools' / 'm4',
        gmmproc_extra_m4_dirs
      ],
      depend_files: defs_files + m4_files,
      build_by_default: maintainer_mode,
      install: false,
    )
    built_cc_file_targets += built_file_target[1]
    built_h_file_targets += built_file_target[2]
  endforeach

  extra_include_dirs = ['..']
  libadwaitamm_library = library(libadwaitamm_pcname,
    built_cc_file_targets, built_h_file_targets, extra_cc_files,
    version: libadwaitamm_libversion,
    darwin_versions: macos_darwin_versions,
    implicit_include_directories: false,
    include_directories: extra_include_dirs,
    dependencies: libadwaitamm_build_dep,
    install: true,
  )

  built_h_cc_dir = meson.current_build_dir()

else # not maintainer_mode

  # Not maintainer mode. Compile built source code files in
  # project_source_root/untracked/libadwaita/libadwaitamm.

  # doc/reference/meson.build needs these.
  built_files_root = project_source_root / 'untracked'
  built_h_file_targets = []

  # Two cases:
  # 1. The source code comes from a tarball, where the built files
  #    are stored in project_source_root/untracked.
  #    There are no built files in the build tree.
  # 2. Files have been built in the build tree. Then maintainer_mode has
  #    been changed from true to false. Files that are missing or not up to date
  #    in project_source_root/untracked are copied from the build tree.

  # Try to copy built source code files to the source tree.
  run_command(
    python3, generate_binding, 'copy_built_files',
    meson.current_build_dir(),
    src_untracked_libadwaitamm,
    hg_ccg_basenames,
    check: true,
  )

  built_cc_files = [ rel_untracked_libadwaitamm / 'wrap_init.cc' ]
  foreach file : hg_ccg_basenames
    built_cc_files += rel_untracked_libadwaitamm / file + '.cc'
  endforeach

  extra_include_dirs = [ '..', '..' / '..' / 'untracked' / 'libadwaita' ]
  libadwaitamm_library = library(libadwaitamm_pcname,
    built_cc_files, extra_cc_files,
    version: libadwaitamm_libversion,
    darwin_versions: macos_darwin_versions,
    implicit_include_directories: false,
    include_directories: extra_include_dirs,
    dependencies: libadwaitamm_build_dep,
    install: true,
  )

  built_h_cc_dir = src_untracked_libadwaitamm

endif

# Install built .h and _p.h files.
meson.add_install_script(
  python3, generate_binding, 'install_built_h_files',
  built_h_cc_dir,
  install_includedir / libadwaitamm_pcname / 'libadwaitamm', # subdir below {prefix}
  hg_ccg_basenames
)

if can_add_dist_script
  # Distribute built files.
  meson.add_dist_script(
    python3, generate_binding, 'dist_built_files',
    built_h_cc_dir,
    untracked_libadwaitamm,
    hg_ccg_basenames,
  )
endif

# This is used when building example programs and test programs.
# It's also a part of libadwaitamm_dep, when libadwaitamm is a subproject.
libadwaitamm_own_dep = declare_dependency(
  sources: built_h_file_targets,
  link_with: libadwaitamm_library,
  include_directories: extra_include_dirs,
  dependencies: libadwaitamm_build_dep
)
