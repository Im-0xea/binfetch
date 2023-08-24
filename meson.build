project('binfetch', 'c')

inc = [
  'include'
]
src = [
  'src/args.c',
  'src/bin_op.c',
  'src/color.c',
  'src/config.c',
  'src/crypto.c',
  'src/elf_parser.c',
  'src/fetch.c',
  'src/info.c',
  'src/jvm_parser.c',
  'src/mach_parser.c',
  'src/mz_parser.c',
  'src/pair.c',
  'src/pe_parser.c',
  'src/sh_parser.c',
  'src/uf2_parser.c'
]
cc = meson.get_compiler('c')
deps = [
  cc.find_library('m', required : false),
  dependency('libconfuse'),
  dependency('libelf'),
  dependency('zlib')
]

exe = executable('binfetch',
  sources : src,
  dependencies : deps,
  include_directories : inc
)

test('version_short', exe, args : [ '-V' ])
test('help_short', exe, args : [ '-h' ])
test('fail_short', exe, args : [ '-x' ], should_fail : true)
test('help_long', exe, args : [ '--help' ])
test('version_long', exe, args : [ '--version' ])
test('fail_long', exe, args : [ '--X' ], should_fail : true)

test('self_fetch', exe, args : [ exe ])
test('missing_fetch', exe, args : [ 'this_file_doesn\'t_exist' ], should_fail : true)
test('unknown_fetch', exe, args : [ '../' + src[0] ])

test('fetch elf_cutoff.bin', exe, args : [ '../tests/elf_cutoff.bin' ])
test('fetch elf_onlyheader.bin', exe, args : [ '../tests/elf_onlyheader.bin' ])

test('fetch elf_x86_64-linux-gnu_shared.bin', exe, args : [ '../tests/elf_x86_64-linux-gnu_shared.bin' ])
test('fetch elf_x86_64-linux-gnu_static.bin', exe, args : [ '../tests/elf_x86_64-linux-gnu_static.bin' ])
test('fetch elf_x86_64-linux-gnu_stripped.bin', exe, args : [ '../tests/elf_x86_64-linux-gnu_stripped.bin' ])
test('fetch elf_x86_64-linux-gnu_upx.bin', exe, args : [ '../tests/elf_x86_64-linux-gnu_upx.bin' ])
test('fetch pe_i686-mingw_shared.bin', exe, args : [ '../tests/pe_i686-mingw_shared.bin' ])
test('fetch pe_i686-mingw_static.bin', exe, args : [ '../tests/pe_i686-mingw_static.bin' ])
test('fetch pe_i686-mingw_stripped.bin', exe, args : [ '../tests/pe_i686-mingw_stripped.bin' ])
test('fetch pe_i686-mingw_upx.bin', exe, args : [ '../tests/pe_i686-mingw_upx.bin' ])
test('fetch mach_x86_64-darwin-osx_static.bin', exe, args : [ '../tests/mach_x86_64-darwin-osx_static.bin' ])
