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
 *     \file       load_dwg.c
 *     \brief      Load a DWG, get lines, text and circles
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Thien-Thi Nguyen
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */



          case DWG_TYPE_LINE:
            line = dwg.object[i].as.entity->as.LINE;
            add_line(line->start.x, line->end.x, line->start.y, line->end.y);
            break;
          case DWG_TYPE_CIRCLE:
            circle = dwg.object[i].as.entity->as.CIRCLE;
            add_circle(circle->center.x, circle->center.y, circle->radius);
            break;
          case DWG_TYPE_TEXT:
            text = dwg.object[i].as.entity->as.TEXT;
            add_text(text->insertion_pt.x, text->insertion_pt.y, text->text_value);
            break;

        }
    }
  dwg_free(&dwg);
  return success;
}

int
main (int argc, char *argv[])
{
  REQUIRE_INPUT_FILE_ARG (argc);
  load_dwg (argv[1]);
  return 0;
}
