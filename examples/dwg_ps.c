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
 *     \file       dwg_ps.c
 *     \brief      Create a PostScript file from a DWG
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Thien-Thi Nguyen
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)

 */



#include "suffix.c"

void
create_postscript(Dwg_Data *dwg, char *output)
{
  float dx, dy, scale, scale_x, scale_y;

  long unsigned i;
  FILE *fh;
  PSDoc *ps;

  /* Initialization */
  PS_boot();
  ps = PS_new();


  PS_set_info(ps, "Creator",  "dwg_ps");
  PS_set_info(ps, "Author",   "LibreDWG example");
  PS_set_info(ps, "Title",    "DWG to Postscript example");
  PS_set_info(ps, "Keywords", "dwg, postscript, conversion, CAD, plot");

  /* First page: Model Space (?) */
  dx = (dwg_model_x_max(dwg) - dwg_model_x_min(dwg));
  dy = (dwg_model_y_max(dwg) - dwg_model_y_min(dwg));

  scale_x = dx / (dwg_model_x_max(dwg) - dwg_model_x_min(dwg));
  scale_y = dy / (dwg_model_y_max(dwg) - dwg_model_y_min(dwg));
  scale   = 25.4 / 72;   // pt:mm

  PS_begin_page(ps, dx / scale, dy / scale);
  scale *= (scale_x > scale_y ? scale_x : scale_y);


  PS_scale(ps, scale, scale);
  PS_translate(ps, -dwg_model_x_min(dwg), -dwg_model_y_min(dwg));
  //printf ("%f (%f, %f)\n", scale, scale_x, scale_y);

  /* Mark the origin with a crossed circle */
  #define H 2000


  PS_circle(ps, 0, 0, H);
  PS_moveto(ps, 0, H);
  PS_lineto(ps, 0, -H);
  PS_moveto(ps, -H, 0);
  PS_lineto(ps, H, 0);
  PS_stroke(ps);

  /* Iterate all entities */
  Dwg_Object *obj;

          line = obj->as.entity->as.LINE;
          PS_moveto(ps, line->start.x, line->start.y);
          PS_lineto(ps, line->end.x,   line->end.y);

          PS_stroke(ps);
        }
    }

  /* End Model Space */
  PS_end_page(ps);

  PS_close(ps);
  PS_delete(ps);
  PS_shutdown();
}

int
main(int argc, char *argv[])
{
  int success;
  char *outfile;
  Dwg_Data dwg;

  REQUIRE_INPUT_FILE_ARG (argc);
  success = dwg_read_file(argv[1], &dwg);


  if (success)
    {
      puts("Not able to read dwg file!");
      return 1;
    }

  outfile = suffix (argv[1], "ps");
  create_postscript(&dwg, outfile);
  dwg_free(&dwg);

  printf ("Success! See the file '%s'\n", outfile);
  free (outfile);
  return 0;
}

