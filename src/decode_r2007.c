/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2010 Free Software Foundation, Inc.                        */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/**
 *     \file       decode_r2007.c
 *     \brief      R2007 decode functions
 *     \author     written by Till Heuschmann
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#include "classes.h"
#include "compress_decompress.h"
#include "decode_r2007.h"
#include "handle.h"
#include "header.h"
#include "logging.h"
#include "section_locate.h"
#include "resolve_pointers.h"

unsigned int loglevel;

#define DWG_LOGLEVEL loglevel

char *

decode_rs(const char *src, int block_count, int data_size)
{
  int i, j;
  const char *src_base = src;
  char *dst_base, *dst;  
  
  dst_base = dst = (char*) malloc(block_count *data_size);

  
      src = ++src_base;
    }
  return (dst_base);
}

/** Read System Section Page */
char *
read_system_page(Bit_Chain *dat, int64_t size_comp, int64_t size_uncomp,
                 int64_t repeat_count)
{
  int i;
  int64_t pesize;      // Pre RS encoded size
  int64_t block_count; // Number of RS encoded blocks
  int64_t page_size;

  char *rsdata;        // RS encoded data
  char *pedata;        // Pre RS encoded data
  char *data;          // The data RS unencoded and uncompressed
  
  // Round to a multiple of 8
  pesize = ((size_comp + 7) & ~7) * repeat_count;
  
  // Divide pre encoded size by RS k-value (239)

  block_count = (pesize + 238) / 239;
  
  // Multiply with codeword size (255) and round to a multiple of 8
  page_size = (block_count * 255 + 7) & ~7;
  
  data = (char*) malloc(size_uncomp + page_size);


  if (data == 0)
    {
      //TODO: report error
      return 0;
    }

  rsdata = &data[size_uncomp];
  
    decompress_R2007(data, size_uncomp, pedata, size_comp);

  else
    memcpy(data, pedata, size_uncomp);
  
  free(pedata);
  return data;
}

/** Read Data Section Page */
int
read_data_page(Bit_Chain *dat, unsigned char *decomp, int64_t page_size,
               int64_t size_comp, int64_t size_uncomp)
{
  int i;
  int64_t pesize;      // Pre RS encoded size
  int64_t block_count; // Number of RS encoded blocks
  char *rsdata;        // RS encoded data
  char *pedata;        // Pre RS encoded data
  
  // Round to a multiple of 8
  pesize = ((size_comp + 7) & ~7);
  block_count = (pesize + 0xFB - 1) / 0xFB;
  rsdata = (char*)malloc(page_size * sizeof(char));


  if (rsdata == NULL)
    return 1;
  

  pedata = decode_rs(rsdata, block_count, 0xFB);
  
read_data_section(Bit_Chain *sec_dat, Bit_Chain *dat,
                  r2007_section *sections_map, r2007_page *pages_map,
                  int64_t hashcode)

{
  r2007_section *section;
  r2007_page *page;
  int64_t max_decomp_size;
  unsigned char *decomp;
  int i;
  
  section = get_section(sections_map, hashcode);

  if (section == NULL)
    return 1;        // Failed to find section

  max_decomp_size = section->data_size;
  decomp = (unsigned char *)malloc(max_decomp_size * sizeof(char));

  if (decomp == NULL)
    return 2;      // No memory

      dat->byte = page->offset; 

      if (read_data_page(dat, &decomp[section->pages[i]->offset], page->size, 
                         section->pages[i]->comp_size,
                         section->pages[i]->uncomp_size) != 0)

        {
          free(decomp);
          return 4;   // Failed to read page
        }
    }

  sec_dat->bit     = 0;
  sec_dat->byte    = 0;
  sec_dat->chain   = decomp;
  sec_dat->size    = max_decomp_size;
  sec_dat->version = dat->version;    

  return 0;
}

#define bfr_read_int16(_p)   *((int16_t*)_p);  _p += 2;
#define bfr_read_int64(_p)   *((int64_t*)_p);  _p += 8;

void
bfr_read(void *dst, char **src, size_t size)
{
  memcpy(dst, *src, size);
  *src += size;
}

