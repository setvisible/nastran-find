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

using namespace std;

namespace {
static const char str_config_filename[] = ".nastranfind";
}

RecentFile::RecentFile()
{
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
    string line;
    ifstream infile( str_config_filename );
    if( infile.is_open() ){
        while( getline( infile, line) ){
            append(line);
        }
        infile.close();
    } else {
        cout << "Unable to open file: " << str_config_filename << endl;
        return false;
    }
    return true;
}

bool RecentFile::write()
{
    ofstream outfile( str_config_filename );
    if( outfile.is_open() ){
        for (std::list<string>::const_iterator it = m_filenames.begin(); it != m_filenames.end(); it++) {
            outfile << *it << endl;
        }
        outfile.close();
    } else {
        cout << "Unable to open file: " << str_config_filename << endl;
        return false;
    }
    return true;
}


