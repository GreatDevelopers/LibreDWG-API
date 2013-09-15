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
 *     \file       decode_r2004.h
 *     \brief      R2004 decode function prototypes
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \author     modified by Till Heuschmann
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#ifndef _DWG_R2004_H_
#define _DWG_R2004_H_

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

/* Encrypted Section Header */
typedef union _encrypted_section_header
{
  uint32_t long_data[8];
  unsigned char char_data[32];
  struct
  {
    int32_t tag;
    int32_t section_type;
    int32_t data_size;
    int32_t section_size;
    int32_t start_offset;
    int32_t unknown;
    int32_t checksum_1;
    int32_t checksum_2;
  } fields;
} encrypted_section_header;

int
decode_R2004(Bit_Chain *dat, Dwg_Data *dwg);

Dwg_Section*
find_section(Dwg_Data *dwg, uint32_t index);

int32_t
page_checksum(int32_t seed, unsigned char *data, int32_t size);

void
read_R2004_section_info(Bit_Chain *dat, Dwg_Data *dwg, uint32_t comp_data_size,
                        uint32_t decomp_data_size);

#endif
