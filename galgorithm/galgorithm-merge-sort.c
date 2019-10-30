/*
 * /galgorithm/galgorithm-merge-sort.c
 *
 * Implementation for GAlgorithm Merge Sort. Runs in O(2N) space
 * and O(n log n) time.
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

#include <glib.h>

#include <galgorithm/galgorithm-merge-sort.h>

static inline size_t min (size_t a, size_t b) {
  return a < b ? a : b;
}

static inline unsigned int nearest_greater_power_of_two (size_t length) {
  unsigned int shift = 0;
  
  // Not the most efficient way to do this
  while ((1 << (shift++)) < length);
  
  return shift;
}

/**
 * g_algorithm_merge_sort:
 * @array: (element-type GObject): A #GPtrArray
 * @cmp: (scope async): A #GAlgorithmCompareFunc . @array should satisfy the ordering
 *       given by @cmp (which is to say that sorting the array using @cmp
 *       should make no difference). If unsure, use a sort function on the
 *       array first.
 *
 * Do a merge sort on @array, returning a reference to @array (for composability).
 * @array will be sorted in-place.
 *
 * Return: (transfer none) (element-type GObject): The index of the @array on success, -1 on failure.
 */
GPtrArray * g_algorithm_merge_sort (GPtrArray            *array,
                                    GAlgorithmCompareFunc cmp)
{
  g_return_val_if_fail(array != NULL, NULL);
  g_return_val_if_fail(cmp != NULL, NULL);

  /* We first create another pointer array of the same size. */
  g_autoptr(GPtrArray) buf = g_ptr_array_sized_new (array->len);
  buf->len = array->len;

  /* Do the first merge iteration copying from the input
   * buffer to to the output buffer */
  for (size_t i = 0; i < (array->len / 2) * 2; i += 2) {
    int res = cmp (array->pdata[i], array->pdata[i + 1]);
    buf->pdata[i] = (res > 0) ? array->pdata[i + 1] : array->pdata[i];
    buf->pdata[i + 1] = (res > 0) ? array->pdata [i] : array->pdata[i + 1];
  }
  
  /* Handle the remaining element. */
  if (array->len % 2 != 0) {
    buf->pdata[array->len - 1] = array->pdata[array->len - 1];
  }
  
  size_t comparisons = nearest_greater_power_of_two(array->len) - 1;
  size_t window = 2;
  
  GPtrArray *input = buf;
  GPtrArray *output = array;
  
  while (comparisons--) {
    window <<= 1;
    
    for (size_t i = 0; i < ((array->len / window) + 1); ++i) {
      /* Divide into two windows */
      size_t window_one_start = i * window;
      size_t window_two_start = window_one_start + (window / 2);
      
      size_t j = window_one_start;
      size_t k = window_two_start;
      size_t p = window_one_start;
      
      while (j < min(window_two_start, array->len) &&
             k < min(window_one_start + window, array->len)) {
        int res = cmp(input->pdata[j], input->pdata[k]);
        output->pdata[p++] = (res > 0) ? input->pdata[k++] : input->pdata[j++];
      }
      
      while (j < min(window_two_start, array->len)) {
        output->pdata[p++] = input->pdata[j++];
      }
      
      /* Now, one of j or k will be exhausted, fill the rest
       * of the array from the array that remains */
      while (k < min(window_one_start + window, array->len)) {
        output->pdata[p++] = input->pdata[k++];
      }
    }
    
    /* Buffer swap */
    GPtrArray *tmp = input;
    input = output;
    output = tmp;
  }

  /* Now, quickly check if our output buffer was the same as @array,
   * if not, we'll need to do one more copy to ensure that the sorted elements
   * end up in @array */
  if (output != array) {
    for (size_t i = 0; i < array->len; ++i) {
      array->pdata[i] = output->pdata[i];
    }
  }

  /* buf gets released here */
  return array;
}
