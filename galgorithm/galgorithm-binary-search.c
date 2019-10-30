/*
 * /galgorithm/galgorithm-binary-search.c
 *
 * Implementation for GAlgorithm Binary Search.
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

#include <galgorithm/galgorithm-binary-search.h>


/**
 * g_algorithm_binary_search:
 * @array: (element-type GObject): A #GPtrArray
 * @needle: An entry in @array to search for
 * @cmp: (scope async): A #GAlgorithmCompareFunc . @array should satisfy the ordering
 *       given by @cmp (which is to say that sorting the array using @cmp
 *       should make no difference). If unsure, use a sort function on the
 *       array first.
 *
 * Do a binary search on sorted data.
 *
 * Return: The index of the @array on success, -1 on failure.
 */
int64_t g_algorithm_binary_search (GPtrArray            *array,
                                   gpointer              needle,
                                   GAlgorithmCompareFunc cmp)
{
  g_return_val_if_fail(array != NULL, -1);
  g_return_val_if_fail(cmp != NULL, -1);

  if (array->len == 0) {
    return -1;
  }

  /* Now do the binary search. We assume that the array is sorted. */
  size_t floor_index = 0;
  size_t ceil_index = array->len - 1;

  while (floor_index <= ceil_index) {
    /* Check using the comparator if we're greater than
     * or less than the midpoint. If we're greater, then
     * midpoint becomes the new floor_index, otherwise
     * midpoint becomes the new ceil_index */
    size_t midpoint = floor_index + ((ceil_index - floor_index) / 2);
    int cmp_result = cmp (needle, array->pdata[midpoint]);

    if (cmp_result == 0) {
      return (int64_t) midpoint;
    }

    if (cmp_result > 0) {
      floor_index = midpoint + 1;
    } else {
      ceil_index = midpoint;
    } 
  }

  /* Didn't find anything */
  return -1;
}
