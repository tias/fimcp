/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: options_fimcp.cpp 305 2009-12-16 15:16:10Z tias $
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
#ifndef __FIMCP_COMMON_OPTIONS__
#define __FIMCP_COMMON_OPTIONS__

#include <cmath>
#include <vector>
using namespace std;
#include <gecode/driver.hh>
using namespace Gecode;
#include "textoption.hh"
#include "stringuintoption.hh"
#include "floatoption.hh"

enum OutputType {
    OUT_NONE,
    OUT_FIMI,
    OUT_FULL,
    OUT_CPVARS,
};

/**
 * \brief Options for examples with additional size parameter
 *
 */
class Options_fimcp : public Options {
protected:
    StringOption _branchval;
    StringOption _output;

    vector<BaseOption*> _list_specific;
    const char* _description;
    const char* _usage;

    // FIM_CP options
    TextOption _datafile;
    TextOption _attrfile;
    TextOption _solfile;
    UnsignedIntOption _cclause;
    FloatOption _freq;
    FloatOption _infreq;
    StringUIntOption _bound1;
    StringUIntOption _bound2;
    UnsignedIntOption _alpha;
    UnsignedIntOption _beta;
    FloatOption _delta;
    FloatOption _epsilon;
public:
  /// Initialize options for example with name \a e
  Options_fimcp(const char* e)
  : Options(e)
  , _branchval("-branchval", "branch value variants", INT_VAL_MAX)
  , _output("-output", "type of output of solutions", OUT_FIMI)
  , _list_specific()
  // defaults (must be add_specific() to be used)
  , _datafile("-datafile", "filename of dataset to use (any name)", "example.txt")
  , _attrfile("-attrfile", "filename of attributes to use (any name)", "")
  , _solfile("-solfile", "filename to write solutions to (any name)", "")
  , _cclause("-cclause", "coverage constraint using clause ?", 1)
  , _freq("-freq", "frequency (>1 is absolute, <1 is percentage, eg 0.10 is 10%)", 0.10)
  , _infreq("-infreq", "frequency (>1 is absolute, <1 is percentage, eg 0.10 is 10%)", 0.10)
  , _bound1("-bound1", "bound parameters: eg GQ 0", IRT_GQ, 0)
  , _bound2("-bound2", "bound parameters: eg GQ 0", IRT_GQ, 0)
  , _alpha("-alpha", "alpha parameter", 0)
  , _beta("-beta", "beta parameter", 0)
  , _delta("-delta", "delta parameter", 0.0)
  , _epsilon("-epsilon", "epsilon parameter", 0.0) {

  // set defaults
  solutions(0);
  iterations(10); // used with -mode time
  c_d(0); // with branchval MAX: less memory (sometimes faster)

  // general options
  _branching.add(INT_VAR_NONE, "input");
  _branching.add(INT_VAR_MIN_MIN, "min");
  _branching.add(INT_VAR_MIN_MAX, "minmax");
  _branching.add(INT_VAR_MAX_MIN, "maxmin");
  _branching.add(INT_VAR_MAX_MAX, "max");
  _branching.add(INT_VAR_SIZE_MIN, "size_min");
  _branching.add(INT_VAR_SIZE_MAX, "size_max");
  _branching.add(INT_VAR_DEGREE_MIN, "degree_min");
  _branching.add(INT_VAR_DEGREE_MAX, "degree_max");
  _branching.add(INT_VAR_SIZE_DEGREE_MIN, "size_degree_min");
  _branching.add(INT_VAR_SIZE_DEGREE_MAX, "size_degree_max");
  _branching.add(INT_VAR_REGRET_MIN_MIN, "regret_min");
  _branching.add(INT_VAR_REGRET_MIN_MAX, "regret_minmax");
  _branching.add(INT_VAR_REGRET_MAX_MIN, "regret_maxmin");
  _branching.add(INT_VAR_REGRET_MAX_MAX, "regret_max");
  branching(INT_VAR_DEGREE_MAX);

  _branchval.add(INT_VAL_MIN, "min");
  _branchval.add(INT_VAL_MAX, "max");
  _branchval.add(INT_VAL_MED, "med");
  add(_branchval);

  // FIM_CP options preparation
  _bound1.add(IRT_EQ, "EQ", "EQual"); ///< Equality (\f$=\f$)
  _bound1.add(IRT_NQ, "NQ", "Not Equal"); ///< Disequality (\f$\neq\f$)
  _bound1.add(IRT_LQ, "LQ", "Less or eQual"); ///< Less or equal (\f$\leq\f$)
  _bound1.add(IRT_LE, "LE", "LEss"); ///< Less (\f$<\f$)
  _bound1.add(IRT_GQ, "GQ", "Greater or eQual"); ///< Greater or equal (\f$\geq\f$)
  _bound1.add(IRT_GR, "GR", "GReater"); ///< Greater (\f$>\f$)
  _bound2.add(IRT_EQ, "EQ", "EQual"); ///< Equality (\f$=\f$)
  _bound2.add(IRT_NQ, "NQ", "Not Equal"); ///< Disequality (\f$\neq\f$)
  _bound2.add(IRT_LQ, "LQ", "Less or eQual"); ///< Less or equal (\f$\leq\f$)
  _bound2.add(IRT_LE, "LE", "LEss"); ///< Less (\f$<\f$)
  _bound2.add(IRT_GQ, "GQ", "Greater or eQual"); ///< Greater or equal (\f$\geq\f$)
  _bound2.add(IRT_GR, "GR", "GReater"); ///< Greater (\f$>\f$)

  _output.add(OUT_NONE, "none", "do not output solutions");
  _output.add(OUT_FIMI, "normal", "print solutions (FIMI-style)");
  _output.add(OUT_FULL, "full", "print solutions (FIMI-style) and transaction sets");
  _output.add(OUT_CPVARS, "cpvars", "print the CP variables of the solutions)");
  add_specific(_output);
  add_specific(_cclause);

  add_specific(_datafile);
  add_specific(_solfile);
}

