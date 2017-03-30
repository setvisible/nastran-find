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
#ifndef APPLICATION_H
#define APPLICATION_H

#include "recentfile.h"
#include "engine.h"

#include <string>
#include <list>
#include <map>

class Application
{
    enum class Color {
        NORMAL           = 1,
        TITLE               ,
        SEARCH_BOX          ,
        FILE_NAME           ,
        LINE_NUMBER         ,
        OCCURRENCE          ,
        ERROR_MESSAGE       ,
        NASTRAN_CARD        ,
        NASTRAN_COMMENT     ,
        NASTRAN_DIGIT       ,
        NASTRAN_QUOTE       ,
        NASTRAN_SYMBOL
    };

    enum class Symbol {
        UNKNOWN  ,
        ALPHA    ,
        DIGIT    ,
        QUOTE
    };
    enum class Mode {
        BROWSE,     ///< Mode when using the keys to scroll the results
        SEARCH      ///< Mode when using the keys to write a new search string
    };

public:
    explicit Application( int argc, char *argv[] );

    int exec();

    void resetConfig();
    void setFilename(const std::string &filename);

private:
    Mode m_mode;
    std::string m_fullFileName;
    std::string m_searchedText;
    int m_currentScroll;
    int m_maximumScroll;
    int m_rowTitleBox;
    int m_rowResultBox;
    int m_rowErrorBox;
    int m_rowInfoBox;

    RecentFile m_recentFile;
    Engine m_engine;

    void initialize();
    void onKeyPressed(const int key);

    void showTitle();
    void showResults();
    void showErrors();
    void showInfo();

    void printwSyntaxColoration(const std::string &text, const int row);

    inline std::string horizontalSeparator(const char c = '=') const;
    inline stringlist::size_type getMaximumScroll() const;
    inline void hideCursor();


    Application::Color m_currentColor;
    inline void colorize(Application::Color color);
    inline void uncolorize();

    void text_in_a_box( const char *istr, const int len);

};

#endif  // APPLICATION_H
