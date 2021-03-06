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

#ifndef ENGINE_H
#define ENGINE_H

#include "result.h"

/* **************************************************************** */
/* Messages stored in header file is required for testing           */
static const char STR_ERR_EMPTY_FILENAME[] = "Error: empty filename. Need a valid DAT-file as argument.";
static const char STR_ERR_CANNOT_OPEN[]    = "Error: cannot open the file '";
static const char STR_ERR_CYCLIC[]         = "Error: cyclic reference in '";
static const char STR_ERR_MISSING_FILE[]   = "Error: no such file. Verify INCLUDE card?";
/// \todo static const char STR_ERR_DUPLICATE[]      = "Error: duplicate reference in '";
static const char STR_ERR_AT_LINE[]    = "' at line ";
static const char STR_ERR_QUOTE_END[]  = "'.";
static const char STR_ERR_END[]        = ".";
/* **************************************************************** */

class Engine
{
public:
    explicit Engine();

    /* Clear the previous search */
    void clear();

    /* Do a search */
    void find(const std::string &fullFileName, const std::string &searchedText );
    void find(std::istream * const iodevice,
              const std::string &searchedText,
              const std::string &currentFileName );

    /* Getters -> return the file hierarchy */
    const stringlist& files() const { return m_files; }
    std::string::size_type linkCount() const { return m_files.size(); }
    const std::string linkAt(const std::string::size_type index) const;

    /* Getters -> return the errors, if so */
    std::string::size_type errorCount() const { return m_errors.size(); }
    const std::string errorAt(const std::string::size_type index) const;

    /* Getters -> return the search results */
    const ResultMap& results() const { return m_results; }
    stringlist::size_type resultCountAll() const;
    stringlist::size_type resultCountLines(const std::string &filename) const;
    stringlist::size_type resultCount(const std::string &filename) const;
    const std::string resultAt(const std::string &filename, const stringlist::size_type index) const;

    stringlist::size_type occurrenceCountAll() const;
    stringlist::size_type occurrenceCount(const std::string &filename) const;


protected:
    const std::string searchInclude(std::istream * const iodevice) const;

    void searchText(const std::string &text,
                    const std::string &searchedText,
                    const std::string &currentFileName,
                    const int currentLineNumber);

private:
    /* list of the filename + all included files */
    stringlist m_files;

    /* list of error messages */
    stringlist m_errors;

    /* map containing the occurences for each file */
    ResultMap m_results;

    void appendFileName(const std::string &filenameToBeInserted,
                        const std::string &currentFileName,
                        const int currentLineNumber);

};

#endif // ENGINE_H
