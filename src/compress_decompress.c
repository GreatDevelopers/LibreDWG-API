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
 *     \file       compress_decompress.c
 *     \brief      R18-R21 Compression and Decompression functions
 *     \author     written by 
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#include "compress_decompress.h"
#include "decode_r2004.h"
#include "logging.h"

/** R2004 Literal Length */
int
read_R2004_literal_length(Bit_Chain *dat, unsigned char *opcode)
{
  unsigned char byte = bit_read_RC(dat);

  *opcode = 0x00;

  if (byte >= 0x01 && byte <= 0x0F)
    return byte + 3;
  else if (byte == 0)
    {
      int total = 0;
      total = 0x0F;

      while ((byte = bit_read_RC(dat)) == 0x00)
        {
          total += 0xFF;
        }
      return total + byte + 3;
    }
  else if (byte & 0xF0)
    *opcode = byte;

  return 0;
}

/** R2004 Long Compression Offset */
int
read_long_compression_offset(Bit_Chain *dat)
{
  int total = 0;
  unsigned char byte = bit_read_RC(dat);

  if (byte == 0)
    {
      total = 0xFF;

      while ((byte = bit_read_RC(dat)) == 0x00)
        total += 0xFF;
    }
  return total + byte;
}

/** R2004 Two Byte Offset */
int
read_two_byte_offset(Bit_Chain *dat, int *lit_length)
{
  int offset;
  unsigned char firstByte, secondByte;

  firstByte  = bit_read_RC(dat);
  secondByte = bit_read_RC(dat);

  offset      = (firstByte >> 2) | (secondByte << 6);
  *lit_length = (firstByte & 0x03);
  return offset;
}

/** Compresses a system section of a 2004 DWG flie */
int
read_R2004_compressed_section(Bit_Chain *dat, Dwg_Data *dwg, 
                              Bit_Chain *sec_dat, uint32_t section_type)
{
  char *decomp;
  int i, j;
  int32_t address, max_decomp_size, sec_mask;
  Dwg_Section_Info *info = 0;
  encrypted_section_header es;

  for (i = 0; i < dwg->header.num_descriptions && info == 0; ++i)
    if (dwg->header.section_info[i].type == section_type)
      info = &dwg->header.section_info[i];

  if (info == 0)
    return 1;   // Failed to find section

  max_decomp_size = info->num_sections * info->max_decomp_size;

  decomp = (char *) malloc(max_decomp_size * sizeof(char));

  if (decomp == 0)
    return 2;   // No memory

  for (i = 0; i < info->num_sections; ++i)
    {
      address = info->sections[i]->address;
      dat->byte = address;

      for (j = 0; j < 0x20; j++)
        es.char_data[j] = bit_read_RC(dat);

      sec_mask = 0x4164536b ^ address;

      for (j = 0; j < 8; ++j)
        es.long_data[j] ^= sec_mask;

  LOG_INFO("\nSection (Class) \n")
  LOG_INFO("Section Tag (should be 0x4163043b): %x \n",
           (unsigned int) es.fields.tag)
  LOG_INFO("Section Type: %x \n", (unsigned int) es.fields.section_type)

  /* this is the number of bytes that is read in decompress_R2004_section
     (+ 2bytes) */

  LOG_INFO("Data size:      %x\n", (unsigned int) es.fields.data_size)   
  LOG_INFO("Comp data size: %x\n", (unsigned int) es.fields.section_size)
  LOG_INFO("StartOffset:   %x \n", (unsigned int) es.fields.start_offset)
  LOG_INFO("Unknown:       %x \n", (unsigned int) es.fields.unknown);
  LOG_INFO("Checksum1:     %x \n", (unsigned int) es.fields.checksum_1)
  LOG_INFO("Checksum2:   %x \n\n", (unsigned int) es.fields.checksum_2)

  decompress_R2004_section(dat, &decomp[i * info->max_decomp_size],
                           es.fields.data_size);
    }
  sec_dat->bit     = 0;
  sec_dat->byte    = 0;
  sec_dat->chain   = (unsigned char *)decomp;
  sec_dat->size    = max_decomp_size;
  sec_dat->version = dat->version;
  return 0;
}

