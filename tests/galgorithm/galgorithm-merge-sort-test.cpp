/*
 * /tests/galgorithm/galgorithm-merge-sort-test.cpp
 *
 * Tests for the GAlgorithm merge sort function
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

#include <galgorithm/galgorithm-merge-sort.h>

using ::testing::ElementsAre;
using ::testing::IsEmpty;
using ::testing::Not;
using ::testing::_;

namespace {
  class PtrArrayWrapper {
    public:
      PtrArrayWrapper(GPtrArray *array) :
        array (array)
      {
      }

      gpointer * begin () const {
        return &array->pdata[0];
      }

      gpointer * end () const {
        return &array->pdata[0] + array->len;
      }

      size_t size () const {
        return array->len;
      }

      bool empty () const {
        return array->len == 0;
      }

      gpointer & operator[] (size_t x) {
        return array->pdata[x];
      }

      typedef gpointer value_type;
      typedef gpointer * const_iterator;
      typedef gpointer * iterator;

    private:
      GPtrArray *array;
  };

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

  TEST (GAlgorithmMergeSort, sort_empty_array) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    EXPECT_THAT (PtrArrayWrapper (g_algorithm_merge_sort (array,
                                                          ptr_compare)),
                 IsEmpty());
  }

  TEST (GAlgorithmMergeSort, sort_single_array) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();
    insert_into_ptr_array (array, 1);

    EXPECT_THAT (PtrArrayWrapper (g_algorithm_merge_sort (array,
                                                          ptr_compare)),
                 ElementsAre (GINT_TO_POINTER (1)));
  }

  TEST (GAlgorithmMergeSort, sort_two_elements) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();
    insert_into_ptr_array (array, 2, 1);

    EXPECT_THAT (PtrArrayWrapper (g_algorithm_merge_sort (array,
                                                          ptr_compare)),
                 ElementsAre (GINT_TO_POINTER (1),
                              GINT_TO_POINTER (2)));
  }

  TEST (GAlgorithmMergeSort, sort_five_elements) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();
    insert_into_ptr_array (array, 2, 1, 5, 4, 3);

    EXPECT_THAT (PtrArrayWrapper (g_algorithm_merge_sort (array,
                                                          ptr_compare)),
                 ElementsAre (GINT_TO_POINTER (1),
                              GINT_TO_POINTER (2),
                              GINT_TO_POINTER (3),
                              GINT_TO_POINTER (4),
                              GINT_TO_POINTER (5)));
  }
}
