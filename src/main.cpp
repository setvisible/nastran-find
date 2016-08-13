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

#include "global.h"
#include "version.h"
#include "application.h"

#include <iostream>
#include <string>

using namespace std;

void intro()
{
    cout << endl;
    cout << "- NASTRANFIND - Copyright (C) 2016 Sebastien Vavassori" << endl;
    cout << "This program comes with ABSOLUTELY NO WARRANTY." << endl;
    cout << "This is free software, and you are welcome to redistribute it" << endl;
    cout << "under certain conditions; type '-v' for details." << endl;
    cout << endl;
}

void usage()
{
    cout << endl;
    cout << "- NASTRANFIND - " << endl;
    cout << endl;
    cout << " [DESCRIPTION] " << endl;
    cout << "    NASTRANFIND is a solution for test search in NASTRAN input files." << endl;
    cout << endl;
    cout << " [USAGE] " << endl;
    cout << "    nastranfind [options] filename" << endl;
    cout << endl;
    cout << " [OPTIONS]" << endl;
    cout << "    -h or --help     Displays this help." << endl;
    cout << "    -v or --version  Displays the version. " << endl;
    cout << "    --reset-config    Clears the saved preference parameters." << endl;
    cout << endl;
}

void version()
{
    cout << endl;
    cout << "- NASTRANFIND - Copyright (C) 2016 Sebastien Vavassori" << endl;
    cout << "Version " << NF_VERSION_LONG << " build " << __DATE__ << " " << __TIME__ << endl;
    cout << endl;
    cout << "This program is free software: you can redistribute it and/or modify" << endl;
    cout << "it under the terms of the GNU Lesser General Public License as published by" << endl;
    cout << "the Free Software Foundation, either version 3 of the License, or" << endl;
    cout << "(at your option) any later version." << endl;
    cout << endl;
    cout << "This program is distributed in the hope that it will be useful," << endl;
    cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
    cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl;
    cout << "GNU Lesser General Public License for more details." << endl;
    cout << endl;
    cout << "You should have received a copy of the GNU Lesser General Public License" << endl;
    cout << "along with this program.  If not, see <http://www.gnu.org/licenses/>." << endl;
    cout << endl;
}

int main( int argc, char *argv[] )
{
    bool forceResetConfig = false;
    string filename;
    for( int i = 1; i < argc; ++i ){
        string arg(argv[i]);

        if ( arg == "--reset-config" ) {
            forceResetConfig = true;
        } else if (arg == "--help" || arg == "-h") {
            usage();
            return 0;
        } else if (arg == "--version" || arg == "-v") {
            version();
            return 0;
        } else {
            intro();
            filename = arg;
        }
    }

    Application app(argc, argv);

    if( forceResetConfig ){
        app.resetConfig();
        cout << "Config cleaned." << endl;
    }

    if( filename.empty() ){
        cout << "Error: Need an argument; type '-h' for details." << endl;
    }

    app.setFilename( filename );
    return app.exec();
}
