project('cotton', 'c',
  version : '0.1',
  default_options : ['c_std=c99', 'warning_level=2'])

src_files = [
  'src/main.c',
  'src/cotton.c',
  'src/interpreter.c',
  'src/cogs.c',
]

install_man('docs/cotton.1', 'docs/cot.5')

executable('cotton', src_files, install: true,)
