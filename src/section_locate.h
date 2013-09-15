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
 *     \file       section_locate.h
 *     \brief      Section locating function prototypes
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \author     modified by Till Heuschmann
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#ifndef _DWG_LOCATE_H_
#define _DWG_LOCATE_H_

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

void
read_R13_R15_section_locate(Bit_Chain *dat, Dwg_Data *dwg);

void
read_R2004_section_map(Bit_Chain *dat, Dwg_Data *dwg, uint32_t comp_data_size,
                       uint32_t decomp_data_size);

r2007_section*
get_section(r2007_section *sections_map, int64_t hashcode);

r2007_section*
read_sections_map(Bit_Chain *dat, int64_t size_comp, int64_t size_uncomp,
                  int64_t correction);

#endif
