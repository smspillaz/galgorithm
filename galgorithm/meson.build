# /galgorithm/meson.build
#
# Build the libgalgorithm library.
#
# Copyright (C) 2019 Sam Spilsbury.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

api_version = '0'

galgorithm_toplevel_headers = files([
  'galgorithm.h',
  'galgorithm-binary-search.h',
  'galgorithm-merge-sort.h',
  'galgorithm-quicksort.h'
])
galgorithm_introspectable_sources = files([
  'galgorithm-binary-search.c',
  'galgorithm-merge-sort.c',
  'galgorithm-quicksort.c'
])
galgorithm_private_headers = files([
])
galgorithm_private_sources = files([
])

galgorithm_headers_subdir = 'galgorithm'

install_headers(galgorithm_toplevel_headers, subdir: galgorithm_headers_subdir)

galgorithm_sources = galgorithm_introspectable_sources + galgorithm_private_sources

glib = dependency('glib-2.0')
gobject = dependency('gobject-2.0')

galgorithm_lib = shared_library(
  'galgorithm',
  galgorithm_sources,
  soversion: api_version,
  install: true,
  include_directories: [ galgorithm_inc ],
  dependencies: [
    glib,
    gobject
  ]
)

galgorithm_dep = declare_dependency(
  link_with: galgorithm_lib,
  include_directories: [ galgorithm_inc ],
)

introspection_sources = [ galgorithm_introspectable_sources, galgorithm_toplevel_headers ]

gnome = import('gnome')
gnome.generate_gir(
  galgorithm_lib,
  extra_args: ['--warn-all', '--warn-error'],
  identifier_prefix: 'GAlgorithm',
  include_directories: galgorithm_inc,
  includes: ['GLib-2.0', 'GObject-2.0'],
  install: true,
  namespace: 'GAlgorithm',
  nsversion: api_version,
  sources: introspection_sources,
  symbol_prefix: 'g_algorithm'
)

pkg = import('pkgconfig')
pkg.generate(
  description: 'Algorithms for GLib applications',
  name: 'libgalgorithm',
  filebase: 'libgalgorithm-' + api_version,
  version: meson.project_version(),
  libraries: galgorithm_lib,
  install_dir: join_paths(get_option('libdir'), 'pkgconfig')
)
