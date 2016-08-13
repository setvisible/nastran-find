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
#include "fileinfo.h"

#include "systemdetection.h"

#include <string>
#include <vector>

using namespace std;

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the file name of the given \a fullFileName.
 *
 * If the fullFileName ends with a slash, returns an empty string.
 *
 * Example:
 * \code
 *   std::string name = FileInfo::fileName("/tmp/archive.tar.gz"); 
 *   // name == "archive.tar.gz"
 * \endcode
 */
std::string FileInfo::fileName(const std::string &fullFileName)
{
    string fn = FileInfo::fromNativeSeparators(fullFileName);
    std::string ret;
    char ch;
    for (int i = 0; i < (int)fn.size(); ++i) {

        ch = fn[i];
        switch (ch) {

        case '/':
        case '\\':
            ret.clear();

            break;
        default:
            ret += ch;
        }
    }
    return ret;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the canonical path excluding the file name and the last separator,
 *         i.e. an absolute path without symbolic links or redundant "." or ".." elements.
 *
 * The \a fullFileName can be absolute, canonical or relative.
 *
 * If the file does not exist, returns an empty string.
 *
 * Example:
 * \code
 *   std::string filename = "c:/usr/project/ussue/1sd2/file.dat";
 *   std::string path = FileInfo::canonicalFilePath(filename);
 *   cout << path;      // "C:/usr/project/ussue/1sd2"
 * \endcode
 *
 */
std::string FileInfo::canonicalFilePath(const std::string &fullFileName)
{
    if (fullFileName.empty()) {
        return std::string();
    }

    if( isRelativePath(fullFileName) ){
        return std::string();
    }

    if( fullFileName[fullFileName.size()-1] == '/' ||
            fullFileName[fullFileName.size()-1] == '\\' ){
        return std::string();
    }

    std::string ret;
    vector<string> tokens;

    char ch = 0;
    string buffer;
    for (int i = 0; i < (int)fullFileName.size(); ++i) {

        ch = fullFileName[i];
        switch (ch) {

        case '/':
        case '\\':
            buffer += '/';
            tokens.push_back( buffer );
            buffer.clear();

            break;
        default:
            buffer += ch;
        }
    }

    /* Checking for ".." or "." starts to relative paths */
    for (int i = 0; i < (int)tokens.size(); ++i) {

        if (tokens[i] == "./") {
            tokens[i] = "";
            continue;
        }

        if (i>0 && tokens[i] == "../") {
            tokens[i-1] = "";
            tokens[i] = "";
            continue;
        }
    }

    for (int i = 0; i < (int)tokens.size(); ++i) {
        if (tokens[i] != "") {
            ret += tokens[i];
            //ret += '/';
        }
    }

    /* clean the last '/' */
    if (ret.size() > 0) {
        if (ret[ret.size()-1] == '/') {
            ret.resize(ret.size()-1);
        }
    }

    /* Windows: Upperize the case of the drive letter */
    if (ret.size() > 1) {
        if (ret[1] == ':') {  // C: D: etc
            ret[0] = toupper(ret[0]);
        }
    }

    return ret;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns \a true if the file path name is relative, otherwise returns
 *  \a false if the path is absolute (e.g. under Unix a path is absolute
 *  if it begins with a "/").
 *
 *  \sa isAbsolute()
 */
bool FileInfo::isRelativePath(const std::string &path)
{
    return ( path.length() == 0
             || (path.at(0) != '/'
            && !(path.length() >= 2 && path.at(1) == ':')));
}


/******************************************************************************
 ******************************************************************************/
std::string FileInfo::concat(const std::string &var1, const std::string &var2)
{
    std::string ret = var1 + separator() + var2;
    return ret;
}


/******************************************************************************
 ******************************************************************************/
/*! \brief Returns the native directory separator,
 *         "/" under Unix and "\\" under Windows.
 */
inline char FileInfo::separator()
{
#if defined(Q_OS_WIN)
    return '\\';
#elif defined(Q_OS_UNIX)
    return '/';
#endif
}


/******************************************************************************
 ******************************************************************************/
/*! \brief Returns \a pathName with the '/' separators converted to
 *  separators that are appropriate for the underlying operating
 *  system.
 *
 *  On Windows, toNativeSeparators("c:/winnt/system32") returns
 *  "c:\\winnt\\system32".
 *
 *  The returned string may be the same as the argument on some
 *  operating systems, for example on Unix.
 *
 *  \sa fromNativeSeparators(), separator()
 */
inline std::string FileInfo::toNativeSeparators(const std::string &pathName)
{
#if defined(Q_OS_WIN)
    string n(pathName);
    char * const data = const_cast<char *>(n.data());
    for (string::size_type i = 0; i < n.length(); ++i) {
        if (data[i] == '/') {
            data[i] = '\\';
        }
    }
    return n;
#endif
    return pathName;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Returns \a pathName using '/' as file separator. On Windows,
 *  for instance, fromNativeSeparators("\c{c:\\winnt\\system32}") returns
 *  "c:/winnt/system32".
 *
 *  The returned string may be the same as the argument on some
 *  operating systems, for example on Unix.
 *
 *  \sa toNativeSeparators(), separator()
 */
inline std::string FileInfo::fromNativeSeparators(const std::string &pathName)
{
#if defined(Q_OS_WIN)
    string n(pathName);
    char * const data = const_cast<char *>(n.data());
    for (string::size_type i = 0; i < n.length(); ++i) {
        if (data[i] == '\\') {
            data[i] = '/';
        }
    }
    return n;
#endif
    return pathName;
}

