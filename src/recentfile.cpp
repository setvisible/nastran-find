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
#endif

using namespace std;

/*! \class RecentFile
 *  \brief The class RecentFile manages the recent files of the application.
 *
 * It stores data in a plain old text file '.nastranfind'.
 *
 * Each platform has its own API for finding the user's home folder,
 * or documents folder, or preferences folder. Permissions to access settings
 * belonging to the current user
 *
 * \li On Windows, it's usually:
 *     C:\Documents and Settings\{User Name}\Application Data\NastranFind.ini
 * or  C:\Users\{User Name}\AppData\Local\NastranFind.ini
 * or  C:\Users\{User Name}\AppData\LocalLow\NastranFind.ini
 * or  C:\Users\{User Name}\AppData\Roaming\NastranFind.ini
 *
 *    (%APPDATA%\NastranFind.ini)
 *
 *     Note: %APPDATA% is returned by SHGetKnownFolderPath().
 *
 * \li On Mac OS X and iOS:
 *    ~/Library/Preferences/com.NastranFind.plist
 *
 *    ("$HOME/Library/Preferences/com.NastranFind.plist")
 *
 * \li On Unix:
 *    ~/.config/NastranFind.ini
 *
 *    ("$HOME/.config/NastranFind.ini")
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
        PathAppendA(szPath, "NastranFind.ini");
        std::string path(szPath);
        m_configFullFilename = path;

        /// \todo /* UNICODE */
        /// \todo PathAppendW(szPath, "NastranFind.ini");
        /// \todo std::wstring path(szPath);
        /// \todo m_configFullFilename = path;

    } else {
        std::cout << "SHGetFolderPath() failed for standard location '%APPDATA%'." << std::endl;
    }

#elif defined(Q_OS_MAC)

    /// \todo  "$HOME/Library/Preferences/com.NastranFind.plist"

#elif defined(Q_OS_UNIX)

    passwd* pw = getpwuid(getuid());
    std::string homepath(pw->pw_dir);
    m_configFullFilename = homepath + "/.config/NastranFind.ini";

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
        std::cout << "Config file '" << m_configFullFilename << "' not found." << std::endl;
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


