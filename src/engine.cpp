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
#include "engine.h"

#include "global.h"
#include "fileinfo.h"
#include "stringhelper.h"

#include <algorithm> // fill()
#include <fstream>
#include <istream>
#include <stdio.h>
#include <regex>

using namespace std;

/*! \class Engine
 *  \brief The class Engine is a search engine for finding occurences
 *         through a INCLUDE file tree.
 */
Engine::Engine()
{
    this->clear();
}

void Engine::clear()
{
    m_files.clear();
    m_results.clear();
    m_errors.clear();
}

/*****************************************************************************
 *****************************************************************************/
/*!  \brief Search all the occurences of the given \a searchedText
 *        in the given \a fullFileName and all the INCLUDE files.
 */
void Engine::find(const string &fullFileName, const string &searchedText )
{
    this->clear();

    if( fullFileName.empty() ){
        m_errors.push_back( STR_ERR_EMPTY_FILENAME );
        return ;
    }

    /* **************************** */
    /* Get the working directory    */
    /* **************************** */
    std::string pwd = FileInfo::canonicalFilePath(fullFileName);
    std::string filename = FileInfo::fileName(fullFileName);

    appendFileName(filename, std::string(), -1);

    /* **************************** */
    /* For each INCLUDE file        */
    /* **************************** */
    for( stringlist::iterator it = m_files.begin(); it != m_files.end(); ++it )  {

        string currentFileName = *it ;

        std::string current_fullfilename;
        if (FileInfo::isRelativePath(currentFileName)) {
            current_fullfilename = FileInfo::concat(pwd, currentFileName);
        } else {
            current_fullfilename = currentFileName;
        }

        ifstream infile( current_fullfilename.c_str() );
        if( !infile.is_open() ){

            string error_msg;
            error_msg += STR_ERR_CANNOT_OPEN + currentFileName + STR_ERR_QUOTE_END;
            m_errors.push_back( error_msg );
            m_results[currentFileName].push_back( STR_ERR_MISSING_FILE );

        } else {

            find( &infile, searchedText, currentFileName );

            infile.close();
        }
    }
}

/*****************************************************************************
 *****************************************************************************/
void Engine::find(istream * const iodevice,
                  const string &searchedText ,
                  const string &currentFileName)
{
    int currentLineNumber = 0;
    string line;

    /* **************************** */
    /* For each line in the file    */
    /* **************************** */
    while (getline( (*iodevice), line )) {  // while not a EOF char

        ++currentLineNumber;
        searchText(line, searchedText, currentFileName, currentLineNumber);

        string childFileName = searchInclude(line);
        if( childFileName.empty() ){
            childFileName = searchIncludeMulti( line,
                                                &currentLineNumber,
                                                currentFileName,
                                                searchedText,
                                                iodevice );
        }

        if( !childFileName.empty() ){
            appendFileName(childFileName, currentFileName, currentLineNumber);
        }
    }
}


/******************************************************************************
 ******************************************************************************/
