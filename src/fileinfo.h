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
#ifndef FILEINFO_H
#define FILEINFO_H

#include <string>

class FileInfo
{
public:
    static std::string fileName(const std::string &fullFileName);
    static std::string canonicalFilePath(const std::string &fullFileName);

    static bool isRelativePath(const std::string &path);
    static inline bool isAbsolutePath(const std::string &path) { return !isRelativePath(path); }

    static std::string concat(const std::string &var1, const std::string &var2);

private:
    static inline std::string toNativeSeparators(const std::string &pathName);
    static inline std::string fromNativeSeparators(const std::string &pathName);
    static inline char separator();
};

#endif // FILEINFO_H
