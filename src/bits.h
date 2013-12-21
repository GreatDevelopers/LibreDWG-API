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
 *     \file       bits.h
 *     \brief      Low level read and write function prototypes
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

/* The position of bits within bytes is numerically ordered as depicted below:
 *
 * position: 01234567 01234567 01234567 ...
 * bits    : 76543210 76543210 76543210 ...
 *          \______/ \______/ \______/
 *           byte 1   byte 2   byte 3  ...

 */

#ifndef BITS_H
#define BITS_H

#ifndef DWG_H
#include "dwg.h"
#endif

#include "common.h"

/** Structure for DWG-files raw data storage */

typedef struct _bit_chain
{
  unsigned char *chain;
  long unsigned int size;
  long unsigned int byte;
  unsigned char bit;
  Dwg_Version_Type version;
  long unsigned int handles_address;
  long unsigned int string_byte;
  unsigned char string_bit;
} Bit_Chain;

/* Functions for raw data manipulations
 */
void
bit_advance_position(Bit_Chain *dat, int salto);

BITCODE_B
bit_read_B(Bit_Chain *bit_datenaro);

void
bit_write_B(Bit_Chain *bit_datenaro, unsigned char value);

BITCODE_BB
bit_read_BB(Bit_Chain *bit_datenaro);

void
bit_write_BB(Bit_Chain *bit_datenaro, unsigned char value);

BITCODE_4BITS
bit_read_4BITS(Bit_Chain *dat);

void
bit_write_4BITS(Bit_Chain *dat, unsigned char value);

BITCODE_RC
bit_read_RC(Bit_Chain *bit_datenaro);

void
bit_write_RC(Bit_Chain *bit_datenaro, unsigned char value);

BITCODE_RS
bit_read_RS(Bit_Chain *bit_datenaro);

void
bit_write_RS(Bit_Chain *bit_datenaro, unsigned int value);

BITCODE_RL
bit_read_RL(Bit_Chain *bit_datenaro);

void
bit_write_RL(Bit_Chain *bit_datenaro, long unsigned int value);

BITCODE_RD
bit_read_RD(Bit_Chain *bit_datenaro);

void
bit_write_RD(Bit_Chain *bit_datenaro, double value);

/* Functions for manipulating compacted data 
 */
BITCODE_BS
bit_read_BS(Bit_Chain *bit_datenaro);

void
bit_write_BS(Bit_Chain *bit_datenaro, unsigned int value);

BITCODE_BL
bit_read_BL(Bit_Chain *bit_datenaro);

void
bit_write_BL(Bit_Chain *bit_datenaro, long unsigned int value);

BITCODE_BD
bit_read_BD(Bit_Chain *bit_datenaro);

void
bit_write_BD(Bit_Chain *bit_datenaro, double value);

BITCODE_MC
bit_read_MC(Bit_Chain *bit_datenaro);

void
bit_write_MC(Bit_Chain *bit_datenaro, long int value);

BITCODE_MS
bit_read_MS(Bit_Chain *bit_datenaro);

void
bit_write_MS(Bit_Chain *bit_datenaro, long unsigned int value);

void
bit_read_BE(Bit_Chain *bit_datenaro, double *x, double *y, double *z);

void
bit_write_BE(Bit_Chain *bit_datenaro, double x, double y, double z);

BITCODE_DD
bit_read_DD(Bit_Chain *bit_datenaro, double antauxdifinajxo);

void
bit_write_DD(Bit_Chain *bit_datenaro, double value, double antauxdifinajxo);

BITCODE_BT
bit_read_BT(Bit_Chain *bit_datenaro);

void
bit_write_BT(Bit_Chain *bit_datenaro, double value);

int
bit_read_H(Bit_Chain *bit_datenaro, Dwg_Handle *handle);

void
bit_write_H(Bit_Chain *dat, Dwg_Handle *handle);

unsigned int
bit_read_CRC(Bit_Chain *bit_datenaro);

int
bit_check_CRC(Bit_Chain *bit_datenaro, long unsigned int start_address,
              unsigned int semo);

unsigned int
bit_write_CRC(Bit_Chain *dat, long unsigned int start_address,
              unsigned int semo);

unsigned char *
bit_read_T(Bit_Chain *dat);

DWGCHAR *
bit_read_TU(Bit_Chain *dat);

unsigned char *
bit_read_TV(Bit_Chain *dat, Bit_Chain *dat_string);

void
bit_write_T(Bit_Chain *dat, unsigned char *chain);

void
bit_write_TV(Bit_Chain *dat, unsigned char *chain);

long unsigned int
bit_read_L(Bit_Chain *dat);

void
bit_write_L(Bit_Chain *dat, long unsigned int value);

void
bit_read_CMC(Bit_Chain *dat, Dwg_Color *color, Bit_Chain *dat_string);

void
bit_write_CMC(Bit_Chain *dat, Dwg_Color *color);

int
bit_search_sentinel(Bit_Chain *dat, unsigned char sentinel[16]);

void
bit_write_sentinel(Bit_Chain *dat, unsigned char sentinel[16]);

void
bit_chain_alloc(Bit_Chain *dat);

void
bit_print(Bit_Chain *dat, long unsigned int size);

void
bit_explore_chain(Bit_Chain *dat, long unsigned int size);

/* Functions for CRC 
 */ 
unsigned int
bit_ckr8(unsigned int dx, unsigned char *adr, long n);

unsigned long int
bit_ckr32 (unsigned int seed, unsigned char *adr, unsigned int n);

unsigned int
calculate_normal_crc(unsigned char data, unsigned long int crc);

unsigned int
calculate_mirror_crc(unsigned char data, unsigned long int crc);


#endif
