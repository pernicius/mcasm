/*
 *
 *    main.cpp - this file is part of Microcode Compiler/Assembler
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
#include <cstring>

#include <iostream>
#include <vector>

#include "Loader.h"
#include "Parser.h"
#include "globals.h"

// config
config_t g_cfg;
// holds all loaded file names
vector<string> g_vfiles;
// holds all loaded code lines
vector<mcLines> g_vlines;


void print_help() {
	cout << "Usage: mcasm [options] [source] [target]" << endl;
	cout << "  source is mandatory" << endl;
	cout << "  target is optional" << endl;
	cout << "Options:" << endl;
//	cout << "  -f=[format]             Outputformat:" << endl;
//	cout << "      logisim                 Logisim v2.0 raw (default)" << endl;
//	cout << "      hex                     Intel hex" << endl;
//	cout << "      bin                     Binary" << endl;
	cout << "  -d, --debug             Print lots of debugging information" << endl;
	cout << "  --debug=[FLAGS]         Print lots of debugging information during..." << endl;
	cout << "      l                       ...file load" << endl;
	cout << "      p                       ...parsing" << endl;
	cout << "      g                       ...generating" << endl;
	cout << "  -h, --help              Print this message" << endl;
	cout << "  -s, --silent, --quiet   Don't echo messages, only errors" << endl;
	cout << "  -v, --version           Print the version info and exit" << endl;
	cout << "" << endl;
	cout << "Report bugs to <pernicius@web.de>" << endl;
	cout << "" << endl;
}


void print_version() {
	cout << "Simple Microcode Compiler/Assembler";
#ifdef VERSION_MAJOR
	cout << " " << VERSION_MAJOR;
#ifdef VERSION_MINOR
	cout << "." << VERSION_MINOR;
#ifdef VERSION_BUGFIX
	cout << "." << VERSION_BUGFIX;
#endif
#ifdef VERSION_XDEV
	cout << "-dev";
#endif
#ifdef VERSION_XALPHA
	cout << "-alpha";
#endif
#ifdef VERSION_XBETA
	cout << "-beta";
#endif
#ifdef VERSION_XRC
	cout << "-RC" << VERSION_XRC;
#endif
#endif
#ifdef DEBUG
	cout << "+debug";
#endif
#endif
	cout << endl;
//#ifdef MAKE_HOST
//	cout << "(Built for: " << MAKE_HOST << ")" << endl;
//#endif
//	cout << "Built at " << __DATE__ << " " << __TIME__ << endl;
	cout << "Copyright (C) 2017-2022 Lennart Molnar <pernicius@web.de>" << endl;
	cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << endl;
	cout << "This is free software: you are free to change and redistribute it." << endl;
	cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
	cout << "" << endl;
}


int main (int argc, char * const argv[]) {
	string in_file;
	string out_file;

	// check num of arguments
	if (argc < 2) {
		print_version();
		print_help();
		return -1;
	}
	else {
		// defaults
		g_cfg.d           = false; // debug
		g_cfg.d_flags.set = false; // any flag is set?
		g_cfg.d_flags.l   = false; // debug FileLoader
		g_cfg.d_flags.p   = false; // debug Parser
		g_cfg.d_flags.g   = false; // debug Generator
		g_cfg.s           = false; // silent

		// check cmdline options
		for (int ac = 1; ac < argc; ac++) {
			// print help
			if ((0 == strcmp(argv[ac], "-h")) || (0 == strcmp(argv[ac], "--help"))) {
				print_help();
				return 0;
			}
			// print version
			if ((0 == strcmp(argv[ac], "-v")) || (0 == strcmp(argv[ac], "--version"))) {
				print_version();
				return 0;
			}
			// set debug mode
			if ((0 == strcmp(argv[ac], "-d")) || (0 == strcmp(argv[ac], "--debug"))) {
				g_cfg.d = true;
				continue;
			}
			if (0 == strncmp(argv[ac], "--debug=", 8)) {
				string tst = argv[ac];
				for (int n = 8; n < (int)tst.length(); n++) {
					switch(tst[n]) {
						case 'l': g_cfg.d_flags.set = g_cfg.d_flags.l = true; break;
						case 'p': g_cfg.d_flags.set = g_cfg.d_flags.p = true; break;
						case 'g': g_cfg.d_flags.set = g_cfg.d_flags.g = true; break;
					}
				}
				continue;
			}
			// set silent mode
			if ((0 == strcmp(argv[ac], "-s")) || (0 == strcmp(argv[ac], "--silent")) || (0 == strcmp(argv[ac], "--quiet"))) {
				g_cfg.s = true;
				continue;
			}
			// is existing file?
			if (FILE *file = fopen(argv[ac], "r")) {
				fclose(file);
				if (in_file.empty())
					in_file = argv[ac];
				else if (out_file.empty())
					out_file = argv[ac];
				else {
					print_help();
					return -1;
				}
				continue;
			} // fopen
			else {
				if (!in_file.empty() && out_file.empty())
					out_file = argv[ac];
				else {
					print_help();
					return -1;
				}
				continue;
			} // !fopen
		} // for
	} // !(argc < 2)

	// no source given :(
	if(in_file.empty()) {
		print_help();
		return -1;
	}

	// default target
	if(out_file.empty())
		out_file = "rom%d.hex";

	// print debug config
	if (g_cfg.d || g_cfg.d_flags.set) {
		cout << "Config used:" << endl;
		cout << "  debug[" << (g_cfg.d ? "ON" : "OFF") << "]" << endl;
		if (g_cfg.d_flags.set) {
			cout << "    flag l [" << (g_cfg.d_flags.l ? "ON" : "OFF") << "]" << endl;
			cout << "    flag p [" << (g_cfg.d_flags.p ? "ON" : "OFF") << "]" << endl;
			cout << "    flag g [" << (g_cfg.d_flags.g ? "ON" : "OFF") << "]" << endl;
		}
		cout << "  silent[" << (g_cfg.s ? "ON" : "OFF") << "]" << endl;
		cout << "  source: ";
		if(!in_file.empty()) cout << in_file << endl; else cout << "unset!" << endl;
		cout << "  target: " << out_file << endl;
	}

	// load source
	Loader*loader = new Loader();
	if (loader->LoadFile(in_file.c_str()) == -1) {
		return -1;
	}
	g_vlines = loader->v_mclines;
	delete loader;

	// Parse
	Parser*parser = new Parser();
	if (parser->Parse() == -1) {
		return -1;
	}

	// Generate
	if (parser->Generate(out_file.c_str()) == -1) {
		return -1;
	}

	delete parser; // will be deleted after generator because it holds all the parsed data
	return 0;
}