  // add the option to our accessible vector
  // for help that uses friend in BaseOption, grrr
  inline void add_specific(BaseOption& o) {
    _list_specific.push_back(&o);
  }

  // branchval (considered part of gecode options)
  inline void branchval(int v) {
    _branchval.value(v);
  }
  inline int branchval(void) const {
    return _branchval.value();
  }

  // output
  inline void output(int v) {
    _output.value(v);
    //add_specific() this one is added in constructor
  }
  inline int output(void) const {
    return _output.value();
  }

  // FIM_CP options (added after mark)
  // datafile
  inline void datafile(const char* s) {
    _datafile.value(s);
    //add_specific() this one is added in constructor
  }
  inline const char* datafile(void) const {
    return _datafile.value();
  }

  // attrfile
  inline void attrfile(const char* s) {
    _attrfile.value(s);
    add_specific(_attrfile);
  }
  inline const char* attrfile(void) const {
    return _attrfile.value();
  }

  // solfile
  inline void solfile(const char* s) {
    _solfile.value(s);
    //add_specific() this one is added in constructor
  }
  inline const char* solfile(void) const {
    return _solfile.value();
  }

  // cclause
  inline void cclause(unsigned int v) {
    _cclause.value(v);
    //add_specific() this one is added in constructor
  }
  inline unsigned int cclause(void) const {
    return _cclause.value();
  }

  // freq
  inline void freq(float v) {
    _freq.value(v);
    add_specific(_freq);
  }
  inline unsigned int getFreq(unsigned int total) const {
    if (_freq.value() < 1) {
        // < 1 means relative value
        return (unsigned int)(floor((_freq.value()*total)+0.5)); // round(x)=floor(x+0.5)
    } else {
        return (unsigned int)_freq.value();
    }
  }
  // infreq
  inline void infreq(float v) {
    _infreq.value(v);
    add_specific(_infreq);
  }
  inline unsigned int getInfreq(unsigned int total) const {
    if (_infreq.value() < 1) {
        // < 1 means relative value
        return (unsigned int)(_infreq.value()*total);
    } else {
        return (unsigned int)_infreq.value();
    }
  }

  // bound1
  inline void bound1_val(unsigned int v) {
    _bound1.value(v);
    add_specific(_bound1);
  }
  inline unsigned int bound1_val(void) const {
    return _bound1.value();
  }
  inline void bound1_op(int r) {
    _bound1.oper(r);
  }
  inline int bound1_op(void) const {
    return _bound1.oper();
  }
  // bound2
  inline void bound2_val(unsigned int v) {
    _bound2.value(v);
    add_specific(_bound2);
  }
  inline unsigned int bound2_val(void) const {
    return _bound2.value();
  }
  inline void bound2_op(int r) {
    _bound2.oper(r);
  }
  inline int bound2_op(void) const {
    return _bound2.oper();
  }

  // alpha
  inline void alpha(unsigned int v) {
    _alpha.value(v);
    add_specific(_alpha);
  }
  inline unsigned int alpha(void) const {
    return _alpha.value();
  }
  // beta
  inline void beta(unsigned int v) {
    _beta.value(v);
    add_specific(_beta);
  }
  inline unsigned int beta(void) const {
    return _beta.value();
  }

  // delta
  inline void delta(float v) {
    _delta.value(v);
    add_specific(_delta);
  }
  inline float delta(void) const {
    return _delta.value();
  }
  // epsilon
  inline void epsilon(float v) {
    _epsilon.value(v);
    add_specific(_epsilon);
  }
  inline float epsilon(void) const {
    return _epsilon.value();
  }

  // description
  inline void description(const char* s) {
    _description = s;
  }

  // usage
  inline void usage(const char* s) {
    _usage = s;
  }


  /// Parse options from arguments \a argv (number is \a argc)
  void parse(int& argc, char* argv[]) {
    int x = _list_specific.size();

  // horrible code needs horrible goto (copy from original code)
  next:
    Options::parse(argc,argv);

    for (int i = 0; i != x; i++)
      if (_list_specific[i]->parse(argc,argv))
        goto next;
  }

  virtual void help(void) {
    Options::help();

    std::cerr << "Specific options for " << name() << ":" << std::endl;
    int x = _list_specific.size();
    for (int i = 0; i != x; i++)
      _list_specific[i]->help();
    
    std::cerr << "Description:" << std::endl
              << "    " << _description << std::endl;
    std::cerr << "Usage example:" << std::endl
              << "    " << "./" << name() << " " << _usage << std::endl;
  }
};

#endif
