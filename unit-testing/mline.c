#include "common.c"

/// Output processing function
void
output_process(dwg_object *obj);

/// Checks the respective DWG entity/object type and then calls the output_process()
void
output_object(dwg_object* obj){
  if (!obj)
    {
      printf("object is NULL\n");
      return;
    }

  if (dwg_get_type(obj)== DWG_TYPE_MLINE)
    {
      output_process(obj);
    }
}

void
low_level_process(dwg_object *obj)
{
  // casting object to mline entity
  dwg_ent_mline *mline = dwg_object_to_MLINE(obj);

  // prints mline scale
  printf("scale of mline : %f\t\n",mline->scale);

  // prints mline just
  printf("just of mline : %d\t\n",mline->just);

  // prints mline extrusion
  printf("extrusion of mline : x = %f, y = %f, z = %f\t\n", 
          mline->extrusion.x, mline->extrusion.y, mline->extrusion.z);

  // prints mline base_point
  printf("base_point of mline : x = %f,y = %f,z = %f\t\n",
          mline->base_point.x, mline->base_point.y, mline->base_point.z);

  // prints number of lines
  printf("number of lines : %d\t\n", mline->num_lines);

  // prints number of verts
  printf("number of verts : %ud\t\n", mline->num_verts);

  int i = 0;
