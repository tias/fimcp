/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: fimcp_basic.hh 325 2010-10-14 09:30:39Z tias $
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
#ifndef __FIMCP_COMMON_BASIC_HH__
#define __FIMCP_COMMON_BASIC_HH__

#include <cstdio>
#include <vector>
using namespace std;

#include <gecode/driver.hh>
using namespace Gecode::Driver;
#include "options_fimcp.cpp"

enum PrintStyle {
    PRINT_NONE,
    PRINT_FIMI,
    PRINT_FULL,
    PRINT_CPVARS
};

/**
 * %FIM-CP: Frequent Itemset Mining in CP, base class.
 *
 * Standard frequent itemset mining with off-the-shelve
 * Constraint Programming techniques.
 *
 * All models either:
 *  - only implement Fimcp_basic::run() {not listed in doxygen}
 *  - inherit Fimcp_basic and implement run()
 *
 */
class Fimcp_basic : public Script {
protected:
  /// print itemsets?
  PrintStyle print_itemsets;
  /// file to print to, if any
  FILE* solfile;

  /// Number of items
  int nr_i;
  /// Number of transactions
  int nr_t;
  /// Transaction Variables
  BoolVarArray transactions;
  /// Item Variables
  BoolVarArray items;

  // saves the class of every transaction (optional)
  vector<bool> classes;

public:
  /// Constructor for creation
  Fimcp_basic(const Options_fimcp&);

  /// Constructor for cloning \a s
  Fimcp_basic(bool, Fimcp_basic&);

  /// Perform copying during cloning
  virtual Space* copy(bool share) {
    return new Fimcp_basic(share,*this);
  }

  /// Do common construction stuff
  const vector< vector<bool> > common_construction(const Options_fimcp&);

  /// Get frequency to use
  int getFreq(const Options_fimcp&);

  /// Calculate sparseness of matrix
  float getSparseness(const vector< vector<bool> >&);
  
  /// Post coverage using the CLAUSE constraint
  void coverage_clause(const vector< vector<bool> >& tdb);

  virtual void run(const Options_fimcp&);

  /// Print a solution to \a os
  virtual void print(std::ostream&) const;
};

#endif
