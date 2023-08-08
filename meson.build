project('binfetch', 'c')

inc = [
  'include'
]
src = [
  'src/args.c',
  'src/binfetch.c',
  'src/bin_op.c',
  'src/color.c',
  'src/config.c',
  'src/crypto.c',
  'src/elf_parser.c',
  'src/info.c',
  'src/jvm.c',
  'src/mach.c',
  'src/mz.c',
  'src/pair.c',
  'src/pe.c',
  'src/sh.c',
  'src/uf2.c'
]
cc = meson.get_compiler('c')
deps = [
  cc.find_library('m', required : false),
  dependency('libelf'),
  dependency('zlib')
]

executable('binfetch',
  sources : src,
  dependencies : deps,
  include_directories : inc
)
