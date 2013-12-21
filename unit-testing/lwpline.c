#include "common.c"

void
output_process(dwg_object *obj);

void
output_object(dwg_object* obj){
  if (!obj)
    {
      printf("object is NULL\n");
      return;
    }

  if (dwg_get_type(obj)== DWG_TYPE_LWPLINE)
    {
      output_process(obj);
    }
}

void
low_level_process(dwg_object *obj)
{
  // casts dwg object to lwpline entity
  dwg_ent_lwpline *lwpline = dwg_object_to_LWPLINE(obj);

  // prints normal points
  printf("normal of lwpline : x = %f, y = %f, z = %f\t\n",
          lwpline->normal.x, lwpline->normal.y, lwpline->normal.z);

  // prints constant width
  printf("const width of lwpline : %f\t\n", lwpline->const_width);

  // prints elevation
  printf("elevation of lwpline : %f\t\n", lwpline->elevation);

  // prints thickness
  printf("thickness of lwpline : %f\t\n", lwpline->thickness);

  // prints width number
  printf("num width of lwpline : %ld\t\n", lwpline->num_widths);

  // prints number of bulges
  printf("num bulges of lwpline : %ld\t\n", lwpline->num_bulges);

  // prints number of points
  printf("num points of lwpline : %ld\t\n", lwpline->num_points);

  // prints flag
  printf("flag of lwpline : %d\t\n", lwpline->flags);

  // prints bulges
  int i;
