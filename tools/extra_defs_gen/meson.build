# tools/extra_defs_gen

# Input: cpp_compiler, libadwaitamm_build_dep, install_libdir, libadwaitamm_pcname,
#        glibmm_dep
# Output: m4_files, install_m4dir

glibmm_generate_extra_defs_dep = cpp_compiler.find_library(
  'glibmm_generate_extra_defs-2.68',
# or:  'glibmm_generate_extra_defs-2.4',
  required: glibmm_dep.type_name() != 'internal',
)
if not glibmm_generate_extra_defs_dep.found()
  glibmm_generate_extra_defs_dep = dependency(
    '', fallback: ['glibmm-2.68', 'glibmm_generate_extra_defs_dep'],
# or:    '', fallback: ['glibmm-2.4', 'glibmm_generate_extra_defs_dep'],
  )
endif

executable('generate_defs_libadwaita', 'generate_defs_libadwaita.cc',
  dependencies: [libadwaitamm_build_dep, glibmm_generate_extra_defs_dep],
  implicit_include_directories: false,
  install: false,
)

# Install m4 files for reuse by other *mm projects, when building from git.
m4_basefiles = [
  'convert.m4',
  'convert_libadwaita.m4',
]
m4_files = []
foreach file : m4_basefiles
  m4_files += '..' / 'm4' / file
endforeach
m4_files = files(m4_files)
install_m4dir = install_libdir / libadwaitamm_pcname / 'proc' / 'm4'
install_data(m4_files, install_dir: install_m4dir)
