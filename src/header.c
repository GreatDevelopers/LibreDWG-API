/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2009 Free Software Foundation, Inc.                        */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

/**
 *     \file       header.c
 *     \brief      Header section functions
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \author     modified by Till Heuschmann
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#include "compress_decompress.h"
#include "decode.h"
#include "header.h"
#include "logging.h"

void
dwg_decode_header_variables(Bit_Chain *dat, Dwg_Data *dwg, Bit_Chain *sstream)
{
  Dwg_Header_Variables* _obj = &dwg->header_vars;
  Dwg_Object* obj = 0;

  #include "header_variables.spec"
}

void
dwg_decode_R2007_header_handles(Bit_Chain *dat, Dwg_Data *dwg)
{
  Dwg_Header_Variables* _obj = &dwg->header_vars;
  Dwg_Object* obj=0;
  char c1, c2, c3;
 
  c1 = bit_read_B(dat);
  c2 = bit_read_RC(dat);
  c3 = bit_read_RC(dat); 

  //FIELD_HANDLE (HANDSEED, ANYCODE);
  FIELD_HANDLE (CLAYER, ANYCODE);
  FIELD_HANDLE (TEXTSTYLE, ANYCODE);
  FIELD_HANDLE (CELTYPE, ANYCODE);
  FIELD_HANDLE (CMATERIAL, ANYCODE);
  FIELD_HANDLE (DIMSTYLE, ANYCODE);
  FIELD_HANDLE (CMLSTYLE, ANYCODE);
  FIELD_HANDLE (UCSNAME_PSPACE, ANYCODE);
  FIELD_HANDLE (PUCSBASE, ANYCODE);
  FIELD_HANDLE (PUCSORTHOREF, ANYCODE);
  FIELD_HANDLE (UCSNAME_MSPACE, ANYCODE);
  FIELD_HANDLE (UCSBASE, ANYCODE);
  FIELD_HANDLE (UCSORTHOREF, ANYCODE);
  FIELD_HANDLE (DIMTXTSTY, ANYCODE);
  FIELD_HANDLE (DIMLDRBLK, ANYCODE);
  FIELD_HANDLE (DIMBLK, ANYCODE);
  FIELD_HANDLE (DIMBLK1, ANYCODE);
  FIELD_HANDLE (DIMBLK2, ANYCODE);
  FIELD_HANDLE (DIMLTYPE, ANYCODE);
  FIELD_HANDLE (DIMLTEX1, ANYCODE);
  FIELD_HANDLE (DIMLTEX2, ANYCODE);
  FIELD_HANDLE (BLOCK_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (LAYER_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (STYLE_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (LINETYPE_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (VIEW_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (UCS_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (VPORT_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (APPID_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (DIMSTYLE_CONTROL_OBJECT, ANYCODE);
  FIELD_HANDLE (DICTIONARY_ACAD_GROUP, ANYCODE);
  FIELD_HANDLE (DICTIONARY_ACAD_MLINESTYLE, ANYCODE);
  FIELD_HANDLE (DICTIONARY_NAMED_OBJECTS, ANYCODE);
  FIELD_HANDLE (DICTIONARY_LAYOUTS, ANYCODE);
  FIELD_HANDLE (DICTIONARY_PLOTSETTINGS, ANYCODE);
  FIELD_HANDLE (DICTIONARY_PLOTSTYLES, ANYCODE);
  FIELD_HANDLE (DICTIONARY_MATERIALS, ANYCODE);
  FIELD_HANDLE (DICTIONARY_COLORS, ANYCODE);
  FIELD_HANDLE (DICTIONARY_VISUALSTYLE, ANYCODE);
	
  if (FIELD_VALUE(CEPSNTYPE) == 3)
    {
      FIELD_HANDLE (CPSNID, ANYCODE);
    }
  FIELD_HANDLE (BLOCK_RECORD_PAPER_SPACE, ANYCODE); 
  FIELD_HANDLE (BLOCK_RECORD_MODEL_SPACE, ANYCODE);
  FIELD_HANDLE (LTYPE_BYLAYER, ANYCODE);
  FIELD_HANDLE (LTYPE_BYBLOCK, ANYCODE);
  FIELD_HANDLE (LTYPE_CONTINUOUS, ANYCODE);
  FIELD_HANDLE (INTERFEREOBJVS, ANYCODE);
  FIELD_HANDLE (INTERFEREVPVS, ANYCODE);
  FIELD_HANDLE (DRAGVS, ANYCODE);
}

/** Read R13-R15 Header Section */
void
read_R13_R15_section_header(Bit_Chain *dat, Dwg_Data *dwg)
{
  uint8_t ckr, ckr2;
  uint32_t pvz;

  LOG_INFO("\n Header Variables: %8X \n",
           (unsigned int) dwg->header.section[0].address)
  LOG_INFO("Header Variables (end): %8X \n",
           (unsigned int) (dwg->header.section[0].address
           + dwg->header.section[0].size))

  dat->byte = dwg->header.section[0].address + 16;
  pvz = bit_read_RL(dat);
  LOG_TRACE("Length: %x \n", pvz)

  dat->bit = 0;
  dwg_decode_header_variables(dat, dwg, NULL);

  /* Check CRC on */
  dat->byte = dwg->header.section[0].address + dwg->header.section[0].size
               - 18;
  dat->bit  = 0;

  ckr  = bit_read_RS(dat);
  ckr2 = bit_ckr8(0xc0c1, dat->chain + dwg->header.section[0].address
                  + 16, dwg->header.section[0].size - 34);

  if (ckr != ckr2)
    {
      printf("Section %d crc todo ckr:%x ckr2:%x \n",
             dwg->header.section[0].number, ckr, ckr2);
      return -1;
    }
}

