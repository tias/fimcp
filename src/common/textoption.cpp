/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: textoption.cpp 231 2009-06-04 09:51:59Z tias $
 *
 *  This file is part of Fim_cp, Frequent Itemset Mining using
 *  Constraint Programming, and uses Gecode.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef __FIMCP_COMMON_TEXTOPTION__
#define __FIMCP_COMMON_TEXTOPTION__

#include "textoption.hh"

#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <cstring>

/*
 * String option
 *
 */
bool
TextOption::parse(int& argc, char* argv[]) {
  if ((argc < 2) || strcmp(argv[1],opt))
    return false;
  if (argc == 2) {
    std::cerr << "Missing argument for option \"" << opt << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }
  cur = argv[2];
  // Remove options
  argc -= 2;
  for (int i=1; i<argc; i++)
    argv[i] = argv[i+2];
  return true;
}

void 
TextOption::help(void) {
  std::cerr << '\t' << opt << " (filename with extention)"
            << " default: " << cur << std::endl
            << "\t\t" << exp << std::endl;
}

TextOption::~TextOption(void) {}

#endif