DWGCHAR *
bfr_read_string(char **src)
{
  uint16_t *ptr  = (uint16_t*)*src;  

  int32_t length = 0, wsize;
  DWGCHAR *str, *str_base;
  int i;
  
  while (*ptr != 0)
    {
      ptr++;
      length++;
    }
  wsize = length * sizeof(DWGCHAR) + sizeof(DWGCHAR);
  str   = str_base = (DWGCHAR*) malloc(wsize);
  ptr   = (uint16_t*)*src;


  *src += length * 2 + 2;
  *str = 0;
  return str_base;
}

/** Read Page Map */
r2007_page *
read_pages_map(Bit_Chain *dat, int64_t size_comp, int64_t size_uncomp,
               int64_t correction)
{
  char *data, *ptr, *ptr_end;  
  r2007_page *page, *pages = 0, *last_page = 0;
  int64_t offset = 0x480;   //dat->byte;
  //int64_t index;


  data = read_system_page(dat, size_comp, size_uncomp, correction);
  
  if (data == NULL)
    return NULL;  

  ptr     = data;
  ptr_end = data + size_uncomp;
  LOG_TRACE("\n System Section (Pages Map) \n")

  


      if (page == NULL)
        {
          //TODO: report error
          free(data);
          pages_destroy(pages);
          return NULL;
        }
      page->size   = bfr_read_int64(ptr);
      page->id     = bfr_read_int64(ptr);
      page->offset = offset;

      offset += page->size;
      //index  = page->id > 0 ? page->id : -page->id;

      LOG_TRACE("\n Page \n")
      LOG_TRACE("size:   0x%jd\n", page->size)
      LOG_TRACE("id:     0x%jd\n", page->id)
      LOG_TRACE("offset: 0x%jd\n", page->offset)


      page->next = 0;      
    
      if (pages == 0)
        pages = last_page = page;
      else
        {
          last_page->next = page;
          last_page = page;
        }           
    }
  free(data);
  return pages;
}

/** Lookup a page in the page map */
r2007_page *

get_page(r2007_page *pages_map, int64_t id)
{
  r2007_page *page = pages_map;
  
  while (page != NULL)
    {
      if (page->id == id)
        break;

      page = page->next;
    }
  return page;
}

/** Destroy Pages */

void
pages_destroy(r2007_page *page)
{
  r2007_page *next;
  
  while (page != 0)
    {
      next = page->next;
      free(page);
      page = next;
    }
}

/** Destroy Sections */

void
sections_destroy(r2007_section *section)
{
  r2007_section *next;
  
  while (section != 0)
    {
      next = section->next;
    
      if (section->pages != 0)
        {
          while (section->num_pages-- > 0)
            {
              free(section->pages[section->num_pages]);
            }
          free(section->pages);
        }

      free(section);
      section = next;
    }
}

/** Read File Header */
void 
read_file_header(Bit_Chain *dat, r2007_file_header *file_header)
{
  char *pedata, data[0x3d8];
  //int64_t seqence_crc, seqence_key, compr_crc;

  int32_t compr_len;
  int i;
  
  dat->byte = 0x80;

  read_file_header(dat, &file_header);
  
  /* Pages Map */
  dat->byte += 0x28;    // overread check data
  dat->byte += file_header.pages_map_offset;
  pages_map = read_pages_map(dat, file_header.pages_map_size_comp,
                             file_header.pages_map_size_uncomp,
                             file_header.pages_map_correction); 
  
  /* Sections Map */

  page = get_page(pages_map, file_header.sections_map_id);
  
  if (page != NULL)
    {
      dat->byte = page->offset;
      sections_map = read_sections_map(dat, file_header.sections_map_size_comp,
                                       file_header.sections_map_size_uncomp,
                                       file_header.sections_map_correction);
    }

  /* Classes */
  read_R2007_section_classes(dat, dwg, sections_map, pages_map);
   
  /* Header */
  read_R2007_section_header(dat, dwg, sections_map, pages_map);
   
  /* Handles */
  //read_R2007_section_handles(dat, dwg, sections_map, pages_map);
  
  /* Destroy sections and pages */
  pages_destroy(pages_map);
  sections_destroy(sections_map);   
  return 0;
}

/** Decode R2007 version */
int
decode_R2007(Bit_Chain *dat, Dwg_Data *dwg)
{
  int i;
  uint8_t  ver_string, sig, dwg_ver, maint_release_ver, acad_maint_ver;
  uint32_t preview_address, security_type, unknown_long, summary_info_address,
           vba_proj_address, app_info_address;

  /* Version string */
  dat->byte = 0x00;
  LOG_INFO("Version string: ")
