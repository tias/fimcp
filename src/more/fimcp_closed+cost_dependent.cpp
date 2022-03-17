/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: fim_cp_standard.cc 91 2008-04-28 17:22:57Z tias $
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
#include "common/reader_ssv.cpp"

/**
 * Closed frequent itemset mining with cost constraint,
 * constraints are modelled as being dependent! (only sensible if bound operator is LQ or LE).
 * implements Fimcp_basic::run.
 */
void Fimcp_basic::run(const Options_fimcp& opt) {
    const vector< vector<bool> > tdb = common_construction(opt);
    if (classes.size() != 0) {
        classes.resize(0);
        fprintf(stdout, " Warning: class labels found, ignored.\n");
    }

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

    /** frequency constraints **/
    {
        int freq = opt.getFreq(nr_t);
        IntArgs col(nr_t);
        for (int i=0; i!=nr_i; i++) {
            // make col
            for (int t=0; t!=nr_t; t++)
                col[t] = tdb[t][i];

            // freq: the item is supported by sufficiently many trans
            BoolVar aux(*this, 0, 1); // auxiliary variable
            // sum(col(item_i)*Trans) >= Freq <=> aux
            linear(*this, col, transactions, IRT_GQ, freq, aux);
            // item_i => aux :: item_i =< aux
            rel(*this, items[i], IRT_LQ, aux);
        }
    }

    /** closed + cost constraints (dependent) **/
    {
        // attributes: get cost
        Reader_SSV<int>* data = new Reader_SSV<int>();
        data->read(opt.attrfile());
        const vector<int> cost = data->data[0];

        // make row_cost
        IntArgs row_cost(nr_i);
        for (int i=0; i!=nr_i; i++) {
            row_cost[i] = cost[i];
        }

        BoolVar one(*this, 1, 1);
        IntArgs col_(nr_t);
        for (int i=0; i!=nr_i; i++) {
            // make 1-col
            for (int t=0; t!=nr_t; t++)
                col_[t] = (1-tdb[t][i]);

            // AM! and closed: the item its complement has no supported trans
            // sum(1-col(item_i)*Trans) = 0 <=> aux_closed
            BoolVar aux_closed(*this, 0, 1);
            linear(*this, col_, transactions, IRT_EQ, 0, aux_closed);

            // reified formulation of AM constraint: imagine this item were in (am[i]=one), what is then the result of this constraint
            // sum(cost*items|items_i=1) OPERATOR bound1 <=> aux_maxcost (OPERATOR must be LQ or LE to be sensible)
            BoolVar aux_maxcost(*this, 0, 1);
            BoolVarArgs am(items); am[i] = one;
            linear(*this, row_cost, am, (IntRelType)opt.bound1_op(), opt.bound1_val(), aux_maxcost);
            // aux_closed AND aux_maxcost <=> item_1
            rel(*this, aux_closed, BOT_AND, aux_maxcost, items[i]);
        }
    }


    /** search **/
    branch(*this, items, (IntVarBranch)opt.branching(), (IntValBranch)opt.branchval());
}


int main(int argc, char* argv[]) {
    Options_fimcp opt(strpbrk(argv[0],"/\\")+1);
    opt.datafile("../data/example.txt");
    opt.freq(0.10);
    opt.attrfile("../data/example.attr");
    opt.bound1_op(IRT_GQ);
    opt.bound1_val(0);
    opt.description("This model finds closed frequent patterns that satisfy the cost constraint\n \
                    \t constraints are modelled as dependent, this is only true for an anti-monotone cost constraint, eg cost =< 100");
    opt.usage("-datafile example.txt -freq 0.10 -attrfile example.attr -bound1 LE 500\n \
              \tthe attrfile has a space separated number for every item: its cost (positive or negative)\n \
              \tbound1 its operator must be LE or LQ to be sensible");
    opt.parse(argc, argv);

    Script::run<Fimcp_basic,DFS,Options_fimcp>(opt);
    return 0;
}
