/*
 *
 *    Loader.h - this file is part of Microcode Compiler/Assembler
 *
 *    Copyright (C) 2017-2022 Lennart Molnar <pernicius@web.de>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef LOADER_H_
#define LOADER_H_


#include <fstream>
#include <string>

#include "globals.h"


using namespace std;


class Loader
{
    int      file_id;   // id in g_vfiles
    ifstream file_ifs;  // internal file stream
    string   file_line; // current line buffer

    int LoadLines();
    int Cleanup();
    int CleanupPass1();
    int CleanupPass2();
    int CleanupPass3();
    int ProcessIncludes();

public:
    vector<mcLines> v_mclines;

    int LoadFile(const char* filename);
};


#endif /* LOADER_H_ */
