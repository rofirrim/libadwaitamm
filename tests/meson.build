# tests

# input: gnome, libadwaitamm_own_dep

tests = [
# [[dir-name], exe-name, [sources]]
  [['test1'], 'test', ['main.cc']],
  [['test2'], 'test', ['main.cc']],
]

foreach ex : tests
  dir = ''
  foreach dir_part : ex[0]
    dir = dir / dir_part
  endforeach
  ex_name = (dir / ex[1]).underscorify()
  ex_sources = []
  resources = []
  foreach src : ex[2]
    if src.endswith('.gresource.xml')
      resources = gnome.compile_resources(dir.underscorify() + '_resources',
        dir / src,
        source_dir: dir
      )
    else
      ex_sources += dir / src
    endif
  endforeach

  exe_file = executable(ex_name, ex_sources, resources,
    dependencies: libadwaitamm_own_dep,
    implicit_include_directories: false,
    build_by_default: true
  )

  test(dir.underscorify(), exe_file)
endforeach
