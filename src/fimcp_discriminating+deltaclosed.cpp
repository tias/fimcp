/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: fimcp_discriminating+deltaclosed.cpp 286 2009-07-15 14:08:08Z tias $
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
 * Discriminating delta-closed frequent itemset mining:
 * frequent on one partition, infrequent on the other and delta-closed (over partition1).
 * implements Fimcp_basic::run.
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

    /** delta-closed constraints on pos **/
    {
        int hdelta = (int)(opt.delta()*PRECISION);
        IntArgs col_delta_pos(nr_t);
        for (int i=0; i!=nr_i; i++) {
            // make col_delta: (1-col)*1000 - delta*1000 {precision with 3 numbers after comma}
            for (int t=0; t!=nr_t; t++) {
                col_delta_pos[t] = 0;
                if (classes[t] == 1)
                    col_delta_pos[t] = ((1-tdb[t][i])*PRECISION) - hdelta;
            }

            // delta-closed: the item its complement has at most a few supported trans
            // sum( (100*(1-col(item_i))-100*delta)*Trans ) =< 0 <=> item_i
            linear(*this, col_delta_pos, transactions, IRT_LQ, 0, items[i]);
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

    /** search **/
    branch(*this, items, (IntVarBranch)opt.branching(), (IntValBranch)opt.branchval());
}


int main(int argc, char* argv[]) {
    Options_fimcp opt(strpbrk(argv[0],"/\\")+1);
    opt.datafile("../data/example.txt");
    opt.freq(0.10);
    opt.infreq(0.10);
    opt.delta(0.80);
    opt.description("This model finds discriminating delta-closed frequent patterns (frequent on pos, infrequent on neg and delta-closed on pos)");
    opt.usage("-datafile example.txt -freq 0.10 -infreq 0.10 -delta 0.80\n \
              \tdelta has a precision of 3 numbers after the comma\n\
              \tthe datafile needs a class label for every transaction: 1=pos, 0=neg");
    opt.parse(argc, argv);

    Script::run<Fimcp_basic,DFS,Options_fimcp>(opt);
    return 0;
}
