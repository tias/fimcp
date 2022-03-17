/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: textoption.hh 231 2009-06-04 09:51:59Z tias $
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
#ifndef __FIMCP_COMMON_TEXTOPTION_HH__
#define __FIMCP_COMMON_TEXTOPTION_HH__

#include <gecode/driver.hh>
using namespace Gecode::Driver;

/**
 * \brief Text-valued option
 *
 */
class TextOption : public BaseOption {
protected:
  const char* cur; ///< current value
public:
  /// Initialize for option \a o and explanation \a e and default value \a v
  TextOption(const char* o, const char* e, const char* v);
  /// Set default value to \a v
  void value(const char* val);
  /// Return current option value
  const char* value(void) const;
  /// Parse option at first position
  virtual bool parse(int& argc, char* argv[]);
  /// Print help text
  virtual void help(void);
  /// Destructor
  virtual ~TextOption(void);
};

inline
TextOption::TextOption(const char* o, const char* e, const char* v) 
  : BaseOption(o,e), cur(v) {}

inline void 
TextOption::value(const char* v) {
  cur = v;
}
inline const char* 
TextOption::value(void) const {
  return cur;
}

#endif
