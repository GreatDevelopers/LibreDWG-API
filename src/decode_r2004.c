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
 *     \file       decode_r2004.c
 *     \brief      R2004 decode functions
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \author     modified by Till Heuschmann
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#include "decode_r2004.h"
#include "classes.h"
#include "compress_decompress.h"
#include "handle.h"
#include "header.h"
#include "logging.h"
#include "resolve_pointers.h"
#include "section_locate.h"

extern unsigned int loglevel;

#define DWG_LOGLEVEL loglevel

Dwg_Section *
find_section(Dwg_Data *dwg, uint32_t index)
{
  int i;

  if (dwg->header.section == 0 || index == 0)
    return 0;

  for (i = 0; i < dwg->header.num_sections; ++i)
    {
      if (dwg->header.section[i].number == index)
        return (&dwg->header.section[i]);
    }
  return 0;
}

/** Section page checksum */
int32_t
page_checksum(int32_t seed, unsigned char *data, int32_t size)
{
  //seed = 0;
  int32_t sum1, sum2, opcode, chunksize;
  int i = 0;

  sum1 = seed & 0xffff;
  sum2 = seed >> 0x10;
  opcode = 0x15b0; 

  while (size!=0)
    {
      if ( &opcode > size)
         return (chunksize = &opcode);
      else
         return (chunksize = size);
 
      size -= chunksize;

      for (i; i < chunksize; i++)
         {
           sum1 += *data++;
           sum2 += sum1;
         }
      sum1 %= 0xFFF1;
      sum2 %= 0xFFF1;
    }

   return (sum2 << 0x10) | (sum1 & 0xffff);
}

/** Read R2004 Section Info (data map) */
void
read_R2004_section_info(Bit_Chain *dat, Dwg_Data *dwg, uint32_t comp_data_size,
                        uint32_t decomp_data_size)
{
  char *decomp, *ptr;
  int data_size, i, j, section_number, start_offset, unknown;

  decomp = (char *)malloc(decomp_data_size * sizeof(char));

  if (decomp == 0)
    return;   // No memory

  decompress_R2004_section(dat, decomp, comp_data_size);
  memcpy(&dwg->header.num_descriptions, decomp, 4);

  dwg->header.section_info = (Dwg_Section_Info*)malloc(sizeof(Dwg_Section_Info)
                              * dwg->header.num_descriptions);

  LOG_TRACE("\n 2004 Section Info fields \n")
  LOG_TRACE("NumDescriptions: %d\n", *((int*)decomp))
  LOG_TRACE("0x02:            %x\n", *((int*)decomp + 1))
  LOG_TRACE("0x00007400:      %x\n", *((int*)decomp + 2))
  LOG_TRACE("0x00:            %x\n", *((int*)decomp + 3))
  LOG_TRACE("Unknown:         %x\n", *((int*)decomp + 4))

  ptr = decomp + 20;

  for (i = 0; i < dwg->header.num_descriptions; ++i)
    {
      dwg->header.section_info[i].size            = *((int*)ptr);
      dwg->header.section_info[i].unknown1 	  = *((int*)ptr + 1);
      dwg->header.section_info[i].num_sections    = *((int*)ptr + 2);
      dwg->header.section_info[i].max_decomp_size = *((int*)ptr + 3);
      dwg->header.section_info[i].unknown2        = *((int*)ptr + 4);
      dwg->header.section_info[i].compressed      = *((int*)ptr + 5);
      dwg->header.section_info[i].type            = *((int*)ptr + 6);
      dwg->header.section_info[i].encrypted       = *((int*)ptr + 7);

      ptr += 32;
      memcpy(dwg->header.section_info[i].name, ptr, 64);
      ptr += 64;

      LOG_TRACE("\n Section Info description fields \n")
      LOG_TRACE("Size: %d \n",
                (int) dwg->header.section_info[i].size)
      LOG_TRACE("Unknown: %d \n",
                (int) dwg->header.section_info[i].unknown1)
      LOG_TRACE("Number of sections: %d \n",
                (int) dwg->header.section_info[i].num_sections)
      LOG_TRACE("Max decompressed size: %d \n",
                (int) dwg->header.section_info[i].max_decomp_size)
      LOG_TRACE("Unknown: %d \n",
                (int) dwg->header.section_info[i].unknown2)
      LOG_TRACE("Compressed (0x02): %x \n",
                (unsigned int) dwg->header.section_info[i].compressed)
      LOG_TRACE("Section Type: %d \n",
                (int) dwg->header.section_info[i].type)
      LOG_TRACE("Encrypted: %d \n",
                (int) dwg->header.section_info[i].encrypted)
      LOG_TRACE("Section Name: %s \n\n", dwg->header.section_info[i].name)

      dwg->header.section_info[i].sections = (Dwg_Section**) malloc
                                      (dwg->header.section_info[i].num_sections
                                       * sizeof(Dwg_Section*));

      if (dwg->header.section_info[i].num_sections < 10000)
	{
	  LOG_INFO("Section count %ld in area %d \n", 
                   dwg->header.section_info[i].num_sections, i)

	  for (j = 0; j < dwg->header.section_info[i].num_sections; j++)
	    {
	      section_number = *((int*)ptr);      // Index into SectionMap
	      data_size      = *((int*)ptr + 1);
	      start_offset   = *((int*)ptr + 2);
              // high 32 bits of 64-bit start offset?
	      unknown        = *((int*)ptr + 3);  
	      ptr += 16;

	      dwg->header.section_info[i].sections[j] = find_section(dwg,
                                                        section_number);

	      LOG_TRACE("Section Number: %d\n", section_number)
	      LOG_TRACE("Data size:      %d\n", data_size)
	      LOG_TRACE("Start offset:   %x\n", start_offset)
	      LOG_TRACE("Unknown:        %d\n", unknown)
	    }
	}  // sanity check
      else
	{
	  LOG_ERROR("section count %ld in area %d too high! skipping \n",
                    dwg->header.section_info[i].num_sections, i)
	}
    }
  free(decomp);
}

