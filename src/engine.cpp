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

#include "engine.h"

#include "global.h"
#include "fileinfo.h"
#include "stringhelper.h"
#include "systemdetection.h"

#include <algorithm> // transform(), powl()
#include <sstream>
#include <stdio.h>

#if defined(Q_OS_WIN)
#  include <windows.h>
#elif defined(Q_OS_UNIX)
#  include <limits.h>
#endif

using namespace std;

/*! \class Engine
 *  \brief The class Engine is a search engine for finding occurences
 *         through a INCLUDE file tree.
 */

/*! \brief Constructor.
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
        return;
    }

    /* **************************** */
    /* Get the working directory    */
    /* **************************** */
    const string pwd = FileInfo::canonicalFilePath(fullFileName);
    const string filename = FileInfo::fileName(fullFileName);

    appendFileName(filename, string(), -1);

    /* **************************** */
    /* For each INCLUDE file        */
    /* **************************** */
    for (stringlist::size_type i = 0; i < m_files.size() ; ++i) {

        const string currentFileName = m_files.at(i);

        string current_fullfilename;
        if (FileInfo::isRelativePath(currentFileName)) {
            current_fullfilename = FileInfo::concat(pwd, currentFileName);
        } else {
            current_fullfilename = currentFileName;
        }

        FILE *fp = fopen( currentFileName.c_str(), "rb" ); // read as binary
        if (fp==NULL) {
            string error_msg;
            error_msg += STR_ERR_CANNOT_OPEN + currentFileName + STR_ERR_QUOTE_END;
            m_errors.push_back( error_msg );

            Result& result = m_results[ currentFileName ];
            stringlist& occurrences = result.occurrences;
            occurrences.push_back( STR_ERR_MISSING_FILE );

        } else {

            const size_t blockSize = 1024;
            char data[blockSize];
            size_t lenFile = 0;
            std::string wholeFileContent = "";
            do {
                lenFile = fread(data, 1, blockSize - 1, fp);
                if (lenFile <= 0) break;

                if (lenFile >= blockSize - 1)
                    data[blockSize - 1] = '\0';
                else
                    data[lenFile - 1] = '\0';

                wholeFileContent += data;

            } while (lenFile > 0);

            fclose(fp);
            istringstream infile(wholeFileContent);
            find( &infile, searchedText, currentFileName );
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

    do {
        searchText(line, searchedText, currentFileName, currentLineNumber);
        ++currentLineNumber;

        string childFileName = searchInclude(iodevice);

        if( !childFileName.empty() ){
            appendFileName(childFileName, currentFileName, currentLineNumber);
        }
    } while( std::getline((*iodevice), line) );
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Return the filepath of the 'INCLUDE' statement, if the
 *         given \a text is an 'INCLUDE' statement.
 * Otherwise returns an empty string.
 *
 * If the 'INCLUDE' statement is declared over several lines, the function
 * returns the entire filepath without the line breaks.
 */
const string Engine::searchInclude(istream * const iodevice) const
{
    const streampos oldpos = iodevice->tellg();  // stores the position

    {
        string keyword( 7, '\0');
        iodevice->read(&keyword[0], 7 );

        std::transform( keyword.begin(), keyword.end(), keyword.begin(), ::toupper );
        if( keyword != "INCLUDE" ) {
            iodevice->clear();
            iodevice->seekg(oldpos);
            return string();
        }
    }

    /* Magic Number 10:                                           */
    /*  -> increases buffer to store quotes and trimming space(s) */
#if defined(Q_OS_WIN)
	string text(MAX_PATH + 10, '\0');
#else
	string text(PATH_MAX + 10, '\0');
#endif

    iodevice->read( &text[0], text.length() );
    iodevice->clear();
    iodevice->seekg(oldpos);

    /* Remove all the CR and LF */
    StringHelper::removeCharsFromString( text, "\r\n" );

    auto p = text.begin(); // + 7;

    /* The first char must be a white space */
    if ( (*p) != ' ' && (*p) != '\t')
        return string();

    while( ((*p) == ' ' || (*p) == '\t') && p != text.end() ){
        ++p;
    }

    if( ((*p) == '\'' || (*p) == '\"') && p != text.end() ) {
        /* Read the buffer until reaching the ending quote */
        auto quoteBegin = p;
        ++p;
        auto pos = p;
        auto len = 0;
        while( p != text.end() ){
            if( (*p) == (*quoteBegin) ){
                string ret = text.substr(pos - text.begin(), len);
                return ret;
            }
            ++len;
            ++p;
        }
        /* At this point, the ending quote was not found */
    }
    return string();
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
    if( currentLineNumber < 1)
        return;

    if( !searchedText.empty() && !currentFileName.empty()) {

        /* Remark : RegExp is not used here, to avoid unwanted RegExp injection */
        /* instead use of a string literal comparison, with a conversion        */
        /* to Upper Case to compare strings in a case insensitivity manner.     */

        int found = StringHelper::count(text, searchedText);
        if ( (found > 0) || StringHelper::hasSpaces(searchedText)) {

            /* Convert an integer into a length-fixed string */
            char buffer[ C_LINE_NUMBER_BUFFER_SIZE + 1 ];       // 8 digits + 1 '\0'
            if( currentLineNumber < C_LINE_NUMBER_MAX_NUMBER ) {
                sprintf( buffer, C_LINE_NUMBER_FORMAT_INT, currentLineNumber);
            } else {
                sprintf( buffer, C_LINE_NUMBER_FORMAT_CHAR, '.');
            }
            string str = string("line")
                    + string(buffer)
                    + string(": ")
                    + text;

            Result& result = m_results[ currentFileName ];
            stringlist& occurrences = result.occurrences;
            occurrences.push_back( std::move(str) );

            result.occurrenceCount += found;
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
    for( stringlist::const_iterator it = m_files.begin(); it != m_files.end(); ++it )  {
        if ( filenameToBeInserted == (*it) ) {

            string lineNumber = std::to_string(currentLineNumber) ;
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
    Result emptyResult;
    m_results[ filenameToBeInserted ] = emptyResult;
}

/******************************************************************************
 ******************************************************************************/
const string Engine::linkAt(const string::size_type index) const
{
    if (index < m_files.size()) {
        return m_files.at(index);
    }
    return string();
}

/******************************************************************************
 ******************************************************************************/
const string Engine::errorAt(const string::size_type index) const
{
    if (index < m_errors.size()) {
        return m_errors.at(index);
    }
    return string();
}


/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the number of lines found, in the file and all its includes.
 * \sa resultCount(), resultCountLines(), occurrenceCount()
 */
stringlist::size_type Engine::resultCountAll() const
{
    auto value = 0;
    for( stringlist::const_iterator it = m_files.begin(); it != m_files.end(); ++it ) {
        const string& file = (*it);
        value += resultCount(file);
    }
    return value;
}

/*! \brief Returns the number of lines needed to correctly display all the occurences
 *  for the given \a filename.
 * Indeed, an occurence can be display on several lines, i.e. continued Deck Entry.
 * \sa resultCount(), resultCountAll()
 */
stringlist::size_type Engine::resultCountLines(const string &filename) const
{
    /// \todo resultCountLines( ) to be implemented
    return resultCount(filename);
}


/*! \brief Returns the number of occurences for the given \a filename.
 * \sa resultCountAll(), resultCountLines()
 */
stringlist::size_type Engine::resultCount(const string &filename) const
{
    if( m_results.count(filename) > 0 ) {
        const Result& result = m_results.at(filename);
        const stringlist& occurrences = result.occurrences;
        return occurrences.size();
    }
    return 0;
}

/*! \brief Returns the occurence at the given \a index, for the given \a filename.
 */
const string Engine::resultAt(const string &filename, const stringlist::size_type index) const
{
    if( m_results.count(filename) > 0 ) {
        const Result& result = m_results.at(filename);
        const stringlist& occurrences = result.occurrences;
        if (index < occurrences.size()) {
            return occurrences.at(index);
        }
    }
    return string();
}

/******************************************************************************
 ******************************************************************************/
stringlist::size_type Engine::occurrenceCountAll() const
{
    auto value = 0;
    for( stringlist::const_iterator it = m_files.begin(); it != m_files.end(); ++it ) {
        const string& file = (*it);
        value += occurrenceCount(file);
    }
    return value;
}

stringlist::size_type Engine::occurrenceCount(const string &filename) const
{

    if( m_results.count(filename) > 0 ) {
        const Result& result = m_results.at(filename);
        return result.occurrenceCount;
    }
    return 0;
}
