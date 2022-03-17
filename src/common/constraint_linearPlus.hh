 /**
  * Tias Guns, <tias.guns@cs.kuleuven.be>
  *
  * FIM_CP specific propagators for Gecode
  */
#ifndef __FIMCP_CONSTRAINT_LINEARPLUS_HH__
#define __FIMCP_CONSTRAINT_LINEARPLUS_HH__

#include <gecode/kernel.hh>
#include <gecode/int.hh>
#include <gecode/int/linear.hh>

namespace constraint_linearPlus {
  using namespace ::Gecode;
  using namespace ::Gecode::Int;
  using namespace ::Gecode::Int::Linear;

  /** \brief Post propagator for reify-implied linear constraint
   *
   * Post a constraint of the form:
   * B -> (sum(a*X) r c)
   *   r must be either GQ, GE, LQ or LE
   *   a must be all unit: 0, 1 or -1
   */
  void imply_linear(Space& home,
         BoolVar b, const IntArgs& a, const BoolVarArgs& x, IntRelType r, int c,
         IntConLevel icl=ICL_DEF);

  /**
   * \brief %Propagator for reify-implied Boolean sum greater or equal to integer
   *
   * B -> (sum(V) >= c)
   * Requires \code #include "gecode/int/linear.hh" \endcode
   * \ingroup FuncIntProp
   */
  template <class VX, class VB>
  class RImpGqBoolInt : public ReLinBoolInt<VX,VB> {
    /// Propagator using less memory but with linear runtime
    protected:
      using ReLinBoolInt<VX,VB>::co;
      using ReLinBoolInt<VX,VB>::x;
      using ReLinBoolInt<VX,VB>::c;
      using ReLinBoolInt<VX,VB>::b;
      using ReLinBoolInt<VX,VB>::n_s;
      using ReLinBoolInt<VX,VB>::normalize;
      /// Constructor for cloning \a p
      RImpGqBoolInt(Space& home, bool share, RImpGqBoolInt& p);
      /// Constructor for creation
      RImpGqBoolInt(Space& home, VB b, ViewArray<VX>& x, int c);
    public:
      /// Create copy during cloning
      virtual Actor* copy(Space& home, bool share);
      /// Give advice to propagator
      virtual ExecStatus advise(Space& home, Advisor& a, const Delta& d);
      /// Perform propagation
      virtual ExecStatus propagate(Space& home, const ModEventDelta& med);

      /// Post propagator for \f$b -> \sum_{i=0}^{|x|-1}x_i \geq c\f$
      static ExecStatus post(Space& home, VB b, ViewArray<VX>& x, int c);
  };

  /** \brief Post propagator for reify-implied linear constraint
   * private (part 2), uses custom RImpGqBoolInt
   * copy-paste from bool-post.cpp
   */
  void
  imply_linear2(Space& home, 
       BoolView b, Term<BoolView>* t, int n, IntRelType r, int c, 
       IntConLevel icl);

} // namespace

#endif
