/* - NASTRANFIND - Copyright (C) 2016-2017 Sebastien Vavassori
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

class StringHelper
{
public:
    static void removeCharsFromString(std::string &text, const std::string &charsToRemove);

    static char* trim_right( char* s, const char* delimiters );
    static char* trim_left( char* s, const char* delimiters );
    static char* trim( char* s, const char* delimiters = " \f\n\r\t\v");

    static bool hasSpaces(const std::string &text);
    static bool contains(const std::string &text, const std::string &searchedText);
    static int count(const std::string &text, const std::string &searchedText);

    static int findNext(const std::string &text,
                        const std::string &searchedText,
                        const std::string::size_type from);

    static int countChar( const char* text, const char* characters );

};


#endif // STRING_HELPER_H
