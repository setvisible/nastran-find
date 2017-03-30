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

#include <QtTest/QtTest>
#include <QtCore/QDebug>
#include <QtCore/QString>

#include <StringHelper>

using namespace std;

class tst_StringHelper : public QObject
{
    Q_OBJECT

private slots:
    void test_findNextInsensitive();
    void test_findNextInsensitive_data();

    void test_countInsensitive();
    void test_countInsensitive_data();

};

/******************************************************************************
 ******************************************************************************/
void tst_StringHelper::test_findNextInsensitive_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("search");
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("expected");

    QTest::newRow("empty") << "" << "" << 0 << -1;
    QTest::newRow("empty") << "" << "yo" << 0 << -1;
    QTest::newRow("empty search") << "sometext" << "" << 0 << -1;

    QTest::newRow("simple") << "ab"  << "Ab" << 0 << 0;
    QTest::newRow("simple") << "aab" << "aab" << 0 << 0;
    QTest::newRow("simple") << "aAaab" << "aab" << 0 << 2;

    QTest::newRow("case insensitive") << "Aab" << "Ab" << 0 << 1;
    QTest::newRow("case insensitive") << ".IN..inc" << ".INc" << 0 << 4;

    QTest::newRow("test 1") << "aaaAaaaA" << "A" << -99 << -1;
    QTest::newRow("test 1") << "aaaAaaaA" << "A" << -1 << -1;
    QTest::newRow("test 1") << "aaaAaaaA" << "A" << 0 << 0;
    QTest::newRow("test 1") << "aaaAaaaA" << "A" << 5 << 5;
    QTest::newRow("test 1") << "aaaAaaaA" << "A" << 7 << 7;
    QTest::newRow("test 1") << "aaaAaaaA" << "A" << 8 << -1;
    QTest::newRow("test 1") << "aaaAaaaA" << "A" << 99 << -1;

    QTest::newRow("test 2") << "aaaAaaaA" << "Aaa" << -1 << -1;
    QTest::newRow("test 2") << "aaaAaaaA" << "Aaa" << 0 << 0;
    QTest::newRow("test 2") << "aaaAaaaA" << "Aaa" << 5 << 5;
    QTest::newRow("test 2") << "aaaAaaaA" << "Aaa" << 6 << -1;

    QTest::newRow("quotes") << "  \"  \"   \"" << "\"" << 3 << 5;
    QTest::newRow("tabulation") << "\t\t\ta" << "a" << 0 << 3;

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
void tst_StringHelper::test_countInsensitive_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("search");
    QTest::addColumn<int>("expected");

    QTest::newRow("empty") << "" << "" << 0 ;
    QTest::newRow("empty") << "" << "yo" << 0 ;
    QTest::newRow("empty search") << "sometext" << "" << 0 ;

    QTest::newRow("spaces") << "" << " " << 0 ;
    QTest::newRow("spaces") << " " << " " << 1 ;
    QTest::newRow("spaces") << "  " << " " << 2 ;

    QTest::newRow("no overlapping") << "aaa"  << "aa" << 1;
    QTest::newRow("no overlapping") << "aaaa"  << "aa" << 2;
    QTest::newRow("no overlapping") << "aaaa"  << "aaa" << 1;

    QTest::newRow("case insensitive") << "aAa"  << "A" << 3;

    QTest::newRow("find quotes") << "  \"  \"   \"" << "\"" << 3;
    QTest::newRow("tabulation") << "\t\t\ta" << "a" << 1;

}

void tst_StringHelper::test_countInsensitive()
{
    QFETCH(QString, text);
    QFETCH(QString, search);
    QFETCH(int, expected);
    string _text   = text.toStdString();
    string _search = search.toStdString();

    int actual = StringHelper::countInsensitive(_text, _search);

    QCOMPARE( actual, expected );
}


QTEST_APPLESS_MAIN(tst_StringHelper)

#include "tst_stringhelper.moc"