/** Decompresses a system section of a 2004 DWG flie */
int
decompress_R2004_section(Bit_Chain *dat, char *decomp, uint32_t comp_data_size)
{
  char *src, *dst = decomp;
  unsigned char opcode1 = 0, opcode2;
  int comp_bytes, comp_offset,  i, lit_length;
  int32_t start_byte = dat->byte;
  
  /* length of the first sequence of uncompressed or literal data */
  lit_length = read_R2004_literal_length(dat, &opcode1);

  for (i = 0; i < lit_length; ++i)
    *dst++ = bit_read_RC(dat);

  opcode1 = 0x00;

  while (dat->byte - start_byte < comp_data_size)
    {
      if (opcode1 == 0x00)
        opcode1 = bit_read_RC(dat);

      if (opcode1 >= 0x40)
        {
          comp_bytes  = ((opcode1 & 0xF0) >> 4) - 1;
          opcode2     = bit_read_RC(dat);
          comp_offset = (opcode2 << 2) | ((opcode1 & 0x0C) >> 2);

          if (opcode1 & 0x03)
            {
              lit_length = (opcode1 & 0x03);
              opcode1  = 0x00;
            }
          else
            lit_length = read_R2004_literal_length(dat, &opcode1);
        }
      else if (opcode1 >= 0x21 && opcode1 <= 0x3F)
        {
          comp_bytes  = opcode1 - 0x1E;
          comp_offset = read_two_byte_offset(dat, &lit_length);

          if (lit_length != 0)
            opcode1 = 0x00;
          else
            lit_length = read_R2004_literal_length(dat, &opcode1);
        }
      else if (opcode1 == 0x20)
        {
          comp_bytes  = read_long_compression_offset(dat) + 0x21;
          comp_offset = read_two_byte_offset(dat, &lit_length);

          if (lit_length != 0)
            opcode1 = 0x00;
          else
            lit_length = read_R2004_literal_length(dat, &opcode1);
        }
      else if (opcode1 >= 0x12 && opcode1 <= 0x1F)
        {
          comp_bytes  = (opcode1 & 0x0F) + 2;
          comp_offset = read_two_byte_offset(dat, &lit_length) + 0x3FFF;

          if (lit_length != 0)
            opcode1 = 0x00;
          else
            lit_length = read_R2004_literal_length(dat, &opcode1);
        }
      else if (opcode1 == 0x10)
        {
          comp_bytes  = read_long_compression_offset(dat) + 9;
          comp_offset = read_two_byte_offset(dat, &lit_length) + 0x3FFF;

          if (lit_length != 0)
            opcode1 = 0x00;
          else
            lit_length = read_R2004_literal_length(dat, &opcode1);
        }
      else if (opcode1 == 0x11)
          break;     // Terminates the input stream, everything is ok!
      else
          return 1;  // error in input stream

      // LOG_TRACE("got compressed data %d\n",comp_bytes)
      // copy "compressed data"
      src = dst - comp_offset - 1;
      assert(src > decomp);

      for (i = 0; i < comp_bytes; ++i)
        *dst++ = *src++;

      // copy "literal data"
      // LOG_TRACE("got literal data %d\n",lit_length)
      for (i = 0; i < lit_length; ++i)
        *dst++ = bit_read_RC(dat);
    }
  return 0;  // Success
}

/** R2007 Literal Length */
uint32_t
read_R2007_literal_length(unsigned char **src, unsigned char opcode)
{
  uint32_t length = opcode + 8;
  
  if (length == 0x17)
    {
      uint32_t n = *(*src)++;
      length += n;
    
      if (n == 0xff)
        {
          do
            {
              n = *(*src)++;
              n |= (*(*src)++ << 8);
              length += n;
            } 
          while (n == 0xFFFF);
        }
    }
  return length;
}

/** Read offset, length and next opcode depending on first opcode byte */
void 
read_instructions(unsigned char **src, unsigned char *opcode, uint32_t *offset,
                  uint32_t *length) 
{
  switch (*opcode >> 4) 
    {
    case 0:
      *length = (*opcode & 0xf) + 0x13;
      *offset = *(*src)++;
      *opcode = *(*src)++;
      *length = ((*opcode >> 3) & 0x10) + *length;
      *offset = ((*opcode & 0x78) << 5) + 1 + *offset;
      break;
    
    case 1:
      *length = (*opcode & 0xf) + 3;
      *offset = *(*src)++;
      *opcode = *(*src)++;
      *offset = ((*opcode & 0xf8) << 5) + 1 + *offset;
      break;
    
    case 2:
      *offset = *(*src)++;
      *offset = ((*(*src)++ << 8) & 0xff00) | *offset;
      *length = *opcode & 7;
    
      if ((*opcode & 8) == 0) 
        {
          *opcode = *(*src)++;
          *length = (*opcode & 0xf8) + *length;
        } 
      else 
        {
          (*offset)++;
          *length = (*(*src)++ << 3) + *length;
          *opcode = *(*src)++;
          *length = (((*opcode & 0xf8) << 8) + *length) + 0x100;
        }
      break;
    
    default:
      *length = *opcode >> 4;
      *offset = *opcode & 15;
      *opcode = *(*src)++;
      *offset = (((*opcode & 0xf8) << 1) + *offset) + 1;
      break;
    }
}

char* 
copy_bytes_2(char *dst, char *src)
{
  dst[0] = src[1];
  dst[1] = src[0];
  return dst + 2;
}

char* 
copy_bytes_3(char *dst, char *src)
{
  dst[0] = src[2];
  dst[1] = src[1];
  dst[2] = src[0];
  return dst + 3;
}

