project('Dungeon', 'c', version: '1.0.0', default_options : ['cpp_std=c++20'])

sourcefiles = files(
	'src/sources/main.c',
	'src/sources/dungeon_utils.c',
	'src/sources/dungeon.c',
	'src/sources/game.c',
	'src/sources/menu.c',
	'src/sources/room.c',
	'src/sources/state.c',
	'src/sources/utils.c',
	'src/sources/wall.c'
	)

buildargs = [
	'-DPROJECT_NAME=' + meson.project_name(),
	'-DPROJECT_VERSION=' + meson.project_version(),
]

executable(meson.project_name(), sourcefiles, install: false, cpp_args: buildargs)