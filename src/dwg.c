/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2009, 2010 Free Software Foundation, Inc.                  */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/**
 *     \file       dwg.c
 *     \brief      Main functions and API
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \author     modified by Anderson Pierre Cardoso
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)

 */

#include "config.h"
#include "logging.h"

int
dwg_read_file(char *filename, Dwg_Data *dwg_data)
{

  FILE *fp;
  struct stat attrib;
  size_t size;
  Bit_Chain bit_chain;

  if (stat(filename, &attrib))
    {
      LOG_ERROR("File not found: %s \n", filename)

      return -1;
    }
  if (!S_ISREG (attrib.st_mode))
    {
      LOG_ERROR("Error: %s \n", filename)
      return -1;
    }
  fp = fopen(filename, "rb");


  if (!fp)
    {
      LOG_ERROR("Could not open file: %s\n", filename)
      return -1;
    }

  /* Load file to memory */
  bit_chain.bit   = 0;
  bit_chain.byte  = 0;
  bit_chain.size  = attrib.st_size;
  bit_chain.chain = (unsigned char *) malloc(bit_chain.size);


  if (!bit_chain.chain)
    {
      LOG_ERROR("Not enough memory.\n")
      fclose(fp);
      return -1;
    }
  size = 0;
  size = fread(bit_chain.chain, sizeof(char), bit_chain.size, fp);

  if (size != bit_chain.size)
    {
      LOG_ERROR("Could not read the entire file (%lu out of %lu): %s \n",
                (long unsigned int) size, bit_chain.size, filename)

      fclose(fp);
      free(bit_chain.chain);
      return -1;
    }
  fclose(fp);
  memset(dwg_data, 0, sizeof(Dwg_Data));

  /* Decode the dwg structure */

  dwg_data->bit_chain = &bit_chain;

  if (dwg_decode_data(&bit_chain, dwg_data))
    {
      LOG_ERROR("Failed to decode file: %s \n", filename)

      free(bit_chain.chain);
      return -1;
    }
  free(bit_chain.chain);
  return 0;
}


/* if write support is enabled */
#ifdef USE_WRITE 

int
dwg_write_file(char *filename, Dwg_Data *dwg_data)

{
  FILE *dt;
  struct stat atrib;
  Bit_Chain bit_chain;

  bit_chain.version = (Dwg_Version_Type)dwg_data->header.version;

  /* Encode the DWG struct */
  bit_chain.size = 0;
  
  if (dwg_encode_chains(dwg_data, &bit_chain))
    {
      LOG_ERROR("Failed to encode datastructure.\n")

      if (bit_chain.size > 0)
        free (bit_chain.chain);
      return -1;
    }

  /* try opening the output file in write mode */
  if (!stat (filename, &atrib))
    {
      LOG_ERROR("The file already exists. We won't overwrite it.")
      return -1;
    }
  dt = fopen (filename, "w");

  if (!dt)
    {
      LOG_ERROR("Failed to create the file: %s\n", filename)
      return -1;
    }
  
  /* Write the data into the file */
  if (fwrite (bit_chain.chain, sizeof (char), bit_chain.size, dt)
              != bit_chain.size)
    {
      LOG_ERROR("Failed to write data into the file: %s\n", filename)
      fclose (dt);
      free (bit_chain.chain);
      return -1;
    }
  fclose (dt);

  if (bit_chain.size > 0)
    free (bit_chain.chain);

  return 0;
}


#endif /* USE_WRITE */ 