/** Decode R2004 version */
int
decode_R2004(Bit_Chain *dat, Dwg_Data *dwg)
{
  int i;
  int32_t preview_address, security_type, unknown_long, dwg_property_address,
          vba_proj_address;
  unsigned char ver_string, sig, dwg_ver, maint_release_ver, acad_maint_ver;

  /* Encrypted Data */
  union
  {
    unsigned char encrypted_data[0x6c];
    struct
    {
      unsigned char file_ID_string[12];
      int32_t x00;
      int32_t x6c;
      int32_t x04;
      int32_t root_tree_node_gap;
      int32_t lowermost_left_tree_node_gap;
      int32_t lowermost_right_tree_node_gap;
      int32_t unknown_long;
      int32_t last_section_id;
      int32_t last_section_address;
      int32_t x00_2;
      int32_t second_header_address;
      int32_t x00_3;
      int32_t gap_amount;
      int32_t section_amount;
      int32_t x20;
      int32_t x80;
      int32_t x40;
      int32_t section_map_id;
      int32_t section_map_address;
      int32_t x00_4;
      int32_t section_info_id;
      int32_t section_array_size;
      int32_t gap_array_size;
      int32_t CRC;
    } fields;
  } _2004_header_data;

  /* System Section */
  typedef union _system_section
  {
    unsigned char data[0x14];
    struct
    {
      int32_t section_type;     //0x4163043b
      int32_t decomp_data_size;
      int32_t comp_data_size;
      int32_t compression_type;
      int32_t checksum;
    } fields;
  } system_section;

  system_section ss;
  int rseed = 1;
  Dwg_Section *section;
 
  /* Version string */
  dat->byte = 0x00;
  LOG_INFO("Version string: ")
  for (i = 0; i < 6; i++)
    {
      ver_string = bit_read_RC(dat);
      LOG_INFO("0x%02X", ver_string)
    }
  LOG_INFO("\n")

  /* 5 bytes of 0x00 */
  dat->byte = 0x06;
  LOG_INFO("5 bytes of 0x00: ")
  for (i = 0; i < 4; i++)
    {
      sig = bit_read_RC(dat);
      LOG_INFO("0x%02X ", sig)
    }
  LOG_INFO("\n")

  /* Maintenance release version */
  dat->byte = 0x0B;
  maint_release_ver = bit_read_RC(dat);
  LOG_INFO("Maintenanace release version: 0x%02X \n", maint_release_ver)

  /* Byte 0x00, 0x01, or 0x03 */
  dat->byte = 0x0C;
  sig = bit_read_RC(dat);
  LOG_INFO("Byte 0x00, 0x01, or 0x03: 0x%02X \n", sig)

  /* Preview Address */
  dat->byte = 0x0D;
  preview_address = bit_read_RL(dat);
  LOG_INFO("Preview Address: 0x%08X \n", (unsigned int) preview_address)

  /* DwgVer */
  dat->byte = 0x11;
  dwg_ver   = bit_read_RC(dat);
  LOG_INFO("DwgVer: %u\n", dwg_ver)

  /* MaintReleaseVer */
  dat->byte = 0x12;
  acad_maint_ver = bit_read_RC(dat);
  LOG_INFO("MaintRelease: %u \n", acad_maint_ver)

  /* Codepage */
  dat->byte = 0x13;
  dwg->header.codepage = bit_read_RS(dat);
  LOG_INFO("Codepage: %u \n", dwg->header.codepage)

  /* 3 0x00 bytes */
  dat->byte = 0x15;
  LOG_INFO("3 0x00 bytes: ")
  for (i = 0; i < 3; i++)
    {
      sig = bit_read_RC(dat);
      LOG_INFO("0x%02X", sig)
    }
  LOG_INFO("\n")

  /* SecurityType */
  dat->byte = 0x18;
  security_type = bit_read_RL(dat);
  LOG_INFO("Security Type: 0x%08X \n", (unsigned int) security_type)

  /* Unknown long */
  dat->byte = 0x1C;
  unknown_long = bit_read_RL(dat);
  LOG_INFO("Unknown long: 0x%08X \n", (unsigned int) unknown_long)

  /* Summary Info Addr */
  dat->byte = 0x20;
  dwg_property_address = bit_read_RL(dat);
  LOG_INFO("Summary Info Addr: 0x%08X \n", (unsigned int) dwg_property_address)

  /* VBA Project Addr */
  dat->byte = 0x24;
  vba_proj_address = bit_read_RL(dat);
  LOG_INFO("VBA Project Addr: 0x%08X \n", (unsigned int) vba_proj_address)

  /* 0x00000080 */
  dat->byte = 0x28;
  unknown_long = bit_read_RL(dat);
  LOG_INFO("0x00000080: 0x%08X \n", (unsigned int) unknown_long)

  /* 0x00 bytes (length = 0x54 bytes) */
  dat->byte = 0x2C;
  for (i = 0; i < 0x54; i++)
    {
      sig = bit_read_RC(dat);
      if (sig != 0 && loglevel)
        LOG_ERROR("Warning: Byte should be zero! But a value = %x was read "
                  "instead.\n", sig)
    }

  dat->byte = 0x80;
  for (i = 0; i < 0x6c; i++)
    {
      rseed *= 0x343fd;
      rseed += 0x269ec3;
      _2004_header_data.encrypted_data[i] = bit_read_RC(dat) ^ (rseed >> 0x10);
    }
  if (loglevel)
    {
      LOG_TRACE("\n 2004 File Header Data fields \n")
      LOG_TRACE("File ID string (must be AcFssFcAJMB): ");

      for (i = 0; i < 12; i++)
        {
          LOG_TRACE("%c", _2004_header_data.fields.file_ID_string[i])
        }

      LOG_TRACE("\n")
      LOG_TRACE("0x00 (long): %x \n",
                (unsigned int) _2004_header_data.fields.x00)
      LOG_TRACE("0x6c (long): %x \n",
                 (unsigned int) _2004_header_data.fields.x6c)
      LOG_TRACE("0x04 (long): %x \n",
                 (unsigned int) _2004_header_data.fields.x04)
      LOG_TRACE("Root tree node gap: %x \n",
                (unsigned int) _2004_header_data.fields.root_tree_node_gap)
      LOG_TRACE("Lowermost left tree node gap: %x \n",
                (unsigned int) _2004_header_data.fields.lowermost_left_tree_node_gap)
      LOG_TRACE("Lowermost right tree node gap: %x \n",
                (unsigned int) _2004_header_data.fields.lowermost_right_tree_node_gap)
      LOG_TRACE("Unknown long: %x \n",
                (unsigned int) _2004_header_data.fields.unknown_long)
      LOG_TRACE("Last section id: %x  \n",
                (unsigned int) _2004_header_data.fields.last_section_id)
      LOG_TRACE("Last section address: %x\n",
                (unsigned int) _2004_header_data.fields.last_section_address)
      LOG_TRACE("0x00 (long): %x \n",
                (unsigned int) _2004_header_data.fields.x00_2)
      LOG_TRACE("Second header address: %x\n",
                (unsigned int) _2004_header_data.fields.second_header_address)
      LOG_TRACE("0x00 (long): %x \n",
                (unsigned int) _2004_header_data.fields.x00_3)
      LOG_TRACE("Gap amount: %x \n",
                (unsigned int) _2004_header_data.fields.gap_amount)
      LOG_TRACE("Section amount: %x \n",
                (unsigned int) _2004_header_data.fields.section_amount)
      LOG_TRACE("0x20 (long): %x \n",
                (unsigned int) _2004_header_data.fields.x20)
      LOG_TRACE("0x80 (long): %x \n",
                (unsigned int) _2004_header_data.fields.x80)
      LOG_TRACE("0x40 (long): %x \n",
                (unsigned int) _2004_header_data.fields.x40)
      LOG_TRACE("Section map id: %x \n",
                (unsigned int) _2004_header_data.fields.section_map_id)
      LOG_TRACE("Section map address: %x \n",
                (unsigned int) _2004_header_data.fields.section_map_address 
                 + 0x100)
      LOG_TRACE("0x00 (long): %x \n",
                (unsigned int) _2004_header_data.fields.x00_4)
      LOG_TRACE("Section Info id: %x \n",
                (unsigned int) _2004_header_data.fields.section_info_id)
      LOG_TRACE("Section array size: %x \n",
                (unsigned int) _2004_header_data.fields.section_array_size)
      LOG_TRACE("Gap array size: %x \n",
                (unsigned int) _2004_header_data.fields.gap_array_size)
      LOG_TRACE("CRC: %x\n", (unsigned int) _2004_header_data.fields.CRC)
    }

  /* Section Map */
  dat->byte = _2004_header_data.fields.section_map_address + 0x100;
  LOG_TRACE("\n\n Raw system section bytes: \n");

  for (i = 0; i < 0x14; i++)
    {
      ss.data[i] = bit_read_RC(dat);
      LOG_TRACE("%x ", ss.data[i])
    }

  LOG_TRACE("\n System Section (Section Map) \n")
  LOG_TRACE("Section Type (should be 0x4163043b): %x \n",
            (unsigned int) ss.fields.section_type)
  LOG_TRACE("DecompDataSize: %x \n",
            (unsigned int) ss.fields.decomp_data_size)
  LOG_TRACE("CompDataSize: %x \n",
            (unsigned int) ss.fields.comp_data_size)
  LOG_TRACE("Compression Type: %x \n",
            (unsigned int) ss.fields.compression_type)
  LOG_TRACE("Checksum: %x \n\n", (unsigned int) ss.fields.checksum)

  read_R2004_section_map(dat, dwg, ss.fields.comp_data_size,
                         ss.fields.decomp_data_size);

  if (dwg->header.section == 0)
    {
      LOG_ERROR("Failed to read R2004 Section Map.\n")
      return -1;
    }

  /* Section Info */
  section = find_section(dwg, _2004_header_data.fields.section_info_id);

  if (section != 0)
    {
      dat->byte = section->address;
      LOG_TRACE("\n Raw system section bytes: \n")

      for (i = 0; i < 0x14; i++)
        {
          ss.data[i] = bit_read_RC(dat);
          LOG_TRACE("%x ", ss.data[i])
        }

      LOG_TRACE("\n  System Section (Section Info) \n")
      LOG_TRACE("Section Type (should be 0x4163043b): %x \n",
                (unsigned int) ss.fields.section_type)
      LOG_TRACE("DecompDataSize: %x \n",
                (unsigned int) ss.fields.decomp_data_size)
      LOG_TRACE("CompDataSize: %x \n",
                (unsigned int) ss.fields.comp_data_size)
      LOG_TRACE("Compression Type: %x \n",
                (unsigned int) ss.fields.compression_type)
      LOG_TRACE("Checksum: %x \n\n", (unsigned int) ss.fields.checksum)

       read_R2004_section_info(dat, dwg, ss.fields.comp_data_size,
                               ss.fields.decomp_data_size);
    }
  read_2004_section_classes(dat, dwg);
  read_2004_section_header(dat, dwg);
  read_2004_section_handles(dat, dwg);

  /* Clean up */
  if (dwg->header.section_info != 0)
    {
      for (i = 0; i < dwg->header.num_descriptions; ++i)
        if (dwg->header.section_info[i].sections != 0)
          free(dwg->header.section_info[i].sections);

      free(dwg->header.section_info);
      dwg->header.num_descriptions = 0;
    }
  resolve_objectref_vector(dwg);
  LOG_ERROR("Decoding of DWG version R2004 header is not fully implemented "
            " yet. We are going to try \n")
  return 0;
}
