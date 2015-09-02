#include "libhistogram.h"
#include "libmemory-serie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Histogram *
histogram_new (void)
{
  return calloc (1, sizeof (Histogram));
}

void
histogram_set_serie (Histogram *histogram, Serie *serie)
{
  if (histogram == NULL || serie == NULL) return;

  /* Obtain the minimum and maximum value of the dataset. */
  histogram->minimum.x = serie->i32_MinimumValue;
  histogram->maximum.x = serie->i32_MaximumValue;

  /* Allocate memory for the histogram data. */
  int shift = abs(histogram->minimum.x);
  histogram->data_len = histogram->maximum.x + shift;
  histogram->data = calloc (sizeof (int), histogram->data_len);

  void **data = serie->data;

   if (data == NULL) return;

  /* Set the data points.  */
  unsigned int blob_size = serie->matrix.i16_x * serie->matrix.i16_y * serie->matrix.i16_z;
  unsigned int index;
  int data_index;
  for (index = 0; index < (unsigned int)blob_size; index++)
  {
    data_index = 0;

    switch (serie->data_type)
    {
      case MEMORY_TYPE_INT8    : data_index = *((unsigned char *)data) + shift; break;
      case MEMORY_TYPE_INT16   : data_index = *((short int *)data) + shift; break;
      case MEMORY_TYPE_INT32   : data_index = *((int *)data) + shift; break;
      case MEMORY_TYPE_INT64   : data_index = *((long *)data) + shift; break;
      case MEMORY_TYPE_UINT8   : data_index = *((unsigned char *)data) + shift; break;
      case MEMORY_TYPE_UINT16  : data_index = *((unsigned short int *)data) + shift; break;
      case MEMORY_TYPE_UINT32  : data_index = *((unsigned int *)data) + shift; break;
      case MEMORY_TYPE_UINT64  : data_index = *((unsigned long *)data) + shift; break;

      // TODO: Is roundf() really correct here?
      case MEMORY_TYPE_FLOAT32 : data_index = (int)(roundf(*((float *)data))) + shift; break;
      case MEMORY_TYPE_FLOAT64 : data_index = (int)(roundf(*((double *)data))) + shift; break;
      default : break;
    }    

    /* Update histogram column. */
    histogram->data[data_index] += 1;

    /* Check for maximum value. */
    if (histogram->data[data_index] > histogram->maximum.y)
      histogram->maximum.y = histogram->data[data_index];

    /* Check for minimum value. */
    if (histogram->data[data_index] < histogram->minimum.y)
      histogram->minimum.y = histogram->data[data_index];
  }

  histogram->title = calloc (1, strlen (serie->name) + 1);
  histogram->title = strcpy (histogram->title, serie->name);
}

static void
plot_background_draw (cairo_t *cr, int width, int height, int x, int y, int padding)
{
  if (cr == NULL) return;

  /*--------------------------------------------------------------------------.
   | DRAW BACKGROUND                                                          |
   '--------------------------------------------------------------------------*/
  cairo_set_source_rgba (cr, 0.5, 0.5, 0.5, 1.0);
  cairo_rectangle (cr, x, y, width, height);
  cairo_fill (cr);

  cairo_rectangle (cr,
		   x + padding + 10,
		   y + padding + 10,
		   x + width - 20 - padding * 2,
		   y + height - 20 - padding * 2);

  cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 1.0);
  cairo_fill (cr);

  /*--------------------------------------------------------------------------.
   | DRAW MARKERS                                                             |
   '--------------------------------------------------------------------------*/
  cairo_set_line_width (cr, 1);

  /* NOTE: In the cairo_move_to() and cairo_line_to() functions I added 0.5 
   * everywhere so the pixels will get rendered as sharp flat lines instead of
   * fat blurry ones. */
  
  /* Draw row stripes. */
  float row;
  float num_rows = 10;
  float row_height = (height - 20 - padding * 2) / (num_rows - 1);
  for (row = 0; row < num_rows; row++)
  {
    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_set_line_width (cr, 1);

    cairo_move_to (cr, padding + 0.5, row * row_height + padding + 10 + 0.5);
    cairo_line_to (cr, padding + 10 + 0.5, row * row_height + padding + 10 + 0.5);
    cairo_stroke (cr);

    if (row != num_rows - 1)
      cairo_set_source_rgba (cr, 0, 0, 0, 0.10);
    else
      cairo_set_line_width (cr, 2.5);

    
    cairo_move_to (cr, padding + 10 + 0.5, row * row_height + padding + 10 + 0.5);
    cairo_line_to (cr, width - 10 - padding + 0.5, row * row_height + padding + 10 + 0.5);
    cairo_stroke (cr);
  }

  /* Draw column stripes. */
  float column;
  float num_columns = 10;
  float column_width = (width - 20 - padding * 2) / (num_columns - 1);
  for (column = 0; column < num_columns; column++)
  {
    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_set_line_width (cr, 1);

    cairo_move_to (cr, column * column_width + padding + 10 + 0.5, height - padding + 0.5);
    cairo_line_to (cr, column * column_width + padding + 10 + 0.5, height - padding - 10 + 0.5);
    cairo_stroke (cr);

    if (column != 0)
      cairo_set_source_rgba (cr, 0, 0, 0, 0.10);
    else
      cairo_set_line_width (cr, 2.5);


    cairo_move_to (cr, column * column_width + padding + 10 + 0.5, height - padding - 10 + 0.5);
    cairo_line_to (cr, column * column_width + padding + 10 + 0.5, padding + 10 + 0.5);
    cairo_stroke (cr);
  }
}

void
histogram_draw (Histogram *histogram, cairo_t *cr, int width, int height, int x, int y)
{
  if (histogram == NULL || cr == NULL) return;

  /*--------------------------------------------------------------------------.
   | DRAW BACKGROUND                                                          |
   '--------------------------------------------------------------------------*/
  plot_background_draw (cr, width, height, x, y, 10);

  /*--------------------------------------------------------------------------.
   | DRAW DATA                                                                |
   '--------------------------------------------------------------------------*/

  /* ... work in progress. */
}

void
histogram_destroy (Histogram *histogram)
{
  if (histogram == NULL) return;

  free (histogram->title);
  free (histogram->data);
  free (histogram);
}