unsigned char *
dwg_bmp(Dwg_Data *stk, long int *size)
{
  char code, num_pictures;

  unsigned i;
  int plene;
  long int header_size;
  Bit_Chain *dat;

  dat = (Bit_Chain*) &stk->picture;
  dat->bit  = 0;

  dat->byte = 0;

  bit_read_RL(dat);
  num_pictures = bit_read_RC(dat);
  LOG_INFO("num_pictures: %i \n", num_pictures)


  *size = 0;
  plene = 0;
  header_size = 0;

          LOG_TRACE("\t\t BMP size: %li \n", *size)

        }
      else if (code == 3)
        {
          bit_read_RL(dat);
          LOG_TRACE("\t\t WMF size: 0x%lx \n", bit_read_RL (dat))

        }
      else
        {
          bit_read_RL(dat);
          LOG_TRACE("\t\t Size: 0x%lx \n", bit_read_RL (dat))
        }
    }
  dat->byte += header_size;
  LOG_TRACE("Current adress: 0x%lx \n", dat->byte)


  if (*size > 0)
    return (dat->chain + dat->byte);
  else
    return NULL;
}

double
dwg_model_x_min(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMIN_MSPACE.x;
}

double
dwg_model_x_max(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMAX_MSPACE.x;
}

double
dwg_model_y_min(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMIN_MSPACE.y;
}

double
dwg_model_y_max(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMAX_MSPACE.y;
}

double
dwg_model_z_min(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMIN_MSPACE.z;
}

double
dwg_model_z_max(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMAX_MSPACE.z;
}

double
dwg_page_x_min(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMIN_PSPACE.x;
}

double
dwg_page_x_max(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMAX_PSPACE.x;
}

double
dwg_page_y_min(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMIN_PSPACE.y;
}

double
dwg_page_y_max(Dwg_Data *dwg)
{
  return dwg->header_vars.EXTMAX_PSPACE.y;
}

unsigned int
dwg_get_layer_count(Dwg_Data *dwg)
{
  return dwg->layer_control->as.object->as.LAYER_CONTROL->num_entries;

}

Dwg_Object_LAYER **
dwg_get_layers(Dwg_Data *dwg)
{
  int i;

  Dwg_Object_LAYER **layers = (Dwg_Object_LAYER **) malloc(dwg_get_layer_count
                               (dwg) * sizeof (Dwg_Object_LAYER*));

/*

long unsigned int
dwg_get_object_count(Dwg_Data *dwg)
{
  return dwg->num_objects;
}
*/

/*

long unsigned int
dwg_get_object_object_count(Dwg_Data *dwg)
{
  return dwg->num_objects - dwg->num_entities;
}
*/


long unsigned int
dwg_get_entity_count(Dwg_Data *dwg)
{
  return dwg->num_entities;
}

Dwg_Object_Entity **
dwg_get_entities(Dwg_Data *dwg)
{
  long unsigned int i, ent_count = 0;

  Dwg_Object_Entity **entities = (Dwg_Object_Entity **) malloc(
                                  dwg_get_entity_count(dwg) *
                                  sizeof (Dwg_Object_Entity*));

dwg_get_entity_layer(Dwg_Object_Entity *ent)
{
  return ent->layer->obj->as.object->as.LAYER;
}

Dwg_Object *
dwg_next_object(Dwg_Object *obj)
{
  if ((obj->index + 1) > obj->parent->num_objects - 1)
    return 0;

  return &obj->parent->object[obj->index + 1];
}

int
dwg_get_object(Dwg_Object *obj, Dwg_Object_Ref *ref)

{
  if (ref->obj)
    {
      obj = ref->obj;
      return 0;
    }
  return -1;
}

Dwg_Object *
get_first_owned_object(Dwg_Object *hdr_obj, Dwg_Object_BLOCK_HEADER *hdr)
{
  unsigned int version = hdr_obj->parent->header.version;

Dwg_Object *
get_next_owned_object(Dwg_Object *hdr_obj, Dwg_Object *current,
                      Dwg_Object_BLOCK_HEADER *hdr)
{

  unsigned int version = hdr_obj->parent->header.version;

      if (current == hdr->last_entity->obj) 
        return 0;


      return dwg_next_object(current);
    }

  if (version >= R_2004)
    {
      hdr->__iterator++;

      if (hdr->__iterator == hdr->owned_object_count)
        return 0;


      return hdr->entities[hdr->__iterator]->obj;
    }
}

void
dwg_free(Dwg_Data *dwg)

{
  if (dwg->header.section)
    free(dwg->header.section);
}
