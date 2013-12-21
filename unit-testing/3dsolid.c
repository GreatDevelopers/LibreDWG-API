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

  if (dwg_get_type(obj)== DWG_TYPE_3DSOLID)
    {
      output_process(obj);
    }
}

void
low_level_process(dwg_object *obj)
{
  // casts object to 3d solid entity
  dwg_ent_3dsolid *_3dsolid = obj->tio.entity->tio._3DSOLID;

  // prints acis empty value
  printf("acis empty of 3dsolid : %d", _3dsolid->acis_empty);

  // prints version value
  printf("version of 3dsolid : %d", _3dsolid->version);

  // prints acis data value
  printf("acis data of 3dsolid : %s", _3dsolid->acis_data);

  // prints wireframe data
  printf("wireframe data of 3dsolid : %d", _3dsolid->wireframe_data_present);

  // prints point present
  printf("point present of 3dsolid : %d", _3dsolid->point_present);

  // prints point of 3dsolid
  printf("point of 3dsolid : x = %f, y = %f, z = %f\t\n", _3dsolid->point.x,
          _3dsolid->point.y, _3dsolid->point.z);

  // prints num isolines
  printf("num isolines of 3dsolid : %ld", _3dsolid->num_isolines);

  // prints isoline present
  printf("isoline present of 3dsolid : %d", _3dsolid->isoline_present);

  // prints num wires 
  printf("num wires of 3dsolid : %ld", _3dsolid->num_wires);
  int i;
