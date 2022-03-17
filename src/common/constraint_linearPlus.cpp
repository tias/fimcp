 /**
  * Tias Guns, <tias.guns@cs.kuleuven.be>
  *
  * FIM_CP specific propagators for Gecode
  */

#include "constraint_linearPlus.hh"

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
         IntConLevel icl) {
    // copy-paste from linear-bool.cpp
    if (a.size() != x.size())
      throw ArgumentSizeMismatch("Int::linear");
    if (home.failed()) return;

    int n=x.size();
    Region re(home);
    Linear::Term<BoolView>* t = re.alloc<Linear::Term<BoolView> >(n);
    for (int i=n; i--; ) {
      t[i].a=a[i]; t[i].x=x[i];
    }

    imply_linear2(home,b,t,n,r,c,icl);
  }


  /** \brief Post propagator for reify-implied linear constraint
   * private (part 2), uses custom RImpGqBoolInt
   * copy-paste from bool-post.cpp
   */
  void
  imply_linear2(Space& home, 
       BoolView b, Term<BoolView>* t, int n, IntRelType r, int c, 
       IntConLevel icl) {

    if (b.one())
      return post(home,t,n,r,c,icl);
    if (b.zero()) {
      // couldn't care less
      return;
    }

    Limits::check(c,"Int::linear");

    double d = c;

    /// Eliminate assigned views
    for (int i=n; i--; )
      if (t[i].x.one()) {
        d -= t[i].a; t[i]=t[--n];
      } else if (t[i].x.zero()) {
        t[i]=t[--n];
      }
    Limits::check(d,"Int::linear");

    /// Rewrite non-strict relations
    switch (r) {
    case IRT_EQ: case IRT_NQ: case IRT_LQ: case IRT_GQ:
        break;
    case IRT_LE:
        d--; r = IRT_LQ; break;
    case IRT_GR:
        d++; r = IRT_GQ; break;
    default:
        throw UnknownRelation("Int::linear");
    }


    c = static_cast<int>(d);
    
    Term<BoolView> *t_p, *t_n;
    int n_p, n_n;
    bool unit = normalize<BoolView>(t,n,t_p,n_p,t_n,n_n);

    if (n == 0) {
      bool fail = false;
      switch (r) {
      case IRT_EQ: fail = (0 != c); break;
      case IRT_NQ: fail = (0 == c); break;
      case IRT_GQ: fail = (0 < c); break;
      case IRT_LQ: fail = (0 > c); break;
      default: GECODE_NEVER;
      }
      if (fail)
        GECODE_ME_FAIL(b.zero_none(home));
      // else: couldn't care less
      return;
    }
    
    // Check for overflow
    {
      // XXX: in int-post.cc they do inverse: sl -= c and su -= c ??
      // in fact.. should it not do su += t_p[i].a ???
      double sl = c;
      double su = c;
      for (int i=n_p; i--; )
        su -= t_p[i].a;
      for (int i=n_n; i--; )
        sl += t_n[i].a;
      Limits::check(sl,"Int::linear");
      Limits::check(su,"Int::linear");
    }

    if (unit && (n_n == 0)) {
      /// All coefficients are 1
      switch (r) {
      case IRT_GQ:
        {
          ViewArray<BoolView> x(home,n_p);
          for (int i=n_p; i--; )
            x[i]=t_p[i].x;
          GECODE_ES_FAIL((RImpGqBoolInt<BoolView,BoolView>::post(home,b,x,c)));
        }
        break;
      case IRT_LQ:
        {
          ViewArray<NegBoolView> x(home,n_p);
          for (int i=n_p; i--; )
            x[i]=NegBoolView(t_p[i].x);
          GECODE_ES_FAIL((RImpGqBoolInt<NegBoolView,BoolView>::post(home,b,x,n_p-c)));
        }
        break;
      default:
        throw Exception("constraint_linearPlus","wrong relation specified");
      }
    } else {
      throw Exception("constraint_linearPlus","non unary constraint posted");
    }
  }

  template <class VX, class VB>
  ExecStatus
  RImpGqBoolInt<VX,VB>::post(Space& home, VB b, ViewArray<VX>& x, int c) {
    assert(!b.assigned()); // checked before posting
        
    // Eliminate assigned views
    int n_x = x.size();
    for (int i=n_x; i--; )
      if (x[i].zero()) {
        x[i] = x[--n_x];
      } else if (x[i].one()) {
        x[i] = x[--n_x]; c--;
      }
    // RHS too large
    if (n_x < c) {
      GECODE_ME_CHECK(b.zero_none(home));
      return ES_OK;
    }
    // Whatever the x[i] take for values, the inequality is subsumed
    if (c <= 0) {
      // couldn't care less
      return ES_OK;
    }
    // This is the needed invariant as c subscriptions must be created (c+1 in non-reified)
    assert(n_x >= c);
    x.size(n_x);
    (void) new (home) RImpGqBoolInt<VX,VB>(home,b,x,c);
    return ES_OK;
  }

  /*
   * Reified greater or equal propagator (integer rhs)
   * 
   */
  template <class VX, class VB>
  forceinline
  RImpGqBoolInt<VX,VB>::RImpGqBoolInt(Space& home, VB b, ViewArray<VX>& x, int c)
    : ReLinBoolInt<VX,VB>(home,x,c,b) {}

  template <class VX, class VB>
  forceinline
  RImpGqBoolInt<VX,VB>::RImpGqBoolInt(Space& home, bool share, RImpGqBoolInt<VX,VB>& p)
    : ReLinBoolInt<VX,VB>(home,share,p) {}

  template <class VX, class VB>
  Actor*
  RImpGqBoolInt<VX,VB>::copy(Space& home, bool share) {
    return new (home) RImpGqBoolInt<VX,VB>(home,share,*this);
  }

  template<class VX, class VB>
  ExecStatus
  RImpGqBoolInt<VX,VB>::advise(Space&, Advisor&, const Delta& d) {
    if (VX::one(d))
      c--;
    n_s--;
    if ((n_s < c) || (c <= 0))
      return ES_NOFIX;
    else
      return ES_FIX;
  }

  template<class VX, class VB>
  ExecStatus
  RImpGqBoolInt<VX,VB>::propagate(Space& home, const ModEventDelta&) {
    if (b.none()) {
      if (n_s < c) {
        GECODE_ME_CHECK(b.zero_none(home));
      }
      // if (c <= 0): couldn't care less
    } else {
      if (b.one()) {
        GECODE_REWRITE(*this,(GqBoolInt<VX>::post(home(*this),x,c)));
      }
      // else: couldn't care less
    }
    return home.ES_SUBSUMED(*this);
  }

} // namespace
