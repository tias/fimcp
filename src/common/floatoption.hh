/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: FloatOption.hh 113 2008-06-27 10:31:27Z tias $
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
#ifndef __FIMCP_COMMON_FLOATOPTION_HH__
#define __FIMCP_COMMON_FLOATOPTION_HH__

#include <gecode/driver.hh>
using namespace Gecode::Driver;

/**
 * \brief float-valued option
 *
 */
class FloatOption : public BaseOption {
protected:
  float cur; ///< current value
public:
  /// Initialize for option \a o and explanation \a e and default value \a v
  FloatOption(const char* o, const char* e, float v);
  /// Set default value to \a v
  void value(float val);
  /// Return current option value
  float value(void) const;
  /// Parse option at first position
  virtual bool parse(int& argc, char* argv[]);
  /// Print help text
  virtual void help(void);
  /// Destructor
  ~FloatOption(void);
};

inline
FloatOption::FloatOption(const char* o, const char* e, 
                                     float v) 
  : BaseOption(o,e), cur(v) {}
inline void 
FloatOption::value(float v) {
  cur = v;
}
inline float 
FloatOption::value(void) const {
  return cur;
}

#endif