string Engine::searchInclude(const string &text)
{
    string ret;
    /// \todo UNIX : reimplement regex if GCC < 4.9 ?
    regex patternInclude {R"(^INCLUDE\s+['"]([^'"]*)['"].*$)", regex::icase};
    smatch matches;
    bool found = regex_match(text, matches, patternInclude);
    if (found) {
        ret = matches[1];
    }
    return ret;
}

/******************************************************************************
 ******************************************************************************/
string Engine::searchIncludeMulti(const string &text,
                                  int * const currentLineNumber,
                                  const string &currentFileName,
                                  const string &searchedText,
                                  istream * const iodevice )
{
    string ret;

    /// \todo UNIX : reimplement regex if GCC < 4.9 ?
    regex patternIncludeBegin {R"(^INCLUDE\s+['"]([^'"]*)$)", regex::icase};
    regex patternIncludeEnd   {R"(^([^'"]*)['"].*$)", regex::icase};

    smatch matches;
    bool found = regex_match(text, matches, patternIncludeBegin);
    if (found) {
        ret = matches[1];

        /* continue reading until reaching the next ' or " */
        found = false;
        string line;
        while( !found && getline( (*iodevice), line) ){

            ++(*currentLineNumber);
            searchText(line, searchedText, currentFileName, (*currentLineNumber));

            found = regex_match(line, matches, patternIncludeEnd);
            if (found) {
                ret += matches[1];
            }else{
                ret += line;
            }
        }

    }
    return ret;
}



/******************************************************************************
 ******************************************************************************/
/*! \brief Returns true if the given \a searchedText is found in the given \a text.
 */
void Engine::searchText(const string &text,
                        const string &searchedText,
                        const string &currentFileName,
                        const int currentLineNumber)
{
    if( !searchedText.empty() && !currentFileName.empty()) {

        /* Remark : RegExp is not used here, to avoid unwanted RegExp injection */
        /* instead use of a string literal comparison, with a conversion        */
        /* to Upper Case to compare strings in a case insensitivity manner.     */

        if ( StringHelper::containsInvisibleChar(searchedText) ||
             StringHelper::containsInsensitive(text, searchedText) ) {

            /* Convert an integer into a length-fixed string */
            char buffer[ C_LINE_NUMBER_BUFFER_SIZE ];
            if( currentLineNumber < C_LINE_NUMBER_MAX_NUMBER ) {
                sprintf( buffer, C_LINE_NUMBER_FORMAT_INT, currentLineNumber);
            } else {
                sprintf( buffer, C_LINE_NUMBER_FORMAT_CHAR, '.');
            }
            string result = "line"
                    + string(buffer)
                    + ": "
                    + text;
            m_results[currentFileName].push_back( result );
        }
    }
}

/******************************************************************************
 ******************************************************************************/
void Engine::appendFileName(const string &filenameToBeInserted,
                            const string &currentFileName,
                            const int currentLineNumber)
{
    /* Check if the filename is not already referenced */
    for( stringlist::iterator it = m_files.begin(); it != m_files.end(); ++it )  {
        if ( filenameToBeInserted == (*it) ) {

            std::string lineNumber = std::to_string(currentLineNumber) ;
            string error_msg
                    = STR_ERR_CYCLIC
                    + currentFileName
                    + STR_ERR_AT_LINE
                    + lineNumber
                    + STR_ERR_END;

            m_errors.push_back( error_msg );
            return;
        }
    }

    /* Append the file to the file list */
    m_files.push_back( filenameToBeInserted );
    stringlist emptyStringList;
    m_results[ filenameToBeInserted ] = emptyStringList;
}

/******************************************************************************
 ******************************************************************************/
int Engine::linkCount() const
{
    return m_files.size();
}

std::string Engine::linkAt(const int index) const
{
    int row = 0;
    for(stringlist::const_iterator it = m_files.begin(); it != m_files.end(); ++it ) {
        if (row == index) {
            return (*it);
        }
        row++;
    }
    return std::string();
}

/******************************************************************************
 ******************************************************************************/
int Engine::errorCount() const
{
    return m_errors.size();
}

std::string Engine::errorAt(const int index) const
{
    int row = 0;
    for(stringlist::const_iterator it = m_errors.begin(); it != m_errors.end(); ++it ) {
        if (row == index) {
            return (*it);
        }
        row++;
    }
    return std::string();
}


/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the number of occurences found, in the file and all its includes.
 * \sa resultCount(), resultCountLines()
 */
int Engine::resultCountAll() const
{
    int value = 0;
    for( int i = 0; i < linkCount(); ++i ) {
        const string file = linkAt(i);
        value += resultCount(file);
    }
    return value;
}

/*! \brief Returns the number of lines needed to correctly display all the occurences
 *  for the given \a filename.
 * Indeed, an occurence can be display on several lines, i.e. continued Deck Entry.
 * \sa resultCount(), resultCountAll()
 */
int Engine::resultCountLines(const std::string &filename) const
{
    /// \todo resultCountLines( ) to be implemented
    return resultCount(filename);
}


/*! \brief Returns the number of occurences for the given \a filename.
 * \sa resultCountAll(), resultCountLines()
 */
int Engine::resultCount(const string &filename) const
{    
    /// \bug libc -> map::at() has a weird bug 'std::out_of_range' when const scope
    /// \bug BUGFIX: use of [] instead of at()
    typedef std::map<std::string, stringlist > myMap;
    myMap rrr = (myMap)m_results;
    stringlist res = rrr[filename];
    return res.size();
}

/*! \brief Returns the occurence at the given \a index, for the given \a filename.
 */
std::string Engine::resultAt(const string &filename, const int index) const
{
    /// \bug libc -> map::at() has a weird bug 'std::out_of_range' when const scope
    /// \bug BUGFIX: use of [] instead of at()
    typedef std::map<std::string, stringlist > myMap;
    myMap rrr = (myMap)m_results;
    stringlist res = rrr[filename];

    int row = 0;
    for(stringlist::const_iterator it = res.begin(); it != res.end(); ++it ) {
        if (row == index) {
            return (*it);
        }
        row++;
    }
    return std::string();
}

