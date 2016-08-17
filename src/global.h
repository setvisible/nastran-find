/* - NASTRANFIND - Copyright (C) 2016 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GLOBAL_H
#define GLOBAL_H

/* General */
#define C_APPLICATION_NAME      "NastranFind"
#define C_APPLICATION_SUBTITLE  " - An Interactive Search Engine for Nastran"

/* Search Box Size */
#define C_SEARCH_SIZE 32 // characters

/*                                                                */
/* Here we make an assumption:                                    */
/*                                                                */
/* A Nastran Bulk file contains less than 100 million of lines    */
/* (10^8), per file.                                              */
/*                                                                */
/* If the file contains more than 99999999 lines,                 */
/* NastranFind simply shows "line........:"                       */
/* because there is not enough place to show the line count.      */
/*                                                                */
/* To increase that limit, change the 5 DEFINE below.             */
/* To increase that limit, change the DEFINE values below.        */
/*                                                                */
#define C_LINE_NUMBER_BUFFER_SIZE 8
#define C_LINE_NUMBER_FORMAT_INT  "%8i"
#define C_LINE_NUMBER_FORMAT_CHAR "%8c"
#define C_LINE_NUMBER_MAX_NUMBER powl(10, C_LINE_NUMBER_BUFFER_SIZE )-1 /* == 10^8-1 */
#define C_LINE_NUMBER_WIDTH C_LINE_NUMBER_BUFFER_SIZE + 6  /* 6 == len("line") + len(": ") */


#endif  // GLOBAL_H
