/*
 * /galgorithm/galgorithm-binary-search.h
 *
 * Forward declarations for GAlgorithm Binary Search.
 *
 * Copyright (C) 2019 Sam Spilsbury.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <glib.h>
#include <stdint.h>

G_BEGIN_DECLS

typedef int (GAlgorithmCompareFunc) (gconstpointer a, gconstpointer b);

int64_t g_algorithm_binary_search (GPtrArray             *array,
                                   gpointer               needle,
                                   GAlgorithmCompareFunc  cmp);

G_END_DECLS
