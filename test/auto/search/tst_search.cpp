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

#include "../../../src/engine.h"

#include <sstream> // for std::istringstream

class tst_Search : public QObject
{
    Q_OBJECT

private slots:
    void test_find();
    void test_find_empty();
    void test_find_injection_regexp();
    void test_find_whitespace();
    void test_find_multiline_deck();
    void test_find_two_occurences_on_same_line();

};

// class EngineFriend : Engine { friend class tst_Search; };


/******************************************************************************
 ******************************************************************************/
void tst_Search::test_find()
{
    // Given
    Engine engine;
    std::string filename("dummy.dat");
    std::istringstream buffer(
                "SOL 101\n"
                "CEND\n"
                "ECHO = NONE\n"
                "SUBCASE 300\n" );

    // When
    engine.find( &buffer, "END", filename);

    // Then
    QCOMPARE( engine.resultCount(filename), 1);
    QCOMPARE( engine.resultAt(filename, 0), std::string("line       2: CEND"));
}

/******************************************************************************
 ******************************************************************************/
void tst_Search::test_find_empty()
{
    /* ***************************************************************** */
    /* Show the filenames only, when "" is searched                      */
    /* ***************************************************************** */
    // Given
    Engine engine;
    std::string filename("dummy.dat");
    std::istringstream buffer( "CBAR         123     456 123.456   \n" );

    // When
    engine.find( &buffer, "", filename);

    // Then
    QCOMPARE( engine.resultCount(filename), 0);
}

/******************************************************************************
 ******************************************************************************/
void tst_Search::test_find_whitespace()
{
    /* ***************************************************************** */
    /* Show all the content, when a() white space(s) is(are) searched    */
    /* ***************************************************************** */
    // Given
    Engine engine;
    std::string filename("dummy.dat");
    std::stringstream buffer(
                /* 1*/ "SOL 101\n"
                /* 2*/ "CEND\n"
                /* 3*/ "SUBCASE 1\n"
                /* 4*/ "  LOAD = 1\n"
                /* 5*/ "  $SPC = 1\n"
                /* 6*/ "  DISP = ALL\n"
                /* 7*/ "BEGIN BULK\n"
                /* 8*/ "\n"
                /* 9*/ "LOAD, 2, 1., 1., 1001, 1., 1002\n"
                /*10*/ "PLOAD4, 1001, 500, 3.\n"
                /*11*/ "FORCE,  1002, 15, 0, 1., 0., 0., 1.\n"
                /*12*/ "PLOAD,  1003, 9., 1, 2, 7, 6\n"
                /*13*/ "GRID, 1,  0, 0., 0., 0.,,123456\n"
                /*14*/ "GRID, 2,  0, 1., 0., 0.\n"
                /*15*/ "GRID, 3,  0, 2., 0., 0.\n"
                /*16*/ "GRID, 4,  0, 3., 0., 0.\n"
                /*17*/ "GRID, 5,  0, 4., 0., 0.\n"
                /*18*/ "\n"
                /*19*/ "CQUAD4, 100, 1000, 1, 2, 7, 6\n"
                /*20*/ "\n"
                /*21*/ "PSHELL, 1000, 10000, 0.1, 10000\n"
                /*22*/ "MAT1,   10000, 1e7, , 0.3\n"
                /*23*/ "ENDDATA\n"
                /*24*/ "\n" );

    // When
    engine.find( &buffer, "     ", filename);

    // Then
    QCOMPARE( engine.resultCount(filename), 24);
    QCOMPARE( engine.resultAt(filename,  0), std::string( "line       1: SOL 101" ));
    QCOMPARE( engine.resultAt(filename,  1), std::string( "line       2: CEND" ));
    QCOMPARE( engine.resultAt(filename,  2), std::string( "line       3: SUBCASE 1" ));
    QCOMPARE( engine.resultAt(filename,  3), std::string( "line       4:   LOAD = 1" ));
    QCOMPARE( engine.resultAt(filename,  4), std::string( "line       5:   $SPC = 1" ));
    QCOMPARE( engine.resultAt(filename,  5), std::string( "line       6:   DISP = ALL" ));
    QCOMPARE( engine.resultAt(filename,  6), std::string( "line       7: BEGIN BULK" ));
    QCOMPARE( engine.resultAt(filename,  7), std::string( "line       8: " ));
    QCOMPARE( engine.resultAt(filename,  8), std::string( "line       9: LOAD, 2, 1., 1., 1001, 1., 1002" ));
    QCOMPARE( engine.resultAt(filename,  9), std::string( "line      10: PLOAD4, 1001, 500, 3." ));
    QCOMPARE( engine.resultAt(filename, 10), std::string( "line      11: FORCE,  1002, 15, 0, 1., 0., 0., 1." ));
    QCOMPARE( engine.resultAt(filename, 11), std::string( "line      12: PLOAD,  1003, 9., 1, 2, 7, 6" ));
    QCOMPARE( engine.resultAt(filename, 12), std::string( "line      13: GRID, 1,  0, 0., 0., 0.,,123456" ));
    QCOMPARE( engine.resultAt(filename, 13), std::string( "line      14: GRID, 2,  0, 1., 0., 0." ));
    QCOMPARE( engine.resultAt(filename, 14), std::string( "line      15: GRID, 3,  0, 2., 0., 0." ));
    QCOMPARE( engine.resultAt(filename, 15), std::string( "line      16: GRID, 4,  0, 3., 0., 0." ));
    QCOMPARE( engine.resultAt(filename, 16), std::string( "line      17: GRID, 5,  0, 4., 0., 0." ));
    QCOMPARE( engine.resultAt(filename, 17), std::string( "line      18: " ));
    QCOMPARE( engine.resultAt(filename, 18), std::string( "line      19: CQUAD4, 100, 1000, 1, 2, 7, 6" ));
    QCOMPARE( engine.resultAt(filename, 19), std::string( "line      20: " ));
    QCOMPARE( engine.resultAt(filename, 20), std::string( "line      21: PSHELL, 1000, 10000, 0.1, 10000" ));
    QCOMPARE( engine.resultAt(filename, 21), std::string( "line      22: MAT1,   10000, 1e7, , 0.3" ));
    QCOMPARE( engine.resultAt(filename, 22), std::string( "line      23: ENDDATA"  ));
    QCOMPARE( engine.resultAt(filename, 23), std::string( "line      24: " ));

}

