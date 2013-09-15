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
 *     \file       compress_decompress.h
 *     \brief      R18-R21 Compression and Decompression function prototypes
 *     \author     written by 
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#ifndef _DWG_COMP_DECOMP_H_
#define _DWG_COMP_DECOMP_H_

#define copy_1(offset) \
*dst++ = *(src + offset);

#define copy_2(offset) \
dst = copy_bytes_2(dst, src + offset);

#define copy_3(offset) \
dst = copy_bytes_3(dst, src + offset);

#define copy_4(offset) \
*(uint32_t*)dst = *(uint32_t*)(src + offset); \
dst += 4;

#define copy_8(offset) \
*(uint64_t*)dst = *(uint64_t*)(src + offset); \
dst += 8;

#define copy_16(offset) \
dst = copy_bytes_16(dst, src + offset);

int
read_R2004_literal_length(Bit_Chain *dat, unsigned char *opcode);

int
read_long_compression_offset(Bit_Chain *dat);

int
read_two_byte_offset(Bit_Chain *dat, int *lit_length);

int
decompress_R2004_section(Bit_Chain *dat, char *decomp,
                         uint32_t comp_data_size);

int
read_R2004_compressed_section(Bit_Chain *dat, Dwg_Data *dwg, Bit_Chain *sec_dat,
                              uint32_t section_type);

uint32_t
read_R2007_literal_length(unsigned char **src, unsigned char opcode);

void 
read_instructions(unsigned char **src, unsigned char *opcode, uint32_t *offset,
                  uint32_t *length);

void
copy_compressed_bytes(char *dst, char *src, int length);

int 
decompress_R2007(char *dst, int dst_size, char *src, int src_size);

#endif