char* 
copy_bytes_16(char *dst, char *src)
{
  *(uint64_t*)dst = *(uint64_t*)(src + 8);
  *(uint64_t*)(dst + 8) = *(uint64_t*)src;  
  return dst + 16;
}

void 
copy_bytes(char *dst, uint32_t length, uint32_t offset)
{
  char *src = dst - offset;
  
  while (length-- > 0)
    *dst++ = *src++;
}

/** R2007 Compression */
void
copy_compressed_bytes(char *dst, char *src, int length)
{
  while (length >= 32)
    {
      copy_16(16);
      copy_16(0);
    
      src += 32;	    
      length -= 32;
    }
  
  switch (length)
  {
    case 1:  
      copy_1(0);
      break;
    case 2:  
      copy_2(0);
      break;
    case 3:  
      copy_3(0);
      break;
    case 4:  
      copy_4(0);        
      break;
    case 5:  
      copy_1(4);
      copy_4(0); 
      break;
    case 6:  
      copy_1(5);
      copy_4(1); 
      copy_1(0);
      break;
    case 7:  
      copy_2(5);
      copy_4(1); 
      copy_1(0); 
      break;
    case 8:  
      copy_8(0);
      break;
    case 9:  
      copy_1(8);
      copy_8(0);
      break;
    case 10:  
      copy_1(9);
      copy_8(1);
      copy_1(0);
      break;
    case 11:  
      copy_2(9);
      copy_8(1);
      copy_1(0);
      break;
    case 12:  
      copy_4(8);
      copy_8(0);
      break;
    case 13:  
      copy_1(12);
      copy_4(8);
      copy_8(0);
      break;
    case 14:  
      copy_1(13);
      copy_4(9);
      copy_8(1);
      copy_1(0);
      break;
    case 15:  
      copy_2(13);
      copy_4(9);
      copy_8(1);
      copy_1(0);
      break;
    case 16:  
      copy_16(0);
      break;
    case 17:
      copy_8(9);
      copy_1(8);
      copy_8(0);
      break;
    case 18:  
      copy_1(17);
      copy_16(1);
      copy_1(0);
      break;
    case 19:  
      copy_3(16);
      copy_16(0);
      break;
    case 20:  
      copy_4(16);
      copy_16(0);
      break;
    case 21:  
      copy_1(20);
      copy_4(16);
      copy_16(0);
      break;
    case 22:  
      copy_2(20);
      copy_4(16);
      copy_16(0);
      break;
    case 23:  
      copy_3(20);
      copy_4(16);
      copy_16(0);
      break;
    case 24:
      copy_8(16);
      copy_16(0);
      break;
    case 25:  
      copy_8(17);
      copy_1(16);
      copy_16(0);        
      break;
    case 26:  
      copy_1(25);
      copy_8(17);
      copy_1(16);
      copy_16(0);
      break;
    case 27:  
      copy_2(25);
      copy_8(17);
      copy_1(16);
      copy_16(0);
      break;
    case 28:  
      copy_4(24);
      copy_8(16);
      copy_16(0);
      break;
    case 29:  
      copy_1(28);
      copy_4(24);
      copy_8(16);
      copy_16(0);
      break;
    case 30:  
      copy_2(28);
      copy_4(24);
      copy_8(16);
      copy_16(0);
      break;
    case 31:  
      copy_1(30);
      copy_4(26);
      copy_8(18);
      copy_16(2);
      copy_2(0);
    break;
      //default:
      //nop
  }
}

/** R2007 Decompression */
int 
decompress_R2007(char *dst, int dst_size, char *src, int src_size)
{
  uint32_t length = 0, offset = 0;

  char *dst_end = dst + dst_size;
  char *src_end = src + src_size;
  unsigned char opcode = *src++;
  
  if ((opcode & 0xf0) == 0x20)
    {
      src += 2;
      length = *src++ & 0x07;
    
      if (length == 0)
        return 1;   
    } 
  while (src < src_end)
    {   
      if (length == 0)
        length = read_R2007_literal_length((unsigned char**)&src, opcode);
      
      if ((dst + length) > dst_end)
        return 1;
      
      copy_compressed_bytes(dst, src, length);
      dst += length;
      src += length;
      length = 0;
      
      if (src >= src_end)
        return 0;
      
      opcode = *src++;
      read_instructions((unsigned char**)&src, &opcode, &offset, &length);
      
      while (1)
        {
          copy_bytes(dst, length, offset);
          dst += length;
          length = (opcode & 7);   
        
          if (length != 0 || src >= src_end)
            break;
        
          opcode = *src++;
        
          if ((opcode >> 4) == 0)
            break;
        
          if ((opcode >> 4) == 0x0f)
            opcode &= 0xf;
        
          read_instructions((unsigned char**)&src, &opcode, &offset, &length);
        } 
    }
  return 0;
}
