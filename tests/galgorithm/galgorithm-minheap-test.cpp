/*
 * /tests/galgorithm/galgorithm-minheap-test.cpp
 *
 * Tests for the GAlgorithm minheap structure
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

#include <galgorithm/galgorithm-minheap.h>

using ::testing::Eq;
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

  TEST (GAlgorithmMinheap, insert_into_empty) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    g_algorithm_insert_minheap (array, GINT_TO_POINTER (1), ptr_compare);
    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (1)));
  }

  TEST (GAlgorithmMinheap, insert_higher_number) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    g_algorithm_insert_minheap (array, GINT_TO_POINTER (1), ptr_compare);
    g_algorithm_insert_minheap (array, GINT_TO_POINTER (2), ptr_compare);

    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (1)));
  }

  TEST (GAlgorithmMinheap, insert_lower_number) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    g_algorithm_insert_minheap (array, GINT_TO_POINTER (2), ptr_compare);
    g_algorithm_insert_minheap (array, GINT_TO_POINTER (1), ptr_compare);

    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (1)));
  }

  TEST (GAlgorithmMinheap, insert_tree) {
    g_autoptr(GPtrArray) array = g_ptr_array_new ();

    g_algorithm_insert_minheap (array, GINT_TO_POINTER (5), ptr_compare);
    g_algorithm_insert_minheap (array, GINT_TO_POINTER (4), ptr_compare);
    g_algorithm_insert_minheap (array, GINT_TO_POINTER (3), ptr_compare);
    g_algorithm_insert_minheap (array, GINT_TO_POINTER (2), ptr_compare);
    g_algorithm_insert_minheap (array, GINT_TO_POINTER (1), ptr_compare);

    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (1)));
    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (2)));
    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (3)));
    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (4)));
    EXPECT_THAT (g_algorithm_minheap_pop (array, ptr_compare), Eq (GINT_TO_POINTER (5)));
  }
}
