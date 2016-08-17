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
#include "recentfile.h"

#include <iostream>
#include <fstream>
#include "systemdetection.h"

#if defined(Q_OS_WIN)
#  include <shlobj.h>     // SHGetKnownFolderPath()
#  include <shlwapi.h>    // PathAppend()
#elif defined(Q_OS_UNIX)
#  include <unistd.h>
#  include <sys/types.h>
#  include <pwd.h>
#  include <sys/stat.h>   // stat(), mkdir()
#endif

using namespace std;

/*! \class RecentFile
 *  \brief The class RecentFile manages the recent files of the application.
 *
 * It stores data in a plain old text file, in the user's preferences directory.
 *
 * Each platform has its own API for finding the user's preferences folder,
 * and permissions to access it.
 *
 * The preferences folder is platform dependant:
 *
 * \li On Windows, it's usually:
 *     C:\Documents and Settings\{User Name}\Application Data\NastranFind\Recent.ini
 * or  C:\Users\{User Name}\AppData\Local\NastranFind\Recent.ini
 * or  C:\Users\{User Name}\AppData\LocalLow\NastranFind\Recent.ini
 * or  C:\Users\{User Name}\AppData\Roaming\NastranFind\Recent.ini
 *
 *    (%APPDATA%\NastranFind\Recent.ini)
 *
 * \li On Mac OS X and iOS:
 *    ~/Library/Preferences/com.NastranFind.plist
 *
 *    ("$HOME/Library/Preferences/com.NastranFind.plist")
 *
 * \li On Unix:
 *    ~/.config/nastranfind/recent.ini
 *
 *    ("$HOME/.config/nastranfind/recent.ini")
 *
 */

/*! \brief Constructor.
 */
RecentFile::RecentFile()
    : m_configFullFilename(std::string())
{
#if defined(Q_OS_WIN)

    /* ANSI */
    char szPath[MAX_PATH];
    HRESULT hr = SHGetFolderPathA(NULL, CSIDL_APPDATA /*|CSIDL_FLAG_CREATE*/,  NULL, 0, szPath);

    /// \todo /* UNICODE */
    /// \todo TCHAR szPath[MAX_PATH];
    /// \todo HRESULT hr = SHGetFolderPathW(NULL, CSIDL_APPDATA,  NULL, 0, szPath);

    if( SUCCEEDED(hr) ){

        /* ANSI */
        PathAppendA(szPath, "NastranFind");
        std::string path(szPath);
        m_configFullFilename = path + std::string("\\Recent.ini");

        if ( !(CreateDirectoryA(path.c_str(), NULL)
               || GetLastError() == ERROR_ALREADY_EXISTS) ) {
            std::cout << "Failed to create directory '" << path << "'." << std::endl;
        }

        /// \todo /* UNICODE */
        /// \todo PathAppendW(szPath, "NastranFind");
        /// \todo std::wstring path(szPath);
        /// \todo m_configFullFilename = path + std::string("\\Recent.ini");
        /// \todo if ( !(CreateDirectoryW(path.c_str(), NULL)
        /// \todo        || GetLastError() == ERROR_ALREADY_EXISTS) ) {
        /// \todo     std::cout << "Failed to create directory '" << path << "'." << std::endl;
        /// \todo }

    } else {
        std::cout << "SHGetFolderPath() failed for standard location '%APPDATA%'." << std::endl;
    }

#elif defined(Q_OS_MAC)

    /// \todo  "$HOME/Library/Preferences/com.NastranFind.plist"

#elif defined(Q_OS_UNIX)

    passwd* pw = getpwuid(getuid());
    std::string homepath(pw->pw_dir);
    std::string configpath = homepath + std::string("/.config/nastranfind");

    /* Create a directory with read/write/search permissions */
    /* for owner and group, and read/search permissions for others. */
    struct stat st = {0};
    if( stat(configpath.c_str(), &st) == -1 ){
        int status = mkdir(configpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if ( status == -1 ) {
            std::cout << "Failed to create directory '" << configpath << "'." << std::endl;
        }
    }
    m_configFullFilename = configpath + std::string("/recent.ini");

#endif

    read();
}

RecentFile::~RecentFile()
{
    write();
}

void RecentFile::clear()
{
    m_filenames.clear();
    write();
}


void RecentFile::append(const string &filename)
{
    m_filenames.remove(filename);
    m_filenames.push_back(filename);
    write();
}

void RecentFile::prepend(const string &filename)
{
    m_filenames.remove(filename);
    m_filenames.push_front(filename);
    write();
}

int RecentFile::count() const
{
    return (int)m_filenames.size();
}

std::string RecentFile::at(const int index) const
{
    if (index >= 0 && index < count()) {
        std::list<string>::const_iterator it = m_filenames.begin();
        std::advance(it, index);
        // now 'it' points to the element at 'index'
        return *it;
    }
    return string();
}

bool RecentFile::read()
{
    ifstream infile( m_configFullFilename );
    if( infile.is_open() ){
        string line;
        while( getline( infile, line) ){
            append(line);
        }
        infile.close();
    } else {
        std::cout << "Creating Config file '" << m_configFullFilename << "'." << std::endl;
                return false;
    }
    return true;
}

bool RecentFile::write()
{
    ofstream outfile( m_configFullFilename );
    if( outfile.is_open() ){
        for (std::list<string>::const_iterator it = m_filenames.begin(); it != m_filenames.end(); it++) {
            outfile << *it << endl;
        }
        outfile.close();
    } else {
        std::cout << "Warning: Cannot write in config file '" << m_configFullFilename << "'." << std::endl;
        return false;
    }
    return true;
}


