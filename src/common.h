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
 *     \file       common.h
 *     \brief      Common general functions and macro
 *     \author     written by Felipe Castro
 *     \author     modified by Felipe Corrêa da Silva Sances
 *     \author     modified by Rodrigo Rodrigues da Silva
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)

 */

#ifndef COMMON_H
#define COMMON_H

#define VERSION(v) if (dat->version == v)
/** DWG version type */

typedef enum DWG_VERSION_TYPE
{
  R_BEFORE, R_13, R_14, R_2000, R_2004, R_2007, R_2010, R_AFTER
} Dwg_Version_Type;

extern char version_codes[8][7];

/** Data types (including compressed forms) used trough the file */
typedef enum DWG_DATA_TYPE
{
