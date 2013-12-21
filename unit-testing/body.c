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

  if (dwg_get_type(obj)== DWG_TYPE_BODY)
    {
      output_process(obj);
    }
}

void
low_level_process(dwg_object *obj)
{
  // casts dwg object to body
  dwg_ent_body *body = dwg_object_to_BODY(obj);

  // prints acis empty
  printf("acis empty of body : %d", body->acis_empty);

  // prints version
  printf("version of body : %d", body->version);

  // prints acis data
  printf("acis data of body : %s", body->acis_data);

  // prints wireframe
  printf("wireframe data of body : %d", body->wireframe_data_present);

  // prints point present
  printf("point present of body : %d", body->point_present);

  // prints point of body
  printf("point of body : x = %f, y = %f, z = %f\t\n", body->point.x,
          body->point.y, body->point.z);

  // prints num isolines
  printf("num isolines of body : %ld", body->num_isolines);

  // prints isolines present
  printf("isoline present of body : %d", body->isoline_present);

  // prints num wires of body
  printf("num wires of body : %ld", body->num_wires);

  // prints wire of body
     int i;
