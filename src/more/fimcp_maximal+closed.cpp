/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: fimcp_maximal+closed.cpp 266 2009-07-08 14:58:05Z tias $
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

/**
 * Maximal frequent itemset mining.
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

    /** closed constraints **/
    {
        IntArgs col_(nr_t);
        for (int i=0; i!=nr_i; i++) {
            // make 1-col
            for (int t=0; t!=nr_t; t++)
                col_[t] = (1-tdb[t][i]);

            // closed: the item its complement has no supported trans
            // sum((1-col(item_i))*Trans) = 0 <=> item_i
            linear(*this, col_, transactions, IRT_EQ, 0, items[i]);
        }
    }

    /** frequency+maximal constraints **/
    {
        int freq = opt.getFreq(nr_t);
        IntArgs col(nr_t);
        for (int i=0; i!=nr_i; i++) {
            // make col
            for (int t=0; t!=nr_t; t++)
                col[t] = tdb[t][i];

            // maximal: the item is supported by sufficiently many trans, and if so then the item itself must be supported
            // sum(col(i_k)*Trans) >= Freq <=> i_k
            linear(*this, col, transactions, IRT_GQ, freq, items[i]);
        }
    }

    /** search **/
    branch(*this, items, (IntVarBranch)opt.branching(), (IntValBranch)opt.branchval());
}


int main(int argc, char* argv[]) {
    Options_fimcp opt(strpbrk(argv[0],"/\\")+1);
    opt.datafile("../data/example.txt");
    opt.freq(0.10);
    opt.description("This model finds maximal frequent patterns (no pattern has a superset that is frequent), it uses the redundant closed constraint");
    opt.usage("-datafile example.txt -freq 0.10");
    opt.parse(argc, argv);

    Script::run<Fimcp_basic,DFS,Options_fimcp>(opt);
    return 0;
}
