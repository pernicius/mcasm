/*
 *
 *    Parser.h - this file is part of Microcode Compiler/Assembler
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
#ifndef PARSER_H_
#define PARSER_H_


#include <string>
#include <vector>

#include "globals.h"


using namespace std;


typedef struct inputs {
    int nstart;     // num of first bit
    int nend;       // num of last bit
    int nnum;       // number of bits
    string sname;   // identifier
} inputs_t;


typedef struct signals {
    int nchip;      // number of the chip
    int nstart;     // if nnum>1 ; lowest bit else same as nend
    int nend;       // if nnum>1 ; highest bit else num of bit
    int nnum;       // number of bits
    string sname;   // identifier
    int defval;     // default value
} signals_t;


typedef struct defs {
    string sname;       // identifier
    string scontent;    // replacement
} defs_t;


typedef struct ops {
    int nival;              // value of all inputs
    int nimask;             // used bits of all inputs
    vector<int> vnsignals;  // value of signals (one for each chip)
    string sname;           // name (optional for debugging)
} ops_t;


class Parser
{
    // just an iteratur to the current line of g_vlines
    vector<mcLines>::iterator cur_line;

    // internal database
    vector<inputs_t>  vinputs;
    vector<signals_t> vsignals;
    vector<defs_t>    vdefs;
    vector<ops_t>     vops;
    int signals_nchips;
    int signals_nbits;
    int inputs_nbits;

    size_t _findSignal(const string& str);
    size_t _findDefine(const string& str);

    int _parseNum(size_t pos, size_t *next);
    size_t _parseDelim(size_t pos, const char *delim);
    string _parseName(size_t pos, size_t *next);

    int ParseInputs();
    int ParseSignals();
    int ParseDefines();
    int ParseDefaults();
    int ParseOpcode();

public:
    int Parse();
};


#endif /* PARSER_H_ */
