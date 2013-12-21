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

  if (dwg_get_type(obj)== DWG_TYPE_REGION)
    {
      output_process(obj);
    }
}

void
low_level_process(dwg_object *obj)
{
  // casts dwg object to region entity
  dwg_ent_region *region = dwg_object_to_REGION(obj);

  // prints acis empty
  printf("acis empty of region : %d", region->acis_empty);

  // prints version
  printf("version of region : %d", region->version);

  // prints acis data
  printf("acis data of region : %s", region->acis_data);

  // prints wireframe data present 
  printf("wireframe data of region : %d", region->wireframe_data_present);

  // prints point present
  printf("point present of region : %d", region->point_present);

  // prints point values
  printf("point of region : x = %f, y = %f, z = %f\t\n", region->point.x,
          region->point.y, region->point.z);

  // prints num isolines
  printf("num isolines of region : %ld", region->num_isolines);

  // prints isoline present
  printf("isoline present of region : %d", region->isoline_present);

  // prints num wires
  printf("num wires of region : %ld", region->num_wires);

  // prints wires
  int i;
