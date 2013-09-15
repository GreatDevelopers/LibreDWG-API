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
 *     \file       decode_r2007.h
 *     \brief      R2007 decode function prototypes
 *     \author     written by Avneet Kaur
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#ifndef _DWG_R2007_H_
#define _DWG_R2007_H_

#include "config.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <sys/types.h>

#include "common.h"
#include "bits.h"
#include "dwg.h"
#include "print.h"

/** \struct   r2007_file_header
 */
typedef struct r2007_file_header
{
  int64_t header_size;            // 0x70
  int64_t file_size;
  int64_t pages_map_crc_compressed;
  int64_t pages_map_correction;
  int64_t pages_map_crc_seed;
  int64_t pages_map2_offset;
  int64_t pages_map2_id;
  int64_t pages_map_offset;       // starting address of the Page Map section
  int64_t pages_map_id;
  int64_t header2_offset;
  int64_t pages_map_size_comp;    // the compressed size of section
  int64_t pages_map_size_uncomp;
  int64_t pages_amount;
  int64_t pages_maxid;
  int64_t unknown1;               // 0x20
  int64_t unknown2;               // 0x40
  int64_t pages_map_crc_uncomp;
  int64_t unknown3;               // 0xf800
  int64_t unknown4;               // 4
  int64_t unknown5;               // 1
  int64_t sections_amount;
  int64_t sections_map_crc_uncomp;
  int64_t sections_map_size_comp;
  int64_t sections_map2_id;
  int64_t sections_map_id;
  int64_t sections_map_size_uncomp;
  int64_t sections_map_crc_comp;
  int64_t sections_map_correction;
  int64_t sections_map_crc_seed;
  int64_t stream_version;         // 0x60100
  int64_t crc_seed;
  int64_t crc_seed_encoded;
  int64_t random_seed;
  int64_t header_crc;
} r2007_file_header;

/** \struct   page map
 */
typedef struct _r2007_page
{
  int64_t id;
  int64_t size;  
  int64_t offset;
  struct _r2007_page *next;
} r2007_page;

/** \struct   section page
 */
typedef struct _r2007_section_page
{
  int64_t offset;
  int64_t size;
  int64_t id;
  int64_t uncomp_size;  // src_size
  int64_t comp_size;
  int64_t checksum;
  int64_t crc;
} r2007_section_page;

/**   \struct   section map
 */
typedef struct _r2007_section
{
  int64_t  data_size;    // max size of page
  int64_t  max_size;
  int64_t  encrypted;
  int64_t  hashcode;
  int64_t  name_length;  // 0x22
  int64_t  unknown;      // 0x00
  int64_t  encoded;
  int64_t  num_pages;
  DWGCHAR  *name;
  r2007_section_page **pages;
  struct _r2007_section *next;
} r2007_section;

r2007_section*
get_section(r2007_section *sections_map, int64_t hashcode);

r2007_page*
get_page(r2007_page *pages_map, int64_t id);

void
pages_destroy(r2007_page *page);

void
sections_destroy(r2007_section *section);

int
decode_R2007(Bit_Chain *dat, Dwg_Data *dwg);

int
read_r2007_meta_data(Bit_Chain *dat, Dwg_Data *dwg);

Bit_Chain *
string_stream_init(Bit_Chain *sstream, Bit_Chain *dat, 
                   unsigned long int bitpos, int check_present_bit);

DWGCHAR*
bfr_read_string(char **src);

int
read_data_page(Bit_Chain *dat, unsigned char *decomp, int64_t page_size, 
               int64_t size_comp, int64_t size_uncomp);

int
read_data_section(Bit_Chain *sec_dat, Bit_Chain *dat,
                  r2007_section *sections_map, r2007_page *pages_map,
                  int64_t hashcode);

void 
read_file_header(Bit_Chain *dat, r2007_file_header *file_header);

char*
decode_rs(const char *src, int block_count, int data_size);

r2007_page*
read_pages_map(Bit_Chain *dat, int64_t size_comp, int64_t size_uncomp,
               int64_t correction);

char*
read_system_page(Bit_Chain *dat, int64_t size_comp, int64_t size_uncomp,
                 int64_t repeat_count);
r2007_section*
read_sections_map(Bit_Chain *dat, int64_t size_comp, int64_t size_uncomp,
                  int64_t correction);

void
bfr_read(void *dst, char **src, size_t size);

#endif
