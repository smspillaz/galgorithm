/*
 * /tests/galgorithm/galgorithm-binary-search-test.cpp
 *
 * Tests for the GAlgorithm binary search function
 *
 * Copyright (C) 2018 Sam Spilsbury.
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <galgorithm/galgorithm-binary-search.h>

using ::testing::Eq;
using ::testing::Not;

namespace {
  int ptr_compare (gconstpointer a, gconstpointer b)
  {
    auto cmp = reinterpret_cast <ptrdiff_t> (a) - reinterpret_cast <ptrdiff_t> (b);
    /* Avoid overflow */
    return cmp == 0 ? 0 : (cmp < 0 ? -1 : 1);
  }

  template <typename T>
  void insert_into_ptr_array (GPtrArray *array, T element)
  {
    g_ptr_array_add (array, reinterpret_cast <gpointer> (element));
  }

  template <typename T, typename... Args>
  void insert_into_ptr_array (GPtrArray *array, T element, Args&&... args)
  {
    g_ptr_array_add (array, reinterpret_cast <gpointer> (element));
    insert_into_ptr_array (array, std::forward<Args> (args)...);
  }

  TEST (GAlgorithmBinarySearch, search_empty_array) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    EXPECT_THAT (g_algorithm_binary_search (array, GINT_TO_POINTER(0), ptr_compare), Eq(-1));
  }

  TEST (GAlgorithmBinarySearch, search_one_in_array) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    insert_into_ptr_array (array, 0);

    EXPECT_THAT (g_algorithm_binary_search (array, GINT_TO_POINTER(0), ptr_compare), Eq(0));
  }

  TEST (GAlgorithmBinarySearch, search_two_in_array) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    insert_into_ptr_array (array, 0, 1);

    EXPECT_THAT (g_algorithm_binary_search (array, GINT_TO_POINTER(1), ptr_compare), Eq(1));
  }

  TEST (GAlgorithmBinarySearch, search_three_in_array) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    insert_into_ptr_array (array, 0, 1, 2);

    EXPECT_THAT (g_algorithm_binary_search (array, GINT_TO_POINTER(1), ptr_compare), Eq(1));
  }

  TEST (GAlgorithmBinarySearch, search_four_in_array) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    insert_into_ptr_array (array, 0, 1, 2, 3);

    EXPECT_THAT (g_algorithm_binary_search (array, GINT_TO_POINTER(2), ptr_compare), Eq(2));
  }
}
