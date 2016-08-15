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
#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <string>

/* ******************************************* */
/* reimplementation of some standard functions */
/* ******************************************* */

class StringHelper
{
public:
    static void removeCharsFromString(std::string &text, const std::string &charsToRemove);

    static char* trim_right( char* s, const char* delimiters );
    static char* trim_left( char* s, const char* delimiters );
    static char* trim( char* s, const char* delimiters = " \f\n\r\t\v");

    static bool containsInvisibleChar(const std::string &text);
    static bool containsInsensitive(const std::string &text, const std::string &str);
    static int findNextInsensitive(const std::string &text, const std::string &str, const int from);

    static int charCount ( const char * string, const char * characters );

};


#endif // STRING_HELPER_H
