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
#include <QtTest/QtTest>
#include <QtCore/QDebug>

#include <FileInfo>


class tst_FileInfo : public QObject
{
    Q_OBJECT

private slots:
    void test_empty();
    void test_correct_path();
    void test_wrong_path();
    void test_separator();
    void test_resolve_path();
    void test_symlink();

};

/******************************************************************************
 ******************************************************************************/
void tst_FileInfo::test_empty()
{
    // Given, When
    std::string expected = "";
    std::string actual = FileInfo::canonicalFilePath("");

    // Then
    QCOMPARE( actual, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_FileInfo::test_correct_path()
{
    // Given
    std::string expected_0 = "/usr/project/ussue/1sd2";
    std::string expected_1 = "C:/usr/project/ussue/1sd2";

    // When
    std::string actual_0 = FileInfo::canonicalFilePath("/usr/project/ussue/1sd2/filename.dat");
    std::string actual_1 = FileInfo::canonicalFilePath("c:/usr/project/ussue/1sd2/filename.dat");

    // Then
    QCOMPARE( actual_0, expected_0 );
    QCOMPARE( actual_1, expected_1 );

}

/******************************************************************************
 ******************************************************************************/
void tst_FileInfo::test_wrong_path()
{
    // Given
    std::string expected = "";

    // When
    std::string actual_0 = FileInfo::canonicalFilePath("");
    std::string actual_1 = FileInfo::canonicalFilePath("c:");
    std::string actual_2 = FileInfo::canonicalFilePath("c:\\");       /* directories are not files */
    std::string actual_3 = FileInfo::canonicalFilePath("/usr");
    std::string actual_4 = FileInfo::canonicalFilePath("/usr/");
    std::string actual_4b = FileInfo::canonicalFilePath("/usr/temp/lib/");
    std::string actual_5 = FileInfo::canonicalFilePath("readme.txt");       /* filename alone */
    std::string actual_6 = FileInfo::canonicalFilePath("./readme.txt");

    // Then
    QCOMPARE( actual_0, expected );
    QCOMPARE( actual_1, expected );
    QCOMPARE( actual_2, expected );
    QCOMPARE( actual_3, expected );
    QCOMPARE( actual_4, expected );
    QCOMPARE( actual_4b, expected );
    QCOMPARE( actual_5, expected );
    QCOMPARE( actual_6, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_FileInfo::test_separator()
{
    // Given
    std::string expected = "C:/temp";

    // When
    std::string actual1 = FileInfo::canonicalFilePath("c:/temp/foo");    /* Unix separator */
    std::string actual2 = FileInfo::canonicalFilePath("c:\\temp\\foo");  /* Windows separator */
    std::string actual3 = FileInfo::canonicalFilePath("c:\\temp/foo");   /* Separator mix */

    // Then
    QCOMPARE( actual1, expected );
    QCOMPARE( actual2, expected );
    QCOMPARE( actual3, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_FileInfo::test_resolve_path()
{
    // Given
    std::string expected = "C:/temp";

    // When
    std::string actual1 = FileInfo::canonicalFilePath("c:/temp/../temp/../temp/foo");
    std::string actual2 = FileInfo::canonicalFilePath("c:\\temp\\..\\temp\\..\\temp\\foo.dat");
    std::string actual3 = FileInfo::canonicalFilePath("c:\\temp/..\\temp/..\\temp\\foo");

    // Then
    QCOMPARE( actual1, expected );
    QCOMPARE( actual2, expected );
    QCOMPARE( actual3, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_FileInfo::test_symlink()
{
    /// XXX symbolic links UNIX
}

/******************************************************************************
 ******************************************************************************/

QTEST_APPLESS_MAIN(tst_FileInfo)

#include "tst_fileinfo.moc"

