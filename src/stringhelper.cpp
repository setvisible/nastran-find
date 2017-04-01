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
 * std::string str("(555) 666-7777");
 * removeCharsFromString( str, "()-" );   // str == "555 6667777"
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
/*! \brief Returns true if the given \a text contains only whitespaces and tabs.
 */
bool StringHelper::hasSpaces(const string &text)
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
/*! \brief Returns the position of the first character of \a searchedText
 *         in the \a text.
 *
 * In particular:
 *   \li Return 0, if the \a text starts with the \a searchedText.
 *   \li Return n, where n is the position of the first character of \a searchedText in \a text.
 *   \li Return string::npos, if \a searchedText is not found in \a text.
 *
 * \remark This function performs a *case insensitive* string comparison.
 */
int StringHelper::findNext(const string &text, const string &searchedText, const string::size_type from)
{
    if( text.empty() || searchedText.empty() )
        return string::npos;

    if( from == string::npos || from >= text.length() )
        return string::npos;

    bool found = false;
    auto p = text.begin() + from;
    auto q = searchedText.begin();
    while( p != text.end() && q != searchedText.end() ){
        if( toupper(*p) == toupper(*q) ){
            ++p;
            ++q;
            found = true;
            continue;
        }else if( found ){
            auto backward = q - searchedText.begin();
            p -= backward;
            q -= backward;
            found = false;
        }
        ++p;
    }
    if( found && q==searchedText.end() ){
        auto ret = p - text.begin() - searchedText.length();
        return ret;
    }
    return string::npos;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Return true if the given \a searchedText is found
 *         in the given \a text. Otherwise return false.
 *
 * \remark This function performs a case-insensitive string comparison.
 */
bool StringHelper::contains(const string &text, const string &searchedText)
{
    return (findNext(text, searchedText, 0) != (int)string::npos);
}


/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Returns the number of (*no* overlapping)
 *        occurrences of the given \a searchedText in the given \a text.
 */
int StringHelper::count(const std::string &text, const std::string &searchedText)
{
    int count = 0;
    const string::size_type length = searchedText.length();
    string::size_type pos = 0;
    while (true) {
        pos = findNext(text, searchedText, pos);
        if (pos == string::npos)
            break;
        pos += length;
        count++;
    }
    return count;
}


/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the number of \a characters in the \a text.
 */
int StringHelper::countChar( const char* text, const char* characters )
{
    int counter = 0;
    while (*text) {
        if (strspn(text, characters)) {
            counter++;
        }
        text++;
    }
    return counter;
}

