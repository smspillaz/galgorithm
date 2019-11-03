/*
 * /galgorithm/galgorithm-minheap.c
 *
 * Implementation for GAlgorithm Minheap.
 *
 * Runs inserts in O(log n).
 *
 * Runs deletes in O(log n).
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

#include <galgorithm/galgorithm-minheap.h>

/* Grow the minheap so that it can fit an element at
 * position */
static void
minheap_grow (GPtrArray *array, size_t position)
{
  size_t parent = position > 1 ? (position / 2) : 1;
  size_t last_child = 2 * parent + 1;

  /* Size must be at least all the children from this parent */
  if (array->len < last_child + 1)
    g_ptr_array_set_size (array, last_child + 1);
}

static inline void
swap (gpointer *lhs, gpointer *rhs)
{
  gpointer tmp = *lhs;
  *lhs = *rhs;
  *rhs = tmp;
}

/**
 * g_algorithm_insert_minheap:
 * @array: (element-type GObject): A #GPtrArray.
 * @candidate: A #gpointer to insert.
 * @cmp: (scope call): A #GAlgorithmCompareFunc to compare two elements.
 *
 * Insert an element into the minheap, growing if necessary.
 */
void
g_algorithm_insert_minheap (GPtrArray             *array,
                            gpointer               candidate,
                            GAlgorithmCompareFunc  cmp)
{
  /* Ensure we have a length of at least one */
  minheap_grow (array, 1);

  size_t insert_position = 0;

  /* Start at the first available node */
  for (int i = 1; i < array->len; ++i)
    {
      /* Insert the child at that position */
      if (array->pdata[i] == NULL) {
        array->pdata[i] = candidate;
        insert_position = i;
        break;
      }
    }

  /* We didn't find a node to insert into! */
  if (insert_position == 0)
    {
      /* Grow big enough that we can fit the children
       * of the last child. */
      insert_position = array->len;
      minheap_grow (array, array->len);
      array->pdata[insert_position] = candidate;
    }

  /* Now, we need to reheapify from that position */
  size_t i = insert_position;

  while (i != 1)
    {
      size_t parent = i / 2;

      /* Parent is greater than us, we need to swap */
      if (cmp (array->pdata[parent], array->pdata[i]) > 0)
        swap (&array->pdata[parent], &array->pdata[i]);
      else
        break;

      i = parent;
    }
}

/**
 * g_algorithm_minheap_pop:
 * @array: (element-type GObject): A #GPtrArray.
 * @cmp: (scope call): A #GAlgorithmCompareFunc to compare two elements.
 *
 * Pop an element from the min-heap.
 *
 * Returns: (transfer none): The minimum element.
 */
gpointer
g_algorithm_minheap_pop (GPtrArray             *array,
                         GAlgorithmCompareFunc  cmp)
{
  /* Ensure we have a length of at least one */
  minheap_grow (array, 1);

  /* Nothing to delete */
  if (array->pdata[1] == NULL)
    {
      return NULL;
    }

  gpointer root = array->pdata[1];
  array->pdata[1] = NULL;

  /* Find the first non-null leaf node starting from the end
   * of the tree */
  for (int i = array->len - 1; i >= 1; --i)
    {
      if (array->pdata[i] != NULL)
        {
          swap (&array->pdata[i], &array->pdata[1]);
          break;
        }
    }

  /* OK, now that we've done that, we need to percolate the
   * root node down the tree */
  size_t i = 1;

  while (i < array->len && array->pdata[i] != NULL)
    {
      size_t child = i * 2;

      if (array->pdata[child] != NULL &&
          cmp (array->pdata[child], array->pdata[i]) < 0)
        {
          swap (&array->pdata[child], &array->pdata[i]);
          break;
        }
      
      i = child;
    }

  return root;
}