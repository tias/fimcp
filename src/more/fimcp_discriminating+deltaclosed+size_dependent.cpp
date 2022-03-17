/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: fim_cp_closed.cc 96 2008-05-05 12:42:25Z tias $
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

#include "common/fimcp_basic.hh"

#define PRECISION 1000 // 3 numbers after comma

/**
 * Discriminating delta-closed frequent itemset mining with size constraints.:
 * frequent on one partition, infrequent on the other and delta-closed (over partition1)
 * constraints are modelled as being dependent! (only sensible if bound1 operator is GQ or GR and bound2 operator is LQ or LE)
 * inherits from fim_cp_basic.
 */
void Fimcp_basic::run(const Options_fimcp& opt) {
    const vector< vector<bool> > tdb = common_construction(opt);
    if (classes.size() == 0)
        throw Exception("Class label error", "no class labels found");

    /** covered constraints **/
    if (opt.cclause()) {
        // Default! Clause is a bit faster and uses less memory
        coverage_clause(tdb);
    } else {
        IntArgs row_(nr_i);
        for (int t=0; t!=nr_t; t++) {
            // make 1-row
            for (int i=0; i!=nr_i; i++)
                row_[i] = (1-tdb[t][i]);

            // coverage: the trans its complement has no supported items
            // sum((1-row(trans_t))*Items) = 0 <=> trans_t
            linear(*this, row_, items, IRT_EQ, 0, transactions[t]);
        }
    }

    // needed for freq and infreq constraints
    int posTot = 0;
    for (int t=0; t!=nr_t; t++) {
        posTot += classes[t];
        if (classes[t] != 1 && classes[t] != 0)
            throw Exception("Class label error",
                "illegal class label found, only '1' (pos) or '0' (neg) allowed");
    }

    /** frequency constraints, only on pos **/
    {
        int freq_pos = opt.getFreq(posTot);
        IntArgs col_pos(nr_t);
        for (int i=0; i!=nr_i; i++) {
            // make col_pos
            for (int t=0; t!=nr_t; t++)
                col_pos[t] = tdb[t][i]*classes[t];

            // freq: the item is supported by sufficiently many trans
            BoolVar aux(*this, 0, 1); // auxiliary variable
            // sum(col_pos(item_i)*Trans) >= Freq <=> aux
            linear(*this, col_pos, transactions, IRT_GQ, freq_pos, aux);
            // item_i => aux :: item_i =< aux
            rel(*this, items[i], IRT_LQ, aux);
        }
    }

    /** infrequency constraint, only on neg (no gain for reified) **/
    {
        int infreq_neg = opt.getInfreq(nr_t-posTot);

        IntArgs col_neg(nr_t);
        // make col_neg
        for (int t=0; t!=nr_t; t++)
            col_neg[t] = (1-classes[t]);

        // infreq: the item is supported by not more than a few trans
        linear(*this, col_neg, transactions, IRT_LQ, infreq_neg);
    }

    /** size constraint, bound 1 **/
    // size: the size of the itemset is larger than bound1
    // sum(items) OPERATOR bound1 (eg. >= 2), OPERATOR must be GQ or GR to be sensible
    linear(*this, items, (IntRelType)opt.bound1_op(), opt.bound1_val());

    /** delta-closed + size (bound 2) constraints: dependently modeled, delta & max_size <=> item **/
    {
        int hdelta = (int)(opt.delta()*PRECISION);
        IntArgs col_delta_pos(nr_t);
        BoolVar one(*this, 1, 1);
        for (int i=0; i!=nr_i; i++) {
            // delta-closed on pos: the item its complement in pos has only in a few supported trans
            // make col_delta_pos: (1-col_pos)*1000 - delta*1000 {precision with 3 numbers after comma}
            for (int t=0; t!=nr_t; t++) {
                col_delta_pos[t] = 0;
                if (classes[t] == 1)
                    col_delta_pos[t] = ((1-tdb[t][i])*PRECISION) - hdelta;
            }

            BoolVar aux_delta(*this, 0, 1);
            // sum( col_delta_pos*Trans ) =< 0 <=> aux_delta
            linear(*this, col_delta_pos, transactions, IRT_LQ, 0, aux_delta);

            BoolVar aux_maxsize(*this, 0, 1);
            // reified formulation of AM constraint: imagine this item were in (am[i]=one), what is then the result of this constraint
            // sum(items|i_k=1) OPERATOR bound2 <=> bi_maxsize_k (operator must be LQ or LE to be sensible)
            BoolVarArgs am(items); am[i] = one;
            linear(*this, am, (IntRelType)opt.bound2_op(), opt.bound2_val(), aux_maxsize);

            // aux_delta & aux_maxsize <=> item_i
            rel(*this, aux_delta, BOT_AND, aux_maxsize, items[i]);
        }
    }

    /** search **/
    branch(*this, items, (IntVarBranch)opt.branching(), (IntValBranch)opt.branchval());
}


int main(int argc, char* argv[]) {
    Options_fimcp opt(strpbrk(argv[0],"/\\")+1);
    opt.datafile("../data/example.txt");
    opt.freq(0.10);
    opt.infreq(0.10);
    opt.delta(0.80);
    opt.bound1_op(IRT_GQ);
    opt.bound1_val(0);
    opt.bound2_op(IRT_LQ);
    opt.bound2_val(999000);
    opt.description("This model finds discriminating delta-closed frequent patterns of a certain size (frequent on one parition, infrequent on the other and delta-closed on the first partition)\n \
                    \t constraints are modelled as dependent, this is only true for an anti-monotone cost constraint, eg cost =< 100");
    opt.usage("-datafile example.txt -attrfile example.attr -freq 0.10 -infreq 0.10 -delta 0.80 -bound1 GQ 2 -bound2 LQ 10\n \
              \tdelta has a precision of 3 numbers after the comma\n\
              \tthe datafile needs a class label for every transaction: 1=pos, 0=neg\n\
              \tbound1 its operator must be GE or GQ to be sensible\n \
              \tbound2 its operator must be LE or LQ to be sensible");
    opt.parse(argc, argv);

    Script::run<Fimcp_basic,DFS,Options_fimcp>(opt);
    return 0;
}
