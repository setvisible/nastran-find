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

#include "stringhelper.h"

#include <algorithm>
#include <string>
#include <cstring>

using namespace std;

/******************************************************************************
 ******************************************************************************/
/*! \brief Remove all the given \a charsToRemove from the given \a text.
 *
 * \code
 * std::string str("(555) 555-5555");
 * removeCharsFromString( str, "()-" );   // str == "555 5555555"
 * \endcode
 */
void StringHelper::removeCharsFromString( string &text, const string &charsToRemove )
{
   for ( unsigned int i = 0; i < charsToRemove.length(); ++i ) {
      text.erase( std::remove(text.begin(), text.end(), charsToRemove.at(i)), text.end() );
      text.resize( strlen( text.c_str() ) );
   }
}

/******************************************************************************
 ******************************************************************************/
#ifndef STRRSPN
/*! \brief A non-standard helper function
 *  Find c such that s=".*c[d]*", where [d]=delimiters and c in [d].
 *  If c is not found, it returns the argument s.
 */
static char* strrspn( const char* s, const char* delimiters )
{
    char* p = (char*)strchr( s, '\0' );
    while (p-- != s)
        if (!strchr( delimiters, *p ))
            break;
    return p + 1;
}
#endif

/******************************************************************************
 ******************************************************************************/
/*! \brief Trim the char array \a s from right by \a delimiters character(s).  
 * Return the trimmed result.
 */
char* StringHelper::trim_right( char* s, const char* delimiters )
{
    if (!s || !delimiters)
        return NULL;
    s[strrspn(s, delimiters) - s] = '\0';
    return s;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Trim the char array \a string from left by \a delimiters character(s). 
 * Return the trimmed result.
 */
char* StringHelper::trim_left( char* s, const char* delimiters )
{
    int i = strspn( s, delimiters );
    return s + i;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Trim the char array \a string from left and right by \a delimiters character(s). 
 * Return the trimmed result.
 */
char* StringHelper::trim( char* s, const char* delimiters)
{
    return trim_left( trim_right( s, delimiters ), delimiters );
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns true if the given \a text has at least one white space or tab.
 */
bool StringHelper::containsInvisibleChar(const string &text)
{
    auto p = text.begin();
    while (p != text.end()) {
        if( (*p) != ' ' && (*p) != '\t' ){
            return false;
        }
        ++p;
    }
    return true;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the location in the given \a text where
 *         the given \a searchedText starts.
 *
 * The function is case insensitive.
 */
int StringHelper::findNextInsensitive(const string &text, const string &str, const int from)
{
    if( text.empty() || str.empty() )
        return string::npos;

    if( from < 0 || from >= (int)text.length() )
        return string::npos;

    bool found = false;
    auto p = text.begin() + from;
    auto q = str.begin();
    while( p != text.end() && q != str.end() ){
        if( toupper(*p) == toupper(*q) ){
            ++p;
            ++q;
            found = true;
            continue;
        }else if( found ){
            auto backward = q - str.begin();
            p -= backward;
            q -= backward;
            found = false;
        }
        ++p;
    }
    if( found && q==str.end() ){
        auto ret = p - text.begin() - str.length();
        return ret;
    }
    return string::npos;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Case-insensitive string comparison.
 */
bool StringHelper::containsInsensitive(const string &text, const string &str)
{
    return (findNextInsensitive(text, str, 0) != (int)string::npos);
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the number of \a characters in the \a string. 
 */
int StringHelper::charCount( const char * string, const char * characters )
{
    int counter = 0;
    while (*string) {
        if(strspn(string, characters))
            counter++;
        string++;
    }
    return counter;
}