/** R2004 Header Section */
void
read_R2004_section_header(Bit_Chain *dat, Dwg_Data *dwg)
{
  Bit_Chain sec_dat;

  if (read_R2004_compressed_section(dat, dwg, &sec_dat, SECTION_HEADER) != 0)
    return;

  if (bit_search_sentinel(&sec_dat, dwg_sentinel(DWG_SENTINEL_VARIABLE_BEGIN)))
    {
      uint32_t size = bit_read_RL(&sec_dat);
      LOG_TRACE("Length: %x \n", size);
      dwg_decode_header_variables(&sec_dat, dwg, NULL);
    }

  // Check CRC-on
  uint32_t ckr, ckr2;
  
  sec_dat.byte = dwg->header.section[0].address + dwg->header.section[0].size
                 - 18;
  sec_dat.bit  = 0;
 
  ckr  = bit_read_RL(&sec_dat);
  ckr2 = bit_ckr32(0xc0c1, &sec_dat.chain + dwg->header.section[0].address
                    + 16, dwg->header.section[0].size - 34);
 
   if (ckr != ckr2)
     {
       printf("Section %d CRC todo ckr: %x ckr2: %x \n\n",
               dwg->header.section[0].number, ckr, ckr2);
       return -1;
    }
  free(sec_dat.chain);
}

/** R2007 Header Section */
void
read_R2007_section_header(Bit_Chain *dat, Dwg_Data *dwg,
                          r2007_section *sections_map, r2007_page *pages_map)
{
  Bit_Chain sec_dat;
  
  if (read_data_section(&sec_dat, dat, sections_map, pages_map, 0x32b803d9)
      != 0)
    return;  

  if (bit_search_sentinel(&sec_dat, dwg_sentinel(DWG_SENTINEL_VARIABLE_BEGIN)))
    {
      Bit_Chain sstream;
      unsigned long int length, bitsize;

      length = bit_read_RL(&sec_dat);
      LOG_TRACE("Length: %ld \n", length);

      bitsize = bit_read_RL(&sec_dat);
      LOG_TRACE("Offset: %ld \n", bitsize); 

      string_stream_init(&sstream, &sec_dat, bitsize, 0);
      dwg_decode_header_variables(&sec_dat, dwg, &sstream);
      dwg_decode_R2007_header_handles(&sstream, dwg);
      bit_search_sentinel(&sec_dat, dwg_sentinel(DWG_SENTINEL_VARIABLE_END));
    }
  free(sec_dat.chain);
}
