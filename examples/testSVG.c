/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2009 Free Software Foundation, Inc.                        */
/*  Copyright (C) 2010 Thien-Thi Nguyen                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/**
 *     \file       testSVG.c
 *     \brief      Convert a DWG to SVG
 *     \author     written by Felipe Corrêa da Silva Sances
 *     \author     modified by Thien-Thi Nguyen 
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)

 */



#include "../src/bits.h"
#include "suffix.c"

double model_xmin, model_ymin;
double page_width, page_height, scale;

double transform_X(double x)
{
  return x - model_xmin;
}

double transform_Y(double y)
{

  return page_height - (y - model_ymin);
}

int
test_SVG(char *filename);

void
output_SVG(Dwg_Data *dwg);


int
main(int argc, char *argv[])
{
  REQUIRE_INPUT_FILE_ARG (argc);
  return test_SVG (argv[1]);
}

int
test_SVG(char *filename)
{
  int error;
  Dwg_Data dwg;

  error = dwg_read_file(filename, &dwg);

  if (!error)
    output_SVG(&dwg);

  dwg_free(&dwg);
  /* This value is the return value for `main', so clamp it to either 0 or 1 */

  return error ? 1 : 0;
}

void
output_TEXT(Dwg_Object *obj)
{
  Dwg_Entity_TEXT* text;
  text = obj->as.entity->as.TEXT;

  /* TODO: Juca, fix it properly: */
  if (text->text_value[0] == '&') 
    return;

    output_INSERT(obj);

  if (obj->type == DWG_TYPE_LINE)
    output_LINE(obj);

  if (obj->type == DWG_TYPE_CIRCLE)
    output_CIRCLE(obj);

  if (obj->type == DWG_TYPE_TEXT)
    output_TEXT(obj);

  if (obj->type == DWG_TYPE_ARC)
      output_ARC(obj);
}

void output_BLOCK_HEADER(Dwg_Object_Ref *ref)

{
  Dwg_Object* obj;
  Dwg_Object_BLOCK_HEADER* hdr;

  if (!ref)
    {
      fprintf(stderr, "Found null object reference. Could not output an SVG "
              "symbol for this BLOCK_HEADER \n");

      return;
    }
  if (!ref->obj)
    {
      fprintf(stderr, "Found null ref->obj \n");
      return;
    }

  /* TODO: Review - This check avoids a segfault, but it is still unclear 
     whether or not the condition is valid */
  if (!ref->obj->as.object)
    {
      fprintf(stderr, "Found null ref->obj->as.object \n");
      return;
    }
  hdr = ref->obj->as.object->as.BLOCK_HEADER;

  double dx      = (dwg_model_x_max(dwg) - dwg_model_x_min(dwg));
  double dy      = (dwg_model_y_max(dwg) - dwg_model_y_min(dwg));
  double scale_x = dx / (dwg_page_x_max(dwg) - dwg_page_x_min(dwg));
  double scale_y = dy / (dwg_page_y_max(dwg) - dwg_page_y_min(dwg));


  //scale = 25.4 / 72; // pt:mm
  page_width = dx;
  page_height = dy;
  //scale *= (scale_x > scale_y ? scale_x : scale_y);

  block_control = obj->as.object->as.BLOCK_CONTROL;
