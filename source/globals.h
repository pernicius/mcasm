/*
 *
 *    globals.h - this file is part of Microcode Compiler/Assembler
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
#ifndef GLOBALS_H_
#define GLOBALS_H_


#include <string>
#include <vector>

using namespace std;


/*
 * return values for class methods:
 *  -1 error
 *   0 ok ... but nothing done
 *   1 ok
 */


// holds the configuration (changeable with cmdline options)
typedef struct config {
    bool d; // debug
    struct {
        bool set; // any flag set?
        bool l;   // debug FileLoader
        bool p;   // debug Parser
    } d_flags;
    bool s; // silent
} config_t;
extern config_t g_cfg;


// holds all loaded file names
extern vector<string> g_vfiles;


// holds all loaded code lines
typedef struct mcLines {
    int    file_id;
    string sline;
    int    nline;
} mcLines;
extern vector<mcLines> g_vlines;


#endif /* GLOBALS_H_ */
