/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2009 Free Software Foundation, Inc.                        */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */

  /*-------------------------------------------------------------------------
   * Unknown section 1
   */

  if (dwg->header.num_sections == 6)
    {
      LOG_TRACE("\n=======> UNKNOWN 1: %8X\n",
              (unsigned int) dwg->header.section[5].address)
      LOG_TRACE("         UNKNOWN 1 (end): %8X\n",
              (unsigned int) (dwg->header.section[5].address
                  + dwg->header.section[5].size))
      dat->byte = dwg->header.section[5].address;
      dwg->unknown1.size = DWG_UNKNOWN1_SIZE;
      dwg->unknown1.byte = dwg->unknown1.bit = 0;
      dwg->unknown1.chain = (unsigned char*)malloc(dwg->unknown1.size);
      memcpy(dwg->unknown1.chain, &dat->chain[dat->byte], dwg->unknown1.size);
      //bit_explore_chain ((Bit_Chain *) &dwg->unknown1, dwg->unknown1.size);
      //bit_print ((Bit_Chain *) &dwg->unknown1, dwg->unknown1.size);
    }

  /*-------------------------------------------------------------------------
   * Picture (Pre-R13C3?)
   */

  if (bit_search_sentinel(dat, dwg_sentinel(DWG_SENTINEL_PICTURE_BEGIN)))
    {
      unsigned long int start_address;

      dat->bit = 0;
      start_address = dat->byte;
      LOG_TRACE("\n=======> PICTURE: %8X\n",
            (unsigned int) start_address - 16)
      if (bit_search_sentinel(dat, dwg_sentinel(DWG_SENTINEL_PICTURE_END)))
        {
          LOG_TRACE("         PICTURE (end): %8X\n",
                (unsigned int) dat->byte)
          dwg->picture.size = (dat->byte - 16) - start_address;
          dwg->picture.chain = (unsigned char *) malloc(dwg->picture.size);
          memcpy(dwg->picture.chain, &dat->chain[start_address],
              dwg->picture.size);
        }
      else
        dwg->picture.size = 0;
    }

  /*-------------------------------------------------------------------------
   * Header Variables
   */

  LOG_INFO("\n=======> Header Variables: %8X\n",
          (unsigned int) dwg->header.section[0].address)
  LOG_INFO("         Header Variables (end): %8X\n",
          (unsigned int) (dwg->header.section[0].address
              + dwg->header.section[0].size))
  dat->byte = dwg->header.section[0].address + 16;
  pvz = bit_read_RL(dat);
  LOG_TRACE("         Length: %lu\n", pvz)

  dat->bit = 0;

  dwg_decode_header_variables(dat, dwg);

  // Check CRC-on
  dat->byte = dwg->header.section[0].address + dwg->header.section[0].size - 18;
  dat->bit = 0;

  ckr = bit_read_RS(dat);
  ckr2 = bit_ckr8(0xc0c1, dat->chain + dwg->header.section[0].address + 16,
          dwg->header.section[0].size - 34);

  if (ckr != ckr2)
    {
      printf("section %d crc todo ckr:%x ckr2:%x\n",
              dwg->header.section[0].number, ckr, ckr2);
      return -1;
    }

  /*-------------------------------------------------------------------------
   * Classes
   */
  LOG_INFO("\n=======> CLASS: %8X\n",
          (unsigned int) dwg->header.section[1].address)
  LOG_INFO("         CLASS (end): %8X\n",
          (unsigned int) (dwg->header.section[1].address
              + dwg->header.section[1].size))
  dat->byte = dwg->header.section[1].address + 16;
  dat->bit = 0;

  size = bit_read_RL(dat);
  lasta = dat->byte + size;
  LOG_TRACE("         Length: %lu\n", size);

  /* read the classes
   */
  dwg->dwg_ot_layout = 0;
  dwg->num_classes = 0;
  i = 0;
  do
    {
      unsigned int idc;

      idc = dwg->num_classes;
      if (idc == 0)
        dwg->dwg_class = (Dwg_Class *) malloc(sizeof(Dwg_Class));
      else
        dwg->dwg_class = (Dwg_Class *) realloc(dwg->dwg_class, (idc + 1)
            * sizeof(Dwg_Class));

      dwg->dwg_class[idc].number = bit_read_BS(dat);
      dwg->dwg_class[idc].version = bit_read_BS(dat);
      dwg->dwg_class[idc].appname = bit_read_TV(dat);
      dwg->dwg_class[idc].cppname = bit_read_TV(dat);
      dwg->dwg_class[idc].dxfname = bit_read_TV(dat);
      dwg->dwg_class[idc].wasazombie = bit_read_B(dat);
      dwg->dwg_class[idc].item_class_id = bit_read_BS(dat);

      if (strcmp((const char *)dwg->dwg_class[idc].dxfname, "LAYOUT") == 0)
        dwg->dwg_ot_layout = dwg->dwg_class[idc].number;

      dwg->num_classes++;
      /*
      if (dwg->num_classes > 100)
	{
	  fprintf(stderr, "number of classes is greater than 100. TODO: Why should we stop here?\n");
	  break;//TODO: Why?!
	}
      */
    }
  dat->byte = object_end;
  object_begin = bit_read_MS(dat);
  LOG_INFO("         Object Data (end): %8X\n",
      (unsigned int) (object_end + object_begin+ 2))

  /*
   dat->byte = dwg->header.section[2].address - 2;
   antckr = bit_read_CRC (dat); // Unknown bitdouble inter object data and object map
   LOG_TRACE("Address: %08X / Content: 0x%04X", dat->byte - 2, antckr)

   // check CRC-on
   antckr = 0xC0C1;
   do
   {
   duabyte = dat->byte;
   sgdc[0] = bit_read_RC (dat);
   sgdc[1] = bit_read_RC (dat);
          (unsigned int) dwg->header.section[2].address)
  LOG_INFO("         Object Map (end): %8X\n",
          (unsigned int) (dwg->header.section[2].address
              + dwg->header.section[2].size))

  /*-------------------------------------------------------------------------
   * Second header
   */

  if (bit_search_sentinel(dat, dwg_sentinel(DWG_SENTINEL_SECOND_HEADER_BEGIN)))
    {
      long unsigned int pvzadr;
      long unsigned int pvz;
      unsigned char sig, sig2;

      LOG_INFO("\n=======> Second Header: %8X\n",
          (unsigned int) dat->byte-16)
      pvzadr = dat->byte;

      pvz = bit_read_RL(dat);
      //LOG_TRACE("Size: %lu\n", pvz)

      pvz = bit_read_BL(dat);
      //LOG_TRACE("Begin address: %8X\n", pvz)

      //LOG_TRACE("AC1015?: ")
          (unsigned int) dwg->header.section[4].address)
  LOG_INFO("         Unknown 2 (end): %8X\n",
          (unsigned int) (dwg->header.section[4].address
              + dwg->header.section[4].size))
  dat->byte = dwg->header.section[4].address;
  dat->bit = 0;
  dwg->measurement = bit_read_RL(dat);

  LOG_TRACE("         Size bytes :\t%lu\n", dat->size)

  //step II of handles parsing: resolve pointers from handle value
  //XXX: move this somewhere else
  LOG_TRACE("\n\nResolving pointers from ObjectRef vector.\n")
  resolve_objectref_vector(dwg);
  LOG_TRACE("\n")

  return 0;
}

