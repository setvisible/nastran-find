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
#ifndef RECENT_FILE_H
#define RECENT_FILE_H

#include <string>
#include <list>

class RecentFile
{
public:
    explicit RecentFile();
    ~RecentFile();

    void clear();
    void append(const std::string &filename);
    void prepend(const std::string &filename);

    int count() const;
    std::string at(const int index) const;

protected:
    bool read();
    bool write();

private:
    std::string m_configFullFilename;
    std::list<std::string> m_filenames;
};

#endif // RECENT_FILE_H
