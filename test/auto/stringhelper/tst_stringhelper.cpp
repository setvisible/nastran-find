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
#include <QtCore/QString>

#include "../../../src/stringhelper.h"

using namespace std;

class tst_StringHelper : public QObject
{
    Q_OBJECT

private slots:
    void test_findNextInsensitive();
    void test_findNextInsensitive_data();

};

/******************************************************************************
 ******************************************************************************/
void tst_StringHelper::test_findNextInsensitive_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("search");
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("expected");

    QTest::newRow("test__1") << "sometext" << "" << 0 << -1;
    QTest::newRow("test__2") << "ab"  << "Ab" << 0 << 0;
    QTest::newRow("test__3") << "Aab" << "Ab" << 0 << 1;
    QTest::newRow("test__4") << "aab" << "aab" << 0 << 0;
    QTest::newRow("test__5") << "aAaab" << "aab" << 0 << 2;
    QTest::newRow("test__6") << ".IN..inc" << ".INc" << 0 << 4;
    QTest::newRow("test__7") << "aaaAaaaA" << "A" << -99 << -1;
    QTest::newRow("test__8") << "aaaAaaaA" << "A" << -1 << -1;
    QTest::newRow("test__9") << "aaaAaaaA" << "A" << 0 << 0;
    QTest::newRow("test_10") << "aaaAaaaA" << "A" << 5 << 5;
    QTest::newRow("test_11") << "aaaAaaaA" << "A" << 7 << 7;
    QTest::newRow("test_12") << "aaaAaaaA" << "A" << 8 << -1;
    QTest::newRow("test_13") << "aaaAaaaA" << "A" << 99 << -1;
    QTest::newRow("test_14") << "aaaAaaaA" << "Aaa" << -1 << -1;
    QTest::newRow("test_15") << "aaaAaaaA" << "Aaa" << 0 << 0;
    QTest::newRow("test_16") << "aaaAaaaA" << "Aaa" << 5 << 5;
    QTest::newRow("test_17") << "aaaAaaaA" << "Aaa" << 6 << -1;
    QTest::newRow("test_18") << "  \"  \"   \"" << "\"" << 3 << 5;
    QTest::newRow("test_19") << "\t\t\ta" << "a" << 0 << 3;

}

void tst_StringHelper::test_findNextInsensitive()
{
    QFETCH(QString, text);
    QFETCH(QString, search);
    QFETCH(int, from);
    QFETCH(int, expected);
    string _text   = text.toStdString();
    string _search = search.toStdString();

    int actual = StringHelper::findNextInsensitive(_text, _search, from);

    QCOMPARE( actual, expected );
}


/******************************************************************************
 ******************************************************************************/

QTEST_APPLESS_MAIN(tst_StringHelper)

#include "tst_stringhelper.moc"

