/*
 *
 *    Parser.cpp - this file is part of Microcode Compiler/Assembler
 *
 *    Copyright (C) 2017 Lennart Molnar <pernicius@web.de>
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
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "Parser.h"

#define debug_gen(out)  if (g_cfg.d || g_cfg.d_flags.g) { cout << out << endl; }
#define silent_gen(out) if (!g_cfg.s || g_cfg.d || g_cfg.d_flags.g) { cout << out << endl; }
// some message macros
#define debug(out)  if (g_cfg.d || g_cfg.d_flags.p) { cout << out << endl; }
#define silent(out) if (!g_cfg.s || g_cfg.d || g_cfg.d_flags.p) { cout << out << endl; }
#define error(out) { cerr << "ERROR: "<< out << endl; }
#define parse_error(msg) { cerr \
    << g_vfiles.at(cur_line->file_id) << ":" << cur_line->nline << ": error: " << msg << endl \
    << "~~~> " << cur_line->sline << endl; }
string spaces = "                                                                                ";
#define parse_error_pos(pos, msg) { parse_error(msg) \
    cerr << "     " << spaces.substr(0, pos) << "^" << endl; }


vector<string> split(const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}


string cout_int2bin(int bnum, int len = sizeof(int)*8) {
    cout << "0b";
    for (int i = len-1; i >= 0; --i) {
        cout << ((bnum >> i) & 1);
    }
    return "";
}


size_t Parser::_findSignal(const string& str) {
	for (size_t x = 0; x < vsignals.size(); ++x) {
		if (vsignals[x].sname == str)
			return x;
	}
	return string::npos;
}


size_t Parser::_findDefine(const string& str) {
	for (size_t x = 0; x < vdefs.size(); ++x) {
		if (vdefs[x].sname == str)
			return x;
	}
	return string::npos;
}


int Parser::_parseNum(size_t pos, size_t *next) {
    size_t p1, p2;
    int num;

    p1 = cur_line->sline.find_first_not_of(" ", pos);
    p2 = cur_line->sline.find_first_of("0123456789", pos);
    if ((p1 == string::npos) || (p1 != p2)) {
        *next = string::npos;
        return -1;
    }

    num = stoi(cur_line->sline.substr(p1), &p2);
    *next = pos + p2;
    return num;
}


size_t Parser::_parseDelim(size_t pos, const char *delim) {
    size_t p1, p2;

    p1 = cur_line->sline.find_first_not_of(" ", pos);
    p2 = cur_line->sline.find_first_of(delim, pos);
    if ((p1 == string::npos) || (p2 == string::npos) || (p1 != p2))
        return string::npos;
    
    p1 = cur_line->sline.find_first_not_of(delim, p2);
    if (p1 == string::npos)
        return cur_line->sline.size();
    
    return p1;
}


string Parser::_parseName(size_t pos, size_t *next) {
    size_t p1, p2;

    p1 = cur_line->sline.find_first_not_of(" ", pos);
    p2 = cur_line->sline.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", pos);
    if ((p1 == string::npos) || (p2 == string::npos) || (p1 != p2)) {
        *next = string::npos;
        return "";
    }
    p2 = cur_line->sline.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", p1);
    *next = p2;
    if (p2 == string::npos)
        return cur_line->sline.substr(p1);
    else
        return cur_line->sline.substr(p1, p2 - p1);
}


// #input { }
int Parser::ParseInputs() {
    for (++cur_line; cur_line != g_vlines.end(); ++cur_line) {
        size_t p1, p2;
        inputs_t new_inputs = {0, 0, 0, ""};
        int a, b;

        if (cur_line->sline.find("}", 0) != string::npos) {
            return 1;
        }
        // bit or start-bit
        a = b = _parseNum(0, &p1);
        if (p1 == string::npos) {
            parse_error_pos(0, "Numeric value (bit position).. expected!");
            return -1;
        }
        // multi-bit?
        if ((p2 = _parseDelim(p1, ".")) != string::npos) {
            b = _parseNum(p2, &p1);
            if (p1 == string::npos) {
                parse_error_pos(p2, "Numeric value (bit position).. expected!");
                return -1;
            }
            new_inputs.nstart = min(a, b);
            new_inputs.nend   = max(a, b);
            new_inputs.nnum = 1 + new_inputs.nend - new_inputs.nstart;
        }
        // single-bit!
        else {
            new_inputs.nstart = a;
            new_inputs.nend = new_inputs.nstart;
            new_inputs.nnum = 1;
        }
        inputs_nbits = max(max(a, b), inputs_nbits);
        // identifier
        if ((p2 = _parseDelim(p1, "=")) == string::npos) {
            parse_error_pos(p1, "Delimiter character '=' expected!");
            return -1;
        }
        new_inputs.sname = _parseName(p2, &p1);
        if (new_inputs.sname.empty()) {
            parse_error_pos(p2, "Identifier (name of input) expected!");
            return -1;
        }

        // insert in list
        vinputs.push_back(new_inputs);

        // debug info
        if (new_inputs.nnum > 1) {
            debug("New input: multi bit(" << new_inputs.nnum << ") id:" << new_inputs.sname << " bits:" << new_inputs.nstart << ".." << new_inputs.nend);
        } else {
            debug("New input: single bit id:" << new_inputs.sname << " bit:" << new_inputs.nstart);
        }
    }
    return 1;
}


// #signals { }
int Parser::ParseSignals() {
    for (++cur_line; cur_line != g_vlines.end(); ++cur_line) {
        size_t p1, p2;
        signals_t new_signal = {0, 0, 0, 0, "", 0};
        int a, b;

        if (cur_line->sline.find("}", 0) != string::npos) {
            return 1;
        }

        // chip
        new_signal.nchip = _parseNum(0, &p1);
        signals_nchips = max(signals_nchips, new_signal.nchip);
        if (p1 == string::npos) {
            parse_error_pos(0, "Numeric value (chip/byte number) expected!");
            return -1;
        }
        // delim
        if ((p2 = _parseDelim(p1, ":")) == string::npos) {
            parse_error_pos(p1, "Delimiter character ':' expected!");
            return -1;
        }
        // bit or start-bit
        a = b = _parseNum(p2, &p1);
        if (p1 == string::npos) {
            parse_error_pos(0, "Numeric value (bit position).. expected!");
            return -1;
        }
        // multi-bit?
        if ((p2 = _parseDelim(p1, ".")) != string::npos) {
            b = _parseNum(p2, &p1);
            if (p1 == string::npos) {
                parse_error_pos(p2, "Numeric value (bit position).. expected!");
                return -1;
            }
            new_signal.nstart = min(a, b);
            new_signal.nend   = max(a, b);
            new_signal.nnum = 1 + new_signal.nend - new_signal.nstart;
        }
        // single-bit!
        else {
            new_signal.nstart = a;
            new_signal.nend = new_signal.nstart;
            new_signal.nnum = 1;
        }
        signals_nbits = max(max(a, b), signals_nbits);
        // identifier
        if ((p2 = _parseDelim(p1, "=")) == string::npos) {
            parse_error_pos(p1, "Delimiter character '=' expected!");
            return -1;
        }
        new_signal.sname = _parseName(p2, &p1);
        if (new_signal.sname.empty()) {
            parse_error_pos(p2, "Identifier (name of signal) expected!");
            return -1;
        }

        // insert in list
        vsignals.push_back(new_signal);

        // debug info
        if (new_signal.nnum > 1) {
            debug("New signal: multi bit(" << new_signal.nnum << ") id:" << new_signal.sname << " chip:" << new_signal.nchip << " bits:" << new_signal.nstart << ".." << new_signal.nend);
        } else {
            debug("New signal: single bit id:" << new_signal.sname << " chip:" << new_signal.nchip << " bit:" << new_signal.nstart);
        }
    }
    return 1;
}


// #define xyz (xyz)
int Parser::ParseDefines() {
    size_t p1, p2, p3;
    defs_t new_def = {"", ""};

    // Identifier
    new_def.sname = _parseName(7, &p1);
    if (new_def.sname.empty()) {
        parse_error_pos(7, "Identifier (of definition) expected!");
        return -1;
    }
    if ((p2 = cur_line->sline.find_first_of("(", p1)) == string::npos) {
        parse_error_pos(p1, "Begin of replacement '(' expected!");
        return -1;
    }
    ++p2;
    if ((p3 = cur_line->sline.substr(p2).find_last_of(")")) == string::npos) {
        parse_error_pos(cur_line->sline.size(), "End of replacement ')' expected!");
        return -1;
    }
    new_def.scontent = cur_line->sline.substr(p2, p3);
    
    // add to list
    vdefs.push_back(new_def);

    // debug info
    debug("New definition: id:" << new_def.sname << " rep:\"" << new_def.scontent << "\"");

    return 1;
}


// #defaults { }
int Parser::ParseDefaults() {
    for (++cur_line; cur_line != g_vlines.end(); ++cur_line) {
        size_t p1, p2, sn;
        int val;
        string ss, sv;

        if (cur_line->sline.find("}", 0) != string::npos) {
            return 1;
        }

        // signal
        ss = _parseName(0, &p1);
        if (ss.empty()) {
            parse_error_pos(7, "Identifier (of Signal) expected!");
            return -1;
        }
        // delim
        if ((p2 = _parseDelim(p1, "=")) == string::npos) {
            parse_error_pos(p1, "Delimiter character '=' expected!");
            return -1;
        }
        // value
        if ((p1 = cur_line->sline.find_first_of("0123456789bx", p2)) == string::npos) {
            parse_error_pos(p2, "Numeric value (for default value) expected!");
            return -1;
        }
        sv = cur_line->sline.substr(p1);
        // save
        val = stoi(sv);
        if (sv.size() > 2) {
            if (sv.substr(0, 2) == "0x")
                val = stoi(sv.substr(2), 0, 16);
            else if (sv.substr(0, 2) == "0b")
                val = stoi(sv.substr(2), 0, 2);
        }
        sn = _findSignal(ss);
        if (sn == string::npos) {
            parse_error("Signal not defined!");
            return -1;
        }
        vsignals[sn].defval = val;

        // debug info
        debug("New default " << ss << "=" << sv << "(" << val << ")");
    }
    return 1;
}


// #op( ... ) { }
int Parser::ParseOpcode() {
    size_t p1, p2;
    vector<string> vi, vs;
    ops_t new_op = {0, 0, {}, "???"};

    // name (optional)
    p1 = cur_line->sline.find('{');
    p1 = cur_line->sline.find_first_not_of(' ', p1+1);
    if (p1 != string::npos)
        new_op.sname = cur_line->sline.substr(p1);
    // inputs
    p1 = cur_line->sline.find('(');
    p2 = cur_line->sline.find(')');
    vi = split(cur_line->sline.substr(p1+1, p2-p1-1), ','); // split by ','
    for (size_t i = 0; i < vi.size(); ++i) {
        int ival=0, imask=0;
        string ss;

        // trim
		p1 = vi[i].find_first_not_of(' ');
        p2 = vi[i].find_last_not_of(' ');
        ss = vi[i].substr(p1, p2-p1+1);
        vi[i] = ss;
        // test for defined identifier
        if ((p1 = _findDefine(ss)) != string::npos) {
            vi[i] = vdefs[p1].scontent;
        }
        // convert to num
        if ((vi[i][0] != 'x') && (vi[i][0] != '*')) {
            if (vi[i].size() > 2) {
                if (vi[i].substr(0, 2) == "0b")
                    ival = stoi(vi[i].substr(2), &p1, 2);
                if (vi[i].substr(0, 2) == "0x")
                    ival = stoi(vi[i].substr(2), &p1, 16);
            }
            else
                ival = stoi(vi[i], &p1);
            // mask
            for (int x=0; x < vinputs[i].nnum; x++)
                imask = (imask << 1) + 1;
            // lshift to position of input
            ival = ival << vinputs[i].nstart;
            imask = imask << vinputs[i].nstart;
            // save
            new_op.nival += ival;
            new_op.nimask += imask;
        }
	}

    // signals
    for (++cur_line; cur_line != g_vlines.end(); ++cur_line) {
        vector<string> vstemp;

        if (cur_line->sline.find("}", 0) != string::npos) {
            break;
        }
        p1 = cur_line->sline.find_first_not_of(' ');
        p2 = cur_line->sline.find_last_not_of(' ');
        if((p1 == string::npos) || (p2 == string::npos)) {
            parse_error("List of signals expected!");
        }
        vstemp = split(cur_line->sline.substr(p1, p2-p1+1), ','); // split by ','
        for (size_t i = 0; i < vstemp.size(); ++i) { // trim
            p1 = vstemp[i].find_first_not_of(' ');
            p2 = vstemp[i].find_last_not_of(' ');
            string ss = vstemp[i].substr(p1, p2-p1+1);
            vstemp[i] = ss;
        }
        vs.insert(vs.end(), vstemp.begin(), vstemp.end());
    }
    // signals - generate with default values
    for (int x=0; x <= signals_nchips; ++x)
        new_op.vnsignals.insert(new_op.vnsignals.begin(), 0);
    for (auto s : vsignals)
        new_op.vnsignals[s.nchip] += s.defval << s.nstart;
    // signals - parse
    for (auto s : vs) {
        size_t nsig;
        int val=0, mask=0;
        // inverted...
        if (s[0] == '!') {
            if ((nsig = _findSignal(s.substr(1))) == string::npos) {
                error("Signal '" << s.substr(1) << "' not defined!");
                return -1;
            }
            for (int x=0; x < vsignals[nsig].nnum; x++)
                mask = (mask << 1) + 1;
            val = mask & (~vsignals[nsig].defval);
            val = val << vsignals[nsig].nstart;
            mask = mask << vsignals[nsig].nstart;
            new_op.vnsignals[vsignals[nsig].nchip] = new_op.vnsignals[vsignals[nsig].nchip] & (~mask);
            new_op.vnsignals[vsignals[nsig].nchip] += val;
        }
        // signal=value
        else if ((p1 = s.find('=')) != string::npos) {
            if ((nsig = _findSignal(s.substr(0, p1))) == string::npos) {
                error("Signal '" << s.substr(0, p1) << "' not defined!");
                return -1;
            }
            val = stoi(s.substr(p1+1));
            if (s.size() > p1+3) {
                if (s[p1+2] == 'x')
                    val = stoi(s.substr(p1+3), 0, 16);
                else if (s[p1+2] == 'b')
                    val = stoi(s.substr(p1+3), 0, 2);
            }
            for (int x=0; x < vsignals[nsig].nnum; x++)
                mask = (mask << 1) + 1;
            val = val << vsignals[nsig].nstart;
            mask = mask << vsignals[nsig].nstart;
            new_op.vnsignals[vsignals[nsig].nchip] = new_op.vnsignals[vsignals[nsig].nchip] & (~mask);
            new_op.vnsignals[vsignals[nsig].nchip] += val;
        }
        // just the signal name (all bits to 1)
        else {
            if ((nsig = _findSignal(s)) == string::npos) {
                error("Signal '" << s << "' not defined!");
                return -1;
            }
            for (int x=0; x < vsignals[nsig].nnum; x++)
                val = (val << 1) + 1;
            val = val << vsignals[nsig].nstart;
            new_op.vnsignals[vsignals[nsig].nchip] |= val;
        }
    }

    // insert in list
    vops.push_back(new_op);

    // debug info
    debug("New opcode(" << new_op.sname << ")");
    debug("  inputs value: " << cout_int2bin(new_op.nival, inputs_nbits+1));
    debug("  inputs mask:  " << cout_int2bin(new_op.nimask, inputs_nbits+1));
    for (int x=0; x <= signals_nchips; ++x)
        debug("  signals[" << x << "]: " << cout_int2bin(new_op.vnsignals[x], signals_nbits+1));

    return 1;
}


int Parser::Parse() {
    silent("Parsing...");
    for (cur_line = g_vlines.begin(); cur_line != g_vlines.end(); ++cur_line) {
        // #inputs {
        if ((cur_line->sline.compare(0, 7, "#inputs") == 0) &&
                (cur_line->sline.find("{") != string::npos)) {
            if (ParseInputs() == -1)
                return -1;
            silent("Number of input bits found: 0.." << inputs_nbits);
            continue;
        }
        // #signals {
        if ((cur_line->sline.compare(0, 8, "#signals") == 0) &&
                (cur_line->sline.find("{") != string::npos)) {
            if (ParseSignals() == -1)
                return -1;
            if (signals_nchips > 0) {
                silent("Number of signal bits found: 0.." << signals_nchips << ":0.." << signals_nbits);
            } else {
                silent("Number of signal bits found: 0:0.." << signals_nbits);
            }
            continue;
        }
        // #define
        if (cur_line->sline.compare(0, 7, "#define") == 0) {
            if (ParseDefines() == -1)
                return -1;
            continue;
        }
        // #defaults
        if ((cur_line->sline.compare(0, 9, "#defaults") == 0) &&
                (cur_line->sline.find("{") != string::npos)) {
            if (signals_nbits == 0) {
                parse_error("Signals must be defined before!");
                return -1;
            }
            if (ParseDefaults() == -1)
                return -1;
            continue;
        }
        // #op() {
        if ((cur_line->sline.compare(0, 3, "#op") == 0) &&
                (cur_line->sline.find("(") != string::npos) &&
                (cur_line->sline.find(")") != string::npos) &&
                (cur_line->sline.find("{") != string::npos)) {
            if ((inputs_nbits == 0) || (signals_nbits == 0)) {
                parse_error("Inputs and signals must be defined before!");
                return -1;
            }
            if (ParseOpcode() == -1)
                return -1;
            continue;
        }
        // none of the above -> error
        parse_error("Unknown command!");
        return -1;
    }

    silent("Parsing... done (" << vops.size() << " ops/instructions)");
    return 1;
}


int Parser::Generate(const string& out_file) {
    silent("Generating...");

    int nmaxinval=0;
    int nmatches=0;
    vector<FILE *> vfile;
    vector<int> vndefault;
    vector<string> vsdefault;

    // max inval
    for (int x=0; x <= inputs_nbits; x++)
        nmaxinval = (nmaxinval << 1) + 1;

    // create outfiles
    silent("Opening target files...");
    for (int x=0; x <= signals_nchips; ++x) {
        FILE * pfile;
        string sfile;

        // i hate that code snipped...
        char buf[128];
        int n;
        n = snprintf(buf, sizeof(buf), out_file.c_str(), x);
        if (n<0) {
            error("Internal error: parser.cpp " << __LINE__);
            return -1;
        }
        sfile = buf;

        silent("File: " << sfile);
        if ((pfile = fopen(sfile.c_str(), "w")) == NULL) {
            error("Can't open file: " << sfile);
            return -1;
        }
        // write header for Logisim format
        fputs("v2.0 raw\n", pfile);
        // add to filelist
        vfile.push_back(pfile);
    }

    // default signals value
    for (int x=0; x<=signals_nchips; ++x) {
        vndefault.push_back(0);
        vsdefault.push_back("");
    }
    for (auto s : vsignals)
        vndefault[s.nchip] += s.defval << s.nstart;
    for (int x=0; x<=signals_nchips; ++x) {
        char buf[128];
        sprintf(buf, "%X", vndefault[x]);
        vsdefault[x] = buf;
    }
    
    // the loop
    for (int inval=0; inval<=nmaxinval; inval++) {
        bool match=false;

        // check if opcode matches
        for (size_t i=0; i < vops.size(); ++i) {
            if ((inval & vops[i].nimask) == vops[i].nival) {
                debug_gen("Match: " << cout_int2bin(inval, inputs_nbits+1) << " => " << vops[i].sname);
                ++nmatches;
                match = true;
                // write matching signals
                for (int x=0; x<=signals_nchips; ++x) {
                    char buf[128];
                    sprintf(buf, "%X\n", vops[i].vnsignals[x]);
                    fputs(buf, vfile[x]);
                }
                break;
            }
        }
        // no match found
        if (!match) {
            // write default signals
            for (int x=0; x<=signals_nchips; ++x) {
                fputs(vsdefault[x].c_str(), vfile[x]);
                fputs("\n", vfile[x]);
            }
        }
    }
    
    silent("Generating... done (" << nmatches << " matches)");
    return 1;
}
