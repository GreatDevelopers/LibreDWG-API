/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2010 Free Software Foundation, Inc.                        */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

/*
 *     \file       dwg.i
 *     \brief      SWIG interface file
 *     \author     written by Rodrigo Rodrigues da Silva
 *     \author     ideas contributed by James Michael DuPont
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

%module libredwg

%{
#include "../../src/dwg.h"
%}

%include "carrays.i"
%array_functions(Dwg_Object, Dwg_Object_Array);

#include "../../src/dwg.h"
