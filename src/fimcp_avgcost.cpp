/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: fimcp_avgcost.cpp 286 2009-07-15 14:08:08Z tias $
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
 * Standard frequent itemset mining with average cost constraint (min or max).
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

    /** cost constraint **/
    {
        // attributes: get cost
        Reader_SSV<int>* data = new Reader_SSV<int>();
        data->read(opt.attrfile());
        const vector<int> cost = data->data[0];

        // make row_avgcost: sum(cost*col)/sum(col) > 10 <=> sum((cost-10)*col) > 0
        IntArgs row_avgcost(nr_i);
        for (int i=0; i!=nr_i; i++) {
            row_avgcost[i] = cost[i] - opt.bound1_val();
        }
        // average cost: the average cost of the itemset is within the bound
        // sum((cost-X)*col*itemz) >=< 0  [>=<:{>=,=<,=,!=,<,>}, X:integer]
        linear(*this, row_avgcost, items, (IntRelType)opt.bound1_op(), 0);
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
    opt.description("This model finds standard frequent patterns that satisfy the average cost constraint");
    opt.usage("-datafile example.txt -freq 0.10 -attrfile example.attr -bound1 GR 10\n \
              \tthe attrfile has a space separated number for every item: its cost (positive or negative)");
    opt.parse(argc, argv);

    Script::run<Fimcp_basic,DFS,Options_fimcp>(opt);
    return 0;
}
