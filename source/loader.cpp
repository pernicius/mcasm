/*
 *
 *    Loader.cpp - this file is part of Microcode Compiler/Assembler
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
#include <string>
#include <algorithm>
#include <iostream>

#include "Loader.h"

// some message macros
#define debug(out)  if (g_cfg.d || g_cfg.d_flags.l) { cout << out << endl; }
#define silent(out) if (!g_cfg.s || g_cfg.d || g_cfg.d_flags.l) { cout << out << endl; }
#define error(out) { cerr << "ERROR: "<< out << endl; }


int Loader::LoadFile(const char* filename) {
    silent("Loading file: '" << filename << "'...");
    
    // check if file is already loaded
    for (int x = 0; x < (int)g_vfiles.size(); ++x) {
        if (g_vfiles[x].compare(filename) == 0) {
            debug("File already loaded: " << filename);
            return 0;
        }
    }

    // add file to list
    string s;
    s.append(filename);
    g_vfiles.push_back(s);
    file_id = g_vfiles.size() - 1;

    // load file
    if (LoadLines() == -1)
        return -1;
    
    // cleanup
    if (Cleanup() == -1)
        return -1;
    debug("Loading file: '" << filename << "' done (" << v_mclines.size() << " lines)");


    // process includes
    if (ProcessIncludes() == -1)
        return -1;

    return 1;
}


int Loader::LoadLines() {
    // open file
    file_ifs.open(g_vfiles.at(file_id), ios::in);
    if (!file_ifs.is_open()) {
        error("Unable to open '" << g_vfiles[file_id] << "'")
        return -1;
    }

    mcLines l;
    l.file_id = file_id;
    l.nline = 0;
    while (file_ifs.good()) {
        // load line
        getline(file_ifs, l.sline);
        l.nline++;
        // convert to upper case
// TODO: find better place for toupper()
//        for_each(l.sline.begin(), l.sline.end(), [](char& in){ in = ::toupper(in); });
        // add line to buffer
        v_mclines.push_back(l);
        debug(g_vfiles[file_id] << "[" << l.nline << "]: " << l.sline);
    } // while

    // close file
    file_ifs.close();

    return 1;
}


int Loader::Cleanup() {
    CleanupPass1();    // pass 1 - remove "rest-of-line" comments (//)
    CleanupPass2();    // pass 2 - remove "multi-line" comments (/* */)
    CleanupPass3();    // pass 3 - remove multiple spaces, tabs, empty lines, trailing spaces and tabs
    return 1;
}


/**
 * @brief remove "rest-of-line" comments
 * 
 * @return int 
 */
int Loader::CleanupPass1() {
    for (unsigned n = 0; n < v_mclines.size(); n++) {
        string sl = v_mclines.at(n).sline;

        // is there a rest-of-line-comment
        size_t p = sl.find("//", 0);
        if (p != string::npos) {
            // remove rest of line
            sl = sl.substr(0, p);
            v_mclines.at(n).sline = sl;
        }
    }
    return 1;
}


/**
 * @brief remove "multi-line" comments
 * 
 * @return int 
 */
int Loader::CleanupPass2() {
    bool bcomment = false;
    for (unsigned n = 0; n < v_mclines.size(); n++) {
        string sl = v_mclines.at(n).sline;
        // inside comment...
        if (bcomment) {
            size_t e = sl.find("*/", 0);

            // end found...
            if (e != string::npos) {
                bcomment = false;
                e += 2;
                sl.replace(0, e, "");
                v_mclines.at(n).sline = sl;
            }

            // end not found
            if (e == string::npos) {
                v_mclines.erase(v_mclines.begin() + n);
                n--;
            }
        }
        // not inside comment...
        else {
            size_t s = sl.find("/*", 0);
            size_t e = sl.find("*/", 0);

            // part of line..
            if ((s != string::npos) && (e != string::npos)) {
                e += 2;
                sl.replace(s, e-s, "");
                v_mclines.at(n).sline = sl;
            }

            // only start of comment...
            if ((s != string::npos) && (e == string::npos)) {
                bcomment = true;
                sl.replace(s, string::npos, "");
                v_mclines.at(n).sline = sl;
            }
        }
    }
    return 1;
}


/**
 * @brief remove multiple spaces, tabs, empty lines, trailing spaces and tabs
 * 
 * @return int 
 */
int Loader::CleanupPass3() {
    for (unsigned n = 0; n < v_mclines.size(); n++) {
        string sl = v_mclines.at(n).sline;

        // replace all "\t" with " "
        size_t r = sl.find("\t", 0);
        while(r != string::npos) {
            sl.replace(r, 1, " ");
            r = sl.find("\t", 0);
        }
        // replace all "  " with " "
        r = sl.find("  ", 0);
        while(r != string::npos) {
            sl.replace(r, 2, " ");
            r = sl.find("  ", 0);
        }
        v_mclines.at(n).sline = sl;

        // find first real char (no space)
        size_t p = sl.find_first_not_of(" ");
        if (p != string::npos) {
            // trim begin of line
            sl = sl.substr(p, string::npos);
            v_mclines.at(n).sline = sl;
        }
        else {
            // line is empty ... remove
            v_mclines.erase(v_mclines.begin() + n);
            n--;
            continue;
        }

        // find last real char (no space)
        p = sl.find_last_not_of(" ", string::npos);
        if ((p != string::npos) && (p != (sl.length() - 1))) {
            // trim end of line
            sl = sl.substr(0, p + 1);
            v_mclines.at(n).sline = sl;
        }
    }
    return 1;
}


int Loader::ProcessIncludes() {
    for (unsigned n = 0; n < v_mclines.size(); n++) {
        // find next #include
        if (v_mclines.at(n).sline.compare(0, 8, "#include") == 0) {
            int s = v_mclines.at(n).sline.find('"', 0) + 1;
            int l = v_mclines.at(n).sline.find('"', s) - s;

            debug("#include: " << v_mclines.at(n).sline.substr(s, l));

            // load include-file
            Loader nl;
            if (nl.LoadFile(v_mclines.at(n).sline.substr(s, l).c_str()) == -1) {
                return -1;
            }
            // remove #include
            v_mclines.erase(v_mclines.begin()+n);
            // replace with loadet lines
            v_mclines.insert(v_mclines.begin()+n, nl.v_mclines.begin(), nl.v_mclines.end());
            // update pointer to the end of the new lines
            n += nl.v_mclines.size() - 1;
        }
    }

    return 1;
}
