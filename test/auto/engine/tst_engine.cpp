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

class tst_Engine : public QObject
{
    Q_OBJECT

private slots:
    void test_empty_filename();
    void test_invalid_filename();
    void test_broken();
    void test_comment();
    void test_cyclic_self_inclusion();
    void test_cyclic_simple();
    void test_cyclic_complex();
    void test_duplicate_1();
    void test_duplicate_2();
    void test_first_last_line();
    void test_multiline();
    void test_quotes();
    void test_single_line();
    void test_subdirectories();
    void test_symbolic_links();
    void test_white_spaces();

};

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_empty_filename()
{
    // Given, When
    Engine engine;
    engine.find("", "");

    // Then
    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0).c_str(), STR_ERR_EMPTY_FILENAME);
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_invalid_filename()
{
    // Given, When
    const char invalid[] = "_?*[]?*![}##*!"; // invalid filename
    Engine engine;
    engine.find(invalid, "");

    // Then
    std::string error_msg
            = STR_ERR_CANNOT_OPEN
            + std::string(invalid)
            + STR_ERR_QUOTE_END;

    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0), error_msg );
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_broken()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/broken/test.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    std::string error_msg_1
            = STR_ERR_CANNOT_OPEN
            + std::string("not_existing_file.dat")
            + STR_ERR_QUOTE_END;

    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0), error_msg_1);

    std::string link0( "test.dat" );
    std::string link1( "not_existing_file.dat" );
    std::string link2( ".\\broken.dat" );

    QCOMPARE( engine.linkCount(), 3);
    QCOMPARE( engine.linkAt(0), link0);
    QCOMPARE( engine.linkAt(1), link1);
    QCOMPARE( engine.linkAt(2), link2);


}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_comment()
{
    /*
     * $
     * $ Include line contains an ending comment with a quote
     * $
     * INCLUDE "include.dat" $ "
     */

    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/comment/test.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    QCOMPARE( engine.errorCount(), 0);

    std::string link0( "test.dat" );
    std::string link1( "include.dat" );

    QCOMPARE( engine.linkCount(), 2);
    QCOMPARE( engine.linkAt(0), link0);
    QCOMPARE( engine.linkAt(1), link1);
}


/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_cyclic_self_inclusion()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/cyclic/test_self.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    std::string error_msg
            = STR_ERR_CYCLIC
            + std::string("test_self.dat")
            + STR_ERR_AT_LINE
            + std::string("4")
            + STR_ERR_END;

    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0), error_msg);

}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_cyclic_simple()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/cyclic/test_simple.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    std::string error_msg
            = STR_ERR_CYCLIC
            + std::string("included_simple.dat")
            + STR_ERR_AT_LINE
            + std::string("2")
            + STR_ERR_END;

    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0), error_msg);
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_cyclic_complex()
{
    /*
     * test_complex.dat > included_A > included_B > included_C > included_A > ...
     */
    // Given
    Engine engine;
    std::string filename = QFINDTESTDATA("share/cyclic/test_complex.dat").toLatin1().data();
    std::string link_0( "test_complex.dat" );
    std::string link_a( "included_A.dat" );
    std::string link_b( "included_B.dat" );
    std::string link_c( "included_C.dat" );

    // When
    engine.find(filename, "");

    // Then
    std::string error_msg
            = STR_ERR_CYCLIC
            + link_c
            + STR_ERR_AT_LINE
            + std::string("2")
            + STR_ERR_END;

    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0), error_msg);

    QCOMPARE( engine.linkCount(), 4);
    QCOMPARE( engine.linkAt(0), link_0 );
    QCOMPARE( engine.linkAt(1), link_a );
    QCOMPARE( engine.linkAt(2), link_b );
    QCOMPARE( engine.linkAt(3), link_c );
}


/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_duplicate_1()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/duplicate/test_1.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    /// XXX replace by STR_ERR_DUPLICATE ?
    std::string error_msg
            = STR_ERR_CYCLIC
            + std::string("test_1.dat")
            + STR_ERR_AT_LINE
            + std::string("8")
            + STR_ERR_END;

    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0), error_msg);
}