static void
resolve_objectref_vector(Dwg_Data * dwg)
{
  long unsigned int i;
  Dwg_Object * obj;
      LOG_TRACE("-objref: HANDLE(%d.%d.%lu) Absolute:%lu\n",
          dwg->object_ref[i]->handleref.code,
          dwg->object_ref[i]->handleref.size,
          dwg->object_ref[i]->handleref.value,
          dwg->object_ref[i]->absolute_ref)

      //look for object
      obj = dwg_resolve_handle(dwg, dwg->object_ref[i]->absolute_ref);

      if(obj)
        {
          LOG_TRACE("-found:  HANDLE(%d.%d.%lu)\n",
              obj->handle.code,
              obj->handle.size,
              obj->handle.value)
        }

      //assign found pointer to objectref vector
      dwg->object_ref[i]->obj = obj;


      if (DWG_LOGLEVEL >= DWG_LOGLEVEL_INSANE)
        {
          if (obj)
            dwg_print_object(obj);
          else
            LOG_ERROR("Null object pointer: object_ref[%lu]\n", i)
        }
    }
}

/* R2004 Literal Length
 */
static int
read_literal_length(Bit_Chain* dat, unsigned char *opcode)
{
  int total = 0;
  unsigned char byte = bit_read_RC(dat);

  *opcode = 0x00;

/**
 *     \file       decode.c
 *     \brief      Decoding functions
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \author     modified by Till Heuschmann
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#include "decode.h"
#include "logging.h"
#include "decode_r13_r15.h"
#include "decode_r2004.h"
#include "decode_r2007.h"


/* The logging level for the read (decode) path. */
static unsigned int loglevel;

#ifdef USE_TRACING

/* This flag means we have checked the environment variable
 *  LIBREDWG_TRACE and set `loglevel' appropriately.
 */
static bool env_var_checked_p;

#define DWG_LOGLEVEL loglevel
#endif  // USE_TRACING

#define REFS_PER_REALLOC 100

/** Decode DWG file */
int
dwg_decode_data(Bit_Chain *dat, Dwg_Data *dwg)
{
  char version[7];

  dwg->num_object_refs = 0;
  dwg->num_layers      = 0;
  dwg->num_entities    = 0;
  dwg->num_objects     = 0;
  dwg->num_classes     = 0;

  #ifdef USE_TRACING

  /* Before starting, set the logging level, but only do so once.  */
  if (! env_var_checked_p)
    {
      char *probe = getenv ("LIBREDWG_TRACE");

      if (probe)
        loglevel = atoi (probe);

      env_var_checked_p = true;
    }
  #endif  /* USE_TRACING */

  /* Version */
  dat->byte = 0;
  dat->bit  = 0;

  strncpy(version, (const char *)dat->chain, 6);
  version[6] = '\0';
  dwg->header.version = 0;

  if (!strcmp(version, version_codes[R_13]))
    dwg->header.version = R_13;

  if (!strcmp(version, version_codes[R_14]))
    dwg->header.version = R_14;

  if (!strcmp(version, version_codes[R_2000]))
    dwg->header.version = R_2000;

  if (!strcmp(version, version_codes[R_2004]))
    dwg->header.version = R_2004;

  if (!strcmp(version, version_codes[R_2007]))
    dwg->header.version = R_2007;

  if (dwg->header.version == 0)
    {
      LOG_ERROR("Invalid or unimplemented version code! "
                 "This file's version code is: %s\n", version)
      return -1;
    }
  dat->version = (Dwg_Version_Type) dwg->header.version;
  LOG_INFO("This file's version code is: %s\n", version)

  PRE(R_2000)
    {
      LOG_INFO("WARNING: This version of LibreDWG is only capable of safely "
               "decoding version R2000 (code: AC1015) dwg-files.\nThis "
               "file's version code is: %s Support for this version is still "
               "experimental. It might crash or give you invalid output.\n",
                version)
      return decode_R13_R15(dat, dwg);
    }
  VERSION(R_2000)
    {
      return decode_R13_R15(dat, dwg);
    }
  VERSION(R_2004)
    {
      LOG_INFO("WARNING: This version of LibreDWG is only capable of properly "
               "decoding version R2000 (code: AC1015) dwg-files.\nThis file's "
               "version code is: %s\n This version is not yet actively "
               "developed. It will probably crash and/or give you invalid "
               "output.\n", version)
      return decode_R2004(dat, dwg);
    }
  VERSION(R_2007)
    {
      LOG_INFO("WARNING: This version of LibreDWG is only capable of properly "
               "decoding version R2000 (code: AC1015) dwg-files.\nThis file's "
               "version code is: %s\n This version is not yet actively "
               "developed. It will probably crash and/or give you invalid "
               "output.\n", version)
      return decode_R2007(dat, dwg);
    }

  // This line should not be reached!
  LOG_ERROR("ERROR: LibreDWG does not support this version: %s.\n", version)
  return -1;
}

/** Decode DWG entity */
int
dwg_decode_entity(Bit_Chain *dat, Dwg_Object_Entity *ent)
{
  int error = 2;
  unsigned int i, size; 

  SINCE(R_2000)
    {
      dat->handles_address += bit_read_RL(dat);
    }
  error = bit_read_H(dat, &(ent->object->handle));

  if (error)
    {
      LOG_ERROR("dwg_decode_entity: \t Error in object handle! Current "
                "Bit_Chain address: 0x%0x \n", (unsigned int)dat->byte)
      ent->extended_size  = 0;
      ent->picture_exists = 0;
      ent->num_handles    = 0;
      return 0;
    }
  ent->extended_size = 0;

  while (size = bit_read_BS(dat))
    {
      LOG_TRACE("EED size: %lu \n", (long unsigned int) size)

      if (size > 10210)
        {
          LOG_ERROR("dwg_decode_entity: Absurd! Extended object data size: "
                    "%lu Object: %lu (handle)\n", (long unsigned int) size,
                    ent->object->handle.value)
          ent->extended_size  = 0;
          ent->picture_exists = 0;
          ent->num_handles    = 0;

          //XXX
          return -1;
          //break;
        }
      if (ent->extended_size == 0)
        {
          ent->extended      = (char *) malloc(size);

          ent->extended_size = size;
        }
      else
        {
          ent->extended_size += size;
          ent->extended      = (char *)realloc(ent->extended,
                                          ent->extended_size);
        }
      error = bit_read_H(dat, &ent->extended_handle);

      if (error)
        LOG_ERROR("Error reading extended handle! \n");



  if (ent->picture_exists)
    {
      ent->picture_size = bit_read_RL(dat);

          LOG_ERROR("dwg_decode_entity: Absurd! Picture-size: %lu kB Object: "
                    "%lu (handle) \n", ent->picture_size / 1000,
                    ent->object->handle.value)
          bit_advance_position(dat, -(4 * 8 + 1));
        }
    }
  VERSIONS(R_13,R_14)
    {
      dat->handles_address += bit_read_RL(dat);
    }
  ent->entity_mode  = bit_read_BB(dat);

  ent->num_reactors = bit_read_BL(dat);

  SINCE(R_2004)
    {
      ent->xdic_missing_flag = bit_read_B(dat);
    }
  VERSIONS(R_13, R_14)
    {
      ent->isbylayerlt = bit_read_B(dat);
    }

  ent->nolinks = bit_read_B(dat);

  SINCE(R_2004)
    {
      if (ent->nolinks == 0)
        {
          char color_mode = 0;
  
          color_mode = bit_read_B(dat);
        
          if (color_mode == 1)
            {
              //unsigned char index;

              //index = bit_read_RC(dat);  // color index
            }
          else
            {              
              unsigned int flags;


              flags = bit_read_RS(dat);
            
              if (flags & 0x8000)
                {
                  /*unsigned char c1, c2, c3, c4;

                  char *name=0;
              
                  c1 = bit_read_RC(dat);  // rgb color
                  c2 = bit_read_RC(dat);
                  c3 = bit_read_RC(dat);
                  c4 = bit_read_RC(dat);
                  name = bit_read_T(dat);*/
                }

              if (flags & 0x4000)
                flags = flags;   // has AcDbColor reference (handle)
            
              if (flags & 0x2000)
                {
                  //int transparency;

                  //transparency = bit_read_BL(dat);

                }
            }
        }
      else
        {
          //char color;

          //color = bit_read_B(dat);
        }
    }
  OTHER_VERSIONS
    bit_read_CMC(dat, &ent->color, NULL);


  ent->linetype_scale = bit_read_BD(dat);

  SINCE(R_2000)
    {
      ent->linetype_flags  = bit_read_BB(dat);
      ent->plotstyle_flags = bit_read_BB(dat);
    }
  SINCE(R_2007)
    {
      ent->material_flags = bit_read_BB(dat);
      ent->shadow_flags   = bit_read_RC(dat);
    }

  ent->invisible = bit_read_BS(dat);

  SINCE(R_2000)
    {
      ent->lineweight = bit_read_RC(dat);
    }
  return 0;
}

/** Decode DWG Object */
int
dwg_decode_object(Bit_Chain *dat, Dwg_Object_Object *ord)
{
  unsigned int i, size;

  int error = 2;

  SINCE(R_2000)
    {
      dat->handles_address += bit_read_RL(dat);
    }
  error = bit_read_H(dat, &ord->object->handle);

  if (error)
    {
      LOG_ERROR("\t Error in object handle! Bit_Chain current address: 0x%0x "
                "\n", (unsigned int) dat->byte)
      ord->extended_size = 0;
      ord->num_handles   = 0;
      return -1;
    }
  ord->extended_size = 0;


  while (size = bit_read_BS(dat))
    {
      if (size > 10210)
        {
          LOG_ERROR("dwg_decode_object: Absurd! Extended object data size: "
                    "%lu Object: %lu (handle) \n", (long unsigned int) size,
                    ord->object->handle.value)
          ord->extended_size = 0;
          ord->num_handles   = 0;

          return 0;
        }
      if (ord->extended_size == 0)
        {
          ord->extended      = (unsigned char *) malloc(size);

          ord->extended_size = size;
        }
      else
        {
          ord->extended_size += size;
          ord->extended      = (unsigned char *) realloc(ord->extended,
                                                    ord->extended_size);
        }
      error = bit_read_H(dat, &ord->extended_handle);

      if (error)
        LOG_ERROR("Error reading extended handle!\n")

  return 0;
}

/** Decode handleref of object */
Dwg_Object_Ref *
dwg_decode_handleref(Bit_Chain *dat, Dwg_Object *obj, Dwg_Data *dwg)

{
  // Welcome to the house of evil code!
  Dwg_Object_Ref* ref = (Dwg_Object_Ref *) malloc(sizeof(Dwg_Object_Ref));

  if (bit_read_H(dat, &ref->handleref))
    {
      if (obj)
        {
          LOG_ERROR("Could not read handleref in object whose handle is: %d."
                    "%d.%lu \n", obj->handle.code, obj->handle.size,
                    obj->handle.value)
        }
      else
        {
          LOG_ERROR("Could not read handleref in the header variables section"
                    " \n")

        }
      free(ref);
      return 0;
    }

  //Reserve memory space for object references
  if (dwg->num_object_refs == 0)
    dwg->object_ref = (Dwg_Object_Ref **) malloc(REFS_PER_REALLOC * 
                       sizeof(Dwg_Object_Ref*));
  else
    if (dwg->num_object_refs % REFS_PER_REALLOC == 0)
      dwg->object_ref = (Dwg_Object_Ref **) realloc(dwg->object_ref,
                         (dwg->num_object_refs + REFS_PER_REALLOC) *
                         sizeof(Dwg_Object_Ref*));

  dwg->object_ref[dwg->num_object_refs++] = ref;
  ref->absolute_ref = ref->handleref.value;
  ref->obj          = 0;

  /* we receive a null obj when we are reading
   * handles in the header variables section
   */
  if (!obj)
    return ref;

  /* sometimes the code indicates the type of ownership
   * in other cases the handle is stored as an offset from some other
   * handle how is it determined?
   */
  ref->absolute_ref = 0;

   // that's right: don't bother the code on the spec.
  switch(ref->handleref.code)
    {
    case 0x06: // what if 6 means HARD_OWNER?

      ref->absolute_ref = (obj->handle.value + 1);
      break;
    case 0x08:
      ref->absolute_ref = (obj->handle.value - 1);
      break;
    case 0x0A:
      ref->absolute_ref = (obj->handle.value + ref->handleref.value);
      break;
    case 0x0C:
      ref->absolute_ref = (obj->handle.value - ref->handleref.value);
      break;
    default: //0x02, 0x03, 0x04, 0x05 or none
      ref->absolute_ref = ref->handleref.value;
      break;
    }
  return ref;
}

/** Decode handleref through handle code */
Dwg_Object_Ref *
dwg_decode_handleref_with_code(Bit_Chain *dat, Dwg_Object *obj, Dwg_Data *dwg,
                               unsigned int code)
{
  Dwg_Object_Ref * ref;


  ref = dwg_decode_handleref(dat, obj, dwg);

  if (!ref)
    {
      LOG_ERROR("dwg_decode_handleref_with_code: ref is a null pointer \n");
      return 0;
    }
  if (ref->absolute_ref == 0 && ref->handleref.code != code)
    {
      LOG_ERROR("Expected a CODE %d handle, got a %d\n", code,
                ref->handleref.code)
      /*TODO: At the moment we are tolerating wrong codes in handles.
       * in the future we might want to get strict and return 0 here so 
       * that code will crash whenever it reaches the first handle parsing
       * error. This might make debugging easier.
       */
      return 0;

    }
  return ref;
}

/** Decode entity data */
void
dwg_decode_common_entity_handle_data(Bit_Chain *dat, Dwg_Object *obj)
{
  //XXX setup required to use macros
  long unsigned int vcount;
  Dwg_Object_Entity *ent, *_obj;

  Dwg_Data *dwg = obj->parent;
  ent = obj->as.entity;
  _obj = ent;

  SEEK_TO_HANDLES
  
  #include "common_entity_handle_data.spec"

}

enum RES_BUF_VALUE_TYPE
{
  VT_INVALID  = 0,
  VT_STRING   = 1,
  VT_POINT3D  = 2,
  VT_REAL     = 3,
  VT_INT16    = 4,
  VT_INT32    = 5,
  VT_INT8     = 6,
  VT_BINARY   = 7,
  VT_HANDLE   = 8,
  VT_OBJECTID = 9,
  VT_BOOL     = 10

};

enum RES_BUF_VALUE_TYPE
get_base_value_type(short gc)
{
  if (gc >= 300)
    {
      if (gc >= 440) 
        {
          if (gc >= 1000)  // 1000-1071
            {
              if (gc == 1004) return VT_BINARY;
          else            // 300-389

            {
/** DEcode Xdata */
Dwg_Resbuf*
dwg_decode_xdata(Bit_Chain *dat, int size)
{
  Dwg_Resbuf *rbuf, *root=0, *curr=0;
  long unsigned int end_address;
  int i, length;

  static int cnt = 0;
  
  cnt++;
  end_address = dat->byte + (unsigned long int) size;

          length = bit_read_RS(dat);

          rbuf->value.chunk.size = bit_read_RC(dat);  
        
          if (rbuf->value.chunk.size > 0)
            {
              rbuf->value.chunk.data = (char *)malloc(rbuf->value.chunk.size
                                                      * sizeof(char));



      if (curr == 0)
        curr = root = rbuf;
      else
        {
          curr->next = rbuf;
          curr = rbuf;
        }
    }
    return root;
}
          dwg->num_objects)

  obj = &dwg->object[dwg->num_objects];
  obj->index = dwg->num_objects;
  dwg->num_objects++;

  obj->handle.code = 0;
  obj->handle.size = 0;
  obj->handle.value = 0;

  obj->parent = dwg;
  obj->size = bit_read_MS(dat);
  object_address = dat->byte;
  ktl_lastaddress = dat->byte + obj->size; /* (calculate the bitsize) */
  obj->type = bit_read_BS(dat);

  LOG_INFO(" Type: %d\n", obj->type)

  /* Check the type of the object
   */
  switch (obj->type)
    {
  case DWG_TYPE_TEXT:
    dwg_decode_TEXT(dat, obj);
    break;
  case DWG_TYPE_ATTRIB:
    dwg_decode_ATTRIB(dat, obj);
    break;
  case DWG_TYPE_ATTDEF:
    dwg_decode_ATTDEF(dat, obj);
    break;
  case DWG_TYPE_BLOCK:
    dwg_decode_BLOCK(dat, obj);
    break;
  case DWG_TYPE_ENDBLK:
    dwg_decode_ENDBLK(dat, obj);
    break;
  case DWG_TYPE_SEQEND:
    dwg_decode_SEQEND(dat, obj);
    break;
  case DWG_TYPE_INSERT:
    dwg_decode_INSERT(dat, obj);
    break;
  case DWG_TYPE_MINSERT:
    dwg_decode_MINSERT(dat, obj);
    break;
  case DWG_TYPE_VERTEX_2D:
    dwg_decode_VERTEX_2D(dat, obj);
    break;
  case DWG_TYPE_VERTEX_3D:
    dwg_decode_VERTEX_3D(dat, obj);
    break;
  case DWG_TYPE_VERTEX_MESH:
    dwg_decode_VERTEX_MESH(dat, obj);
    break;
  case DWG_TYPE_VERTEX_PFACE:
    dwg_decode_VERTEX_PFACE(dat, obj);
    break;
  case DWG_TYPE_VERTEX_PFACE_FACE:
    dwg_decode_VERTEX_PFACE_FACE(dat, obj);
    break;
  case DWG_TYPE_POLYLINE_2D:
    dwg_decode_POLYLINE_2D(dat, obj);
    break;
  case DWG_TYPE_POLYLINE_3D:
    dwg_decode_POLYLINE_3D(dat, obj);
    break;
  case DWG_TYPE_ARC:
    dwg_decode_ARC(dat, obj);
    break;
  case DWG_TYPE_CIRCLE:
    dwg_decode_CIRCLE(dat, obj);
    break;
  case DWG_TYPE_LINE:
    dwg_decode_LINE(dat, obj);
    break;
  case DWG_TYPE_DIMENSION_ORDINATE:
    dwg_decode_DIMENSION_ORDINATE(dat, obj);
    break;
  case DWG_TYPE_DIMENSION_LINEAR:
    dwg_decode_DIMENSION_LINEAR(dat, obj);
    break;
  case DWG_TYPE_DIMENSION_ALIGNED:
    dwg_decode_DIMENSION_ALIGNED(dat, obj);
    break;
  case DWG_TYPE_DIMENSION_ANG3PT:
    dwg_decode_DIMENSION_ANG3PT(dat, obj);
    break;
  case DWG_TYPE_DIMENSION_ANG2LN:
    dwg_decode_DIMENSION_ANG2LN(dat, obj);
    break;
  case DWG_TYPE_DIMENSION_RADIUS:
    dwg_decode_DIMENSION_RADIUS(dat, obj);
    break;
  case DWG_TYPE_DIMENSION_DIAMETER:
    dwg_decode_DIMENSION_DIAMETER(dat, obj);
    break;
  case DWG_TYPE_POINT:
    dwg_decode_POINT(dat, obj);
    break;
  case DWG_TYPE__3DFACE:
    dwg_decode__3DFACE(dat, obj);
    break;
  case DWG_TYPE_POLYLINE_PFACE:
    dwg_decode_POLYLINE_PFACE(dat, obj);
    break;
  case DWG_TYPE_POLYLINE_MESH:
    dwg_decode_POLYLINE_MESH(dat, obj);
    break;
  case DWG_TYPE_SOLID:
    dwg_decode_SOLID(dat, obj);
    break;
  case DWG_TYPE_TRACE:
    dwg_decode_TRACE(dat, obj);
    break;
  case DWG_TYPE_SHAPE:
    dwg_decode_SHAPE(dat, obj);
    break;
  case DWG_TYPE_VIEWPORT:
    dwg_decode_VIEWPORT(dat, obj);
    break;
  case DWG_TYPE_ELLIPSE:
    dwg_decode_ELLIPSE(dat, obj);
    break;
  case DWG_TYPE_SPLINE:
    dwg_decode_SPLINE(dat, obj);
    break;
  case DWG_TYPE_REGION:
    dwg_decode_REGION(dat, obj);
    break;
  case DWG_TYPE_3DSOLID:
    dwg_decode__3DSOLID(dat, obj);
    break;
  case DWG_TYPE_BODY:
    dwg_decode_BODY(dat, obj);
    break;
  case DWG_TYPE_RAY:
    dwg_decode_RAY(dat, obj);
    break;
  case DWG_TYPE_XLINE:
    dwg_decode_XLINE(dat, obj);
    break;
  case DWG_TYPE_DICTIONARY:
    dwg_decode_DICTIONARY(dat, obj);
    break;
  case DWG_TYPE_MTEXT:
    dwg_decode_MTEXT(dat, obj);
    break;
  case DWG_TYPE_LEADER:
    dwg_decode_LEADER(dat, obj);
    break;
  case DWG_TYPE_TOLERANCE:
    dwg_decode_TOLERANCE(dat, obj);
    break;
  case DWG_TYPE_MLINE:
    dwg_decode_MLINE(dat, obj);
    break;
  case DWG_TYPE_BLOCK_CONTROL:
    dwg_decode_BLOCK_CONTROL(dat, obj);
    break;
  case DWG_TYPE_BLOCK_HEADER:
    dwg_decode_BLOCK_HEADER(dat, obj);
    break;
  case DWG_TYPE_LAYER_CONTROL:
    //set LAYER_CONTROL object - helps keep track of layers
    obj->parent->layer_control = obj;
    dwg_decode_LAYER_CONTROL(dat, obj);
    break;
  case DWG_TYPE_LAYER:
    dwg_decode_LAYER(dat, obj);
    break;
  case DWG_TYPE_SHAPEFILE_CONTROL:
    dwg_decode_SHAPEFILE_CONTROL(dat, obj);
    break;
  case DWG_TYPE_SHAPEFILE:
    dwg_decode_SHAPEFILE(dat, obj);
    break;
  case DWG_TYPE_LTYPE_CONTROL:
    dwg_decode_LTYPE_CONTROL(dat, obj);
    break;
  case DWG_TYPE_LTYPE:
    dwg_decode_LTYPE(dat, obj);
    break;
  case DWG_TYPE_VIEW_CONTROL:
    dwg_decode_VIEW_CONTROL(dat, obj);
    break;
  case DWG_TYPE_VIEW:
    dwg_decode_VIEW(dat, obj);
    break;
  case DWG_TYPE_UCS_CONTROL:
    dwg_decode_UCS_CONTROL(dat, obj);
    break;
  case DWG_TYPE_UCS:
    dwg_decode_UCS(dat, obj);
    break;
  case DWG_TYPE_VPORT_CONTROL:
    dwg_decode_VPORT_CONTROL(dat, obj);
    break;
  case DWG_TYPE_VPORT:
    dwg_decode_VPORT(dat, obj);
    break;
  case DWG_TYPE_APPID_CONTROL:
    dwg_decode_APPID_CONTROL(dat, obj);
    break;
  case DWG_TYPE_APPID:
    dwg_decode_APPID(dat, obj);
    break;
  case DWG_TYPE_DIMSTYLE_CONTROL:
    dwg_decode_DIMSTYLE_CONTROL(dat, obj);
    break;
  case DWG_TYPE_DIMSTYLE:
    dwg_decode_DIMSTYLE(dat, obj);
    break;
  case DWG_TYPE_VP_ENT_HDR_CONTROL:
    dwg_decode_VP_ENT_HDR_CONTROL(dat, obj);
    break;
  case DWG_TYPE_VP_ENT_HDR:
    dwg_decode_VP_ENT_HDR(dat, obj);
    break;
  case DWG_TYPE_GROUP:
    dwg_decode_GROUP(dat, obj);
    break;
  case DWG_TYPE_MLINESTYLE:
    dwg_decode_MLINESTYLE(dat, obj);
    break;
  case DWG_TYPE_LWPLINE:
    dwg_decode_LWPLINE(dat, obj);
    break;
  case DWG_TYPE_HATCH:
    dwg_decode_HATCH(dat, obj);
    break;
  case DWG_TYPE_XRECORD:
    dwg_decode_XRECORD(dat, obj);
    break;
  case DWG_TYPE_PLACEHOLDER:
    dwg_decode_PLACEHOLDER(dat, obj);
    break;
  case DWG_TYPE_LAYOUT:
    dwg_decode_LAYOUT(dat, obj);
    break;
  default:
    if (!dwg_decode_variable_type(dwg, dat, obj))
      {
        LOG_INFO("Object UNKNOWN:\n")

        SINCE(R_2000)
          {
            bit_read_RL(dat);  // skip bitsize
          }

        if (!bit_read_H(dat, &obj->handle))
          {
            LOG_INFO("Object handle: %x.%x.%lx\n", 
              obj->handle.code, obj->handle.size, obj->handle.value)
          }

        obj->supertype = DWG_SUPERTYPE_UNKNOWN;
        obj->tio.unknown = (unsigned char*)malloc(obj->size);
        memcpy(obj->tio.unknown, &dat->chain[object_address], obj->size);
      }
    }

  /*
   if (obj->supertype != DWG_SUPERTYPE_UNKNOWN)
   {
   fprintf (stderr, " Begin address:\t%10lu\n", address);
   fprintf (stderr, " Last address:\t%10lu\tSize: %10lu\n", dat->byte, obj->size);
   fprintf (stderr, "End address:\t%10lu (calculated)\n", address + 2 + obj->size);
   }
   */

  /* Register the previous addresses for return
   */
  dat->byte = previous_address;
  dat->bit = previous_bit;
}

#undef IS_DECODER
=======

