/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2009 Free Software Foundation, Inc.                        */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/**
 *     \file       encode.h
 *     \brief      Encoding function prototypes
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)

 */

#ifndef ENCODE_H
#define ENCODE_H

#include "bits.h"
#include "dwg.h"

int
dwg_encode_chains(Dwg_Data *dwg_struct, Bit_Chain *bitaro);

void
dwg_encode_entity(Dwg_Object *obj, Bit_Chain *dat);

void
dwg_encode_object(Dwg_Object *obj, Bit_Chain *dat);

void
dwg_encode_header_variables(Bit_Chain *dat, Dwg_Data *dwg);

void
dwg_encode_handleref(Bit_Chain *dat, Dwg_Object *obj, Dwg_Data *dwg,
                     Dwg_Object_Ref *ref);

void 
dwg_encode_handleref_with_code(Bit_Chain *dat, Dwg_Object *obj, Dwg_Data *dwg,
                               Dwg_Object_Ref *ref, int code);

void
dwg_encode_add_object(Dwg_Object *obj, Bit_Chain *dat,
                      long unsigned int address);


#endif
