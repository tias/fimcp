/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: stringuintoption.cpp 231 2009-06-04 09:51:59Z tias $
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
#ifndef __FIMCP_COMMON_STRINGUINTOPTION__
#define __FIMCP_COMMON_STRINGUINTOPTION__

#include "stringuintoption.hh"

#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <cstring>

/*
 * String option
 *
 */
void 
StringUIntOption::add(int v, const char* o, const char* h) {
  Value* n = new Value;
  n->val  = v;
  n->opt  = o;
  n->help = h;
  n->next = NULL;
  if (fst == NULL) {
    fst = n;
  } else {
    lst->next = n; 
  }
  lst = n;
}

bool
StringUIntOption::parse(int& argc, char* argv[]) {
  if ((argc < 3) || strcmp(argv[1],opt))
    return false;
  if (argc == 3) {
    std::cerr << "Missing argument for option \"" << opt << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }
  for (Value* v = fst; v != NULL; v = v->next)
    if (!strcmp(argv[2],v->opt)) {
      op = v->val;
      cur = atoi(argv[3]);
      // Remove options
      argc -= 3;
      for (int i=1; i<argc; i++)
        argv[i] = argv[i+3];
      return true;
    }
  std::cerr << "Wrong argument \"" << argv[2] 
            << "\" for option \"" << opt << "\"" 
            << std::endl;
  exit(EXIT_FAILURE);
}

void 
StringUIntOption::help(void) {
  if (fst == NULL)
    return;
  std::cerr << '\t' << opt << " (";
  const char* d = NULL;
  for (Value* v = fst; v != NULL; v = v->next) {
    std::cerr << v->opt << ((v->next != NULL) ? ", " : "");
    if (v->val == op)
      d = v->opt;
  }
  std::cerr << ") (unsigned int value)";
  if (d != NULL) 
    std::cerr << " default: " << d;
  std::cerr << std::endl << "\t\t" << exp << std::endl;
  for (Value* v = fst; v != NULL; v = v->next)
    if (v->help != NULL)
      std::cerr << "\t\t  " << v->opt << ": " << v->help << std::endl;
} 

StringUIntOption::~StringUIntOption(void) {
  Value* v = fst;
  while (v != NULL) {
    Value* n = v->next;
    delete v;
    v = n;
  }
}


#endif
