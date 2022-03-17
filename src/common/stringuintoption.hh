/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: textoption.hh 90 2008-04-28 17:08:59Z tias $
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
#ifndef __FIMCP_COMMON_STRINGUINTOPTION_HH__
#define __FIMCP_COMMON_STRINGUINTOPTION_HH__

#include <gecode/driver.hh>
using namespace Gecode::Driver;

/**
 * \brief Value-bound option
 * Consists of the ?operator and the limit
 *
 */
class StringUIntOption : public BaseOption {
protected:
  int op; // current operator
  unsigned int cur; ///< current value
  /// Option value
  class Value {
  public:
    int         val;  ///< Value for an option value
    const char* opt;  ///< String for option value
    const char* help; ///< Optional help text
    Value*      next; ///< Next option value
  };
  Value* fst; ///< First option value
  Value* lst; ///< Last option value
public:
  /// Initialize for option \a o and explanation \a e and default value \a v
  /// and default operator \a r
  StringUIntOption(const char* o, const char* e, int r, unsigned int v);
  /// Set default value to \a v
  void value(unsigned int v);
  /// Return current option value
  unsigned int value(void) const;
  /// Set default value to \a r
  void oper(int r);
  /// Return current operator value
  int oper(void) const;
  /// Add an option constant with text and help
  void add(int v, const char* o, const char* h);
  /// Parse option at first position
  virtual bool parse(int& argc, char* argv[]);
  /// Print help text
  virtual void help(void);
  /// Destructor
  virtual ~StringUIntOption(void);
};

inline
StringUIntOption::StringUIntOption(const char* o, const char* e, int r, unsigned int v) 
  : BaseOption(o,e), op(r), cur(v), fst(NULL), lst(NULL) {}

inline void 
StringUIntOption::value(unsigned int v) {
  cur = v;
}
inline unsigned int 
StringUIntOption::value(void) const {
  return cur;
}
inline void 
StringUIntOption::oper(int r) {
  op = r;
}
inline int 
StringUIntOption::oper(void) const {
  return op;
}

#endif