/* *****************************************************************************
 ***************************************************************************** */
void tst_Search::test_find_injection_regexp()
{
    // Given
    Engine engine;
    std::string filename("dummy.dat");
    std::istringstream buffer( "CARD  1.23  .23  0.23  1.  1.0  1.2e10  1.23e−15 \n" );

    // When
    engine.find( &buffer, ".*", filename); /* RegExp not allowed */

    // Then
    QCOMPARE( engine.resultCount(filename), 0);
}


/* *****************************************************************************
 ***************************************************************************** */
void tst_Search::test_find_multiline_deck()
{
    /* ***************************************************************** */
    /* Treat a Bulk Deck Entry as a single line.                         */
    /* --> Show the entire Bulk Deck Entry when an occurence is found    */
    /* ***************************************************************** */
    // Given
    Engine engine;
    std::string filename("dummy.dat");
    std::istringstream buffer(
                /* 1*/  "ASSIGN OUTPUT2='myMatrix.op2',\n"
                /* 2*/  "       UNIT=15\n"
                /* 3*/  "$\n"
                /* 4*/  "SOL 100\n"
                /* 5*/  "DIAG 8,44\n"
                /* 6*/  "COMPILE USERDMAP\n"
                /* 7*/  "ALTER 2\n"
                /* 8*/  "DMIIN DMI,DMINDX/A,B,MYDOF,,,,,,,/ $\n"
                /* 9*/  "MPYAD A,B,/ATB/1///$\n"
                /*10*/  "MPYAD B,A,/BTA/1///$\n"
                /*11*/  "OUTPUT2 A,B,ATB,BTA,MYDOF//0/15$\n"
                /*12*/  "MATPRN A,B,ATB,BTA,MYDOF/$\n"
                /*13*/  "CEND\n"
                /*14*/  "$\n"
                /*15*/  "BEGIN BULK\n"
                /*16*/  "$......1$......2$......3$......4$......5$......6$......7$......8$......9$.....10\n"
                /*17*/  "$DMI 	NAME	0	 FORM 	TIN 	TOUT 	M 	N\n"
                /*18*/  "$\n"
                /*19*/  "DMI	A	0	2	1	1		4	2\n"
                /*20*/  "DMI	A	1 	1 	1. 	3. 	5.\n"
                /*21*/  "DMI	A	2 	2 	6. 	4 	8.\n"
                /*22*/  "$\n"
                /*23*/  "DMI	B	0	2	1	1		4	2\n"
                /*24*/  "DMI	B	1 	1 	1. 	3. 	5.\n"
                /*25*/  "DMI	B	2 	2 	6. 	4 	8.\n"
                /*26*/  "$\n"
                /*27*/  "DMI     MYDOF   0       2       1       1               36      1\n"
                /*28*/  "DMI     MYDOF   1       1       -1.0    1.0     1.0     -1.0    1.0     +\n"
                /*29*/  "+       2.0     -1.0    1.0     3.0     -1.0    1.0     4.0     -1.0    +\n"
                /*30*/  "+       1.0     5.0     -1.0    1.0     6.0     -1.0    2.0     1.0     +\n"
                /*31*/  "+       -1.0    2.0     2.0     -1.0    2.0     3.0     -1.0    2.0     +\n"
                /*32*/  "+       4.0     -1.0    2.0     5.0     -1.0    2.0     6.0\n"
                /*33*/  "ENDDATA\n" );

    // When
    engine.find( &buffer, "+       1.0", filename); // injection REGEXP !!!

    // Then
    QCOMPARE( engine.resultCount(filename), 1);
    /// \todo   QCOMPARE( engine.resultAt(filename, 0), std::string(
    /// \todo               "line      28: DMI     MYDOF   1       1       -1.0    1.0     1.0     -1.0    1.0     +\n"
    /// \todo               "            : +       2.0     -1.0    1.0     3.0     -1.0    1.0     4.0     -1.0    +\n"
    /// \todo               "            : +       1.0     5.0     -1.0    1.0     6.0     -1.0    2.0     1.0     +\n"
    /// \todo               "            : +       -1.0    2.0     2.0     -1.0    2.0     3.0     -1.0    2.0     +\n"
    /// \todo               "            : +       4.0     -1.0    2.0     5.0     -1.0    2.0     6.0" ));

}

/******************************************************************************
 ******************************************************************************/
void tst_Search::test_find_two_occurences_on_same_line()
{
    /* ************************************************************* */
    /* Don't repeat the lines where several occurences are found.    */
    /* ************************************************************* */
    // Given
    Engine engine;
    std::string filename("dummy.dat");
    std::istringstream buffer( "CBAR         123     456 123.456   \n" );

    // When
    engine.find( &buffer, "123", filename);

    // Then
    QCOMPARE( engine.resultCount(filename), 1);
    QCOMPARE( engine.resultAt(filename, 0), std::string("line       1: CBAR         123     456 123.456   "));
}

/* *****************************************************************************
 ***************************************************************************** */

QTEST_APPLESS_MAIN(tst_Search)

#include "tst_search.moc"

