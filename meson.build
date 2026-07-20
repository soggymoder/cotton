project('cotton', 'c',
  version : '0.1',
  default_options : ['c_std=c99', 'warning_level=2'])

sdl2 = dependency('sdl2')

src_files = [
  'src/main.c',
  'src/cotton.c',
  'src/cottonwindow.c',
  'src/interpreter.c',
]

executable('cotton', src_files, dependencies : sdl2)
