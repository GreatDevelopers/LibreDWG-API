/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2009 Free Software Foundation, Inc.                        */
/*  Copyright (C) 2010 Thien-Thi Nguyen                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

/**
 *     \file       test_logging.c
 *     \version    
 *     \copyright  GNU General Public License (version 3 or later)
 */

#define DWG_LOGLEVEL DWG_LOGLEVEL_INFO

#include "logging.h"

int main()
{
  LOG(NONE, "Errei por %d!", 1) //never use this trick!
  LOG_ERROR("Errei por %d!", 2)
  LOG_INFO("Errei por %d!",  3)
  LOG_TRACE("Errei por %d!", 4)
  LOG_ALL("Errei por %d!",   5)
  return 0;
}
