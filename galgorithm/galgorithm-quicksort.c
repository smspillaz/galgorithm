/*
 * /galgorithm/galgorithm-quicksort.c
 *
 * Implementation for GAlgorithm Quicksort. Runs in O(2N) space
 * and O(N^2) worst case time and 
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

#include <galgorithm/galgorithm-quicksort.h>

static inline void
swap (gpointer *lhs, gpointer *rhs)
{
  gpointer tmp = *lhs;
  *lhs = *rhs;
  *rhs = tmp;
}

/*
 * partition the array. @lower and @upper are inclusive bounds
 */
static int
partition (GPtrArray             *array,
           GAlgorithmCompareFunc  cmp,
           size_t                 lower,
           size_t                 upper)
{
  int pivotReplacementIdx = lower;
  gpointer pivot = array->pdata[upper];

  for (size_t i = lower; i < upper; ++i)
    {
      if (cmp (array->pdata[i], pivot) <= 0)
        {
          swap(&array->pdata[pivotReplacementIdx++], &array->pdata[i]);
        }
    }

  swap(&array->pdata[upper], &array->pdata[pivotReplacementIdx]);
  return pivotReplacementIdx;
}


/**
 * g_algorithm_quicksort:
 * @array: (element-type GObject): A #GPtrArray
 * @cmp: (scope async): A #GAlgorithmCompareFunc . @array should satisfy the ordering
 *       given by @cmp (which is to say that sorting the array using @cmp
 *       should make no difference). If unsure, use a sort function on the
 *       array first.
 *
 * Do quicksort on the array, returning a reference to the array
 *
 * Return: (transfer none) (element-type GObject): The index of the @array on success, -1 on failure.
 */
GPtrArray * g_algorithm_quicksort (GPtrArray            *array,
                                   GAlgorithmCompareFunc cmp)
{
  g_return_val_if_fail(array != NULL, NULL);
  g_return_val_if_fail(cmp != NULL, NULL);

  /* Quick check, an empty array can't be accessed */
  if (array->len <= 1)
    return array;

  /* Non-recursive quicksort, here is how it works
   *
   * 1. We maintain a stack of height at most the array size.
   * 2. Push the upper and lower bounds on to the stack.
   * 3. Do partition and get a pivot
   * 4. If the pivot was greater than our lower bound, we
   *    can push the lower part of the array on to the stack.
   * 5. If the pivot was lower than our upper bound, we can
   *    push the uppwer part of the array on to the stack
   */
  g_autoptr(GArray) stack = g_array_sized_new (FALSE, FALSE, sizeof (size_t), array->len);
  size_t *stack_data = (size_t *) stack->data;
  stack->len = array->len;
  size_t top = 0;

  stack_data[top++] = 0;
  stack_data[top++] = array->len - 1;

  while (top > 0)
    {
      size_t upper = stack_data[--top];
      size_t lower = stack_data[--top];

      size_t pivot = partition(array, cmp, lower, upper);

      /* We have now done the partition, check if we have any work
       * left to do on the lower and upper partitions */
      if (pivot > lower)
        {
          stack_data[top++] = lower;
          /* We need to subtract one here because we don't include
           * the pivot in our new bounds */
          stack_data[top++] = pivot - 1;
        }

      if (pivot + 1 < upper)
        {
          /* We need to add one here as we don't include the pivot
           * in our new bounds */
          stack_data[top++] = pivot + 1;
          stack_data[top++] = upper;
        }
    }
  
  return array;
}