void tst_Engine::test_duplicate_2()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/duplicate/test_2.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    /// XXX replace by STR_ERR_DUPLICATE ?
    std::string error_msg
            = STR_ERR_CYCLIC
            + std::string("included_parent.dat")
            + STR_ERR_AT_LINE
            + std::string("3")
            + STR_ERR_END;

    QCOMPARE( engine.errorCount(), 1);
    QCOMPARE( engine.errorAt(0), error_msg);
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_first_last_line()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/first_last/test.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    QCOMPARE( engine.errorCount(), 0);
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_multiline()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/multiline/test.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    QCOMPARE(engine.errorCount(), 0);

    std::string link0( "test.dat" );
    std::string link1( "./path/to/included_A.dat" );
    std::string link2( "./path/to/../../path/to/../../path/to/../../path/to/included_B.dat" );
    std::string link3( "./path/to/included_C.dat" );
    std::string link4( "./path/to/included_D.dat" );
    std::string link5( "./path/to/included_E.dat" );

    QCOMPARE( engine.linkCount(), 6);
    QCOMPARE( engine.linkAt(0), link0 );
    QCOMPARE( engine.linkAt(1), link1);
    QCOMPARE( engine.linkAt(2), link2);
    QCOMPARE( engine.linkAt(3), link3);
    QCOMPARE( engine.linkAt(4), link4);
    QCOMPARE( engine.linkAt(5), link5);

}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_quotes()
{
    /*
     * $
     * $ Starts with double-quote and ends with simple quote
     * $
     * INCLUDE "include.dat'
     */

    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/quotes/test.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    QCOMPARE( engine.errorCount(), 0);

    std::string link0( "test.dat" );
    std::string link1( "include.dat" );

    QCOMPARE( engine.linkCount(), 2);
    QCOMPARE( engine.linkAt(0), link0);
    QCOMPARE( engine.linkAt(1), link1);
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_single_line()
{
    const int count = 4;
    std::string data[count] = {
        "share/singleline/test_1_windows_simple_quote.dat"  ,
        "share/singleline/test_2_windows_double_quote.dat"  ,
        "share/singleline/test_3_linux_simple_quote.dat"    ,
        "share/singleline/test_4_linux_double_quote.dat"    };
    /// XXX  "share/singleline/test_5_mac_simple_quote.dat"      ,
    /// XXX  "share/singleline/test_6_mac_double_quote.dat"      };

    for (int i = 0; i < count; ++i) {
        // Given,
        Engine engine;
        std::string filename = QFINDTESTDATA( data[i].c_str() ).toLatin1().data();

        //When
        engine.find(filename, "");

        // Then
        QCOMPARE(engine.errorCount(), 0);

        std::string link1( "included_A.dat" );
        std::string link2( "included_B.dat" );
        std::string link3( "./included_C.dat" );
        std::string link4( "./included_D.dat" );
        std::string link5( "./included_E.dat" );

        QCOMPARE(engine.linkCount(), 6);
        QCOMPARE( engine.linkAt(1), link1);
        QCOMPARE( engine.linkAt(2), link2);
        QCOMPARE( engine.linkAt(3), link3);
        QCOMPARE( engine.linkAt(4), link4);
        QCOMPARE( engine.linkAt(5), link5);
    }

}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_subdirectories()
{
    // Given, When
    Engine engine;
    std::string filename = QFINDTESTDATA("share/subdirectory/test/test.dat").toLatin1().data();
    engine.find(filename, "");

    // Then
    QCOMPARE(engine.errorCount(), 0);

    std::string link0( "test.dat" );
    std::string link1( "bulk/included_1.dat" );
    std::string link2( "./bulk/included_2.dat" );
    std::string link3( "../bulk/included_A.dat" );
    std::string link4( "./../bulk/included_B.dat" );

    QCOMPARE( engine.linkCount(), 5);
    QCOMPARE( engine.linkAt(0), link0);
    QCOMPARE( engine.linkAt(1), link1);
    QCOMPARE( engine.linkAt(2), link2);
    QCOMPARE( engine.linkAt(3), link3);
    QCOMPARE( engine.linkAt(4), link4);
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_symbolic_links()
{
    /// XXX symbolic link test under UNIX and MacOS
}

/******************************************************************************
 ******************************************************************************/
void tst_Engine::test_white_spaces()
{
    /// XXX "test with white spaces.dat"
}

/******************************************************************************
 ******************************************************************************/

QTEST_APPLESS_MAIN(tst_Engine)

#include "tst_engine.moc"

