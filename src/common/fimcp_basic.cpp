/*
 *  Main authors:
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2008
 *
 *  Revision information:
 *      $Id: fimcp_basic.cpp 305 2009-12-16 15:16:10Z tias $
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
#ifndef __FIMCP_COMMON_BASIC__
#define __FIMCP_COMMON_BASIC__
#define _FILE_OFFSET_BITS  64

#include "fimcp_basic.hh"
#include "textoption.hh"
#include "stringuintoption.hh"
#include "floatoption.hh"
#include "reader_eliz.cpp"

/// Constructor for creation
Fimcp_basic::Fimcp_basic(const Options_fimcp& opt) :
        print_itemsets(PRINT_NONE), solfile(stdout) {
    // all stuff implemented in ::run
    run(opt);
}

/// Model must be implemented in this function.
//void Fimcp_basic::run(const Options_fimcp& opt) {
//}
            
/// Constructor for cloning \a s
Fimcp_basic::Fimcp_basic(bool share, Fimcp_basic& s) :
    Script(share,s),
    print_itemsets(s.print_itemsets),
    solfile(s.solfile),
    nr_i(s.nr_i),
    nr_t(s.nr_t),
    classes(s.classes) {
    transactions.update(*this, share, s.transactions);
    items.update(*this, share, s.items);
}

/// Do common construction stuff
const vector< vector<bool> > Fimcp_basic::common_construction(const Options_fimcp& opt) {

    // Read data
    Reader_Eliz<bool> data;
    data.read(opt.datafile()); 

    const vector< vector<bool> >& tdb = data.data;
    classes = data.classes;
    nr_t = tdb.size();
    nr_i = tdb[0].size();

    // output stuff
    if (opt.output() == OUT_FIMI)
        print_itemsets = PRINT_FIMI;
    else if (opt.output() == OUT_FULL)
        print_itemsets = PRINT_FULL;
    else if (opt.output() == OUT_CPVARS)
        print_itemsets = PRINT_CPVARS;
#ifdef GECODE_HAS_GIST
    if (opt.mode() == SM_GIST)
        print_itemsets = PRINT_CPVARS;
#endif
    if (strcmp(opt.solfile(),"") != 0) { // solfile != ""
        fprintf(stdout, "writing solutions to file %s\n", opt.solfile());
        solfile = fopen(opt.solfile(),"w");
        if (solfile == NULL)
          throw Exception("Solution file", "Can not open solution file");
    }
    //fprintf(stdout, " dataset %s: %ix%i:%1.2f freq is %i\n", opt.datafile(), nr_t, nr_i, getSparseness(tdb), opt.getFreq(nr_t));
    fprintf(stdout, " dataset %s: %ix%i:%1.2f", opt.datafile(), nr_t, nr_i, getSparseness(tdb));
    if (classes.size() != 0) {
        int posTot = 0;
        for (int t = 0; t!=nr_t;t++ )
            posTot += classes[t];
        fprintf(stdout, " %i:+%i-%i", nr_t, posTot, (nr_t-posTot));
    }
    if (opt.getFreq(nr_t) != 0)
        fprintf(stdout, " minfreq %i\n", opt.getFreq(nr_t));
    else
        fprintf(stdout, "\n");

    // init vars
    transactions = BoolVarArray(*this, nr_t, 0, 1);
    items = BoolVarArray(*this, nr_i, 0, 1);

    return tdb;
}

/// Calculate sparseness of matrix
inline
float Fimcp_basic::getSparseness(const vector< vector<bool> >& tdb) {
    int sum = 0;
    for (int t=0; t!=nr_t; t++) {
        for (int i=0; i!=nr_i; i++)
            sum += tdb[t][i];
    }
    return ((float)sum/(nr_i*nr_t));
}

/// Post coverage using the CLAUSE constraint
void Fimcp_basic::coverage_clause(const vector< vector<bool> >& tdb) {
    BoolVarArgs none(0);
    for (int t=0; t!=nr_t; t++) {
        // count row
        int row_sum = nr_i;
        for (int i=0; i!=nr_i; i++)
            row_sum -= tdb[t][i];
        BoolVarArgs row(row_sum);
        // make row
        for (int i=0; i!=nr_i; i++) {
            if (1-tdb[t][i])
                row[--row_sum] = items[i];
        }

        // coverage: the trans its complement has no supported items
        // !row_compl[1] AND !row_compl[2] ...  <=> t_k
        clause(*this, BOT_AND, none, row, transactions[t]);
    }
}

/// Print solution
void Fimcp_basic::print(std::ostream& os) const {
    if (print_itemsets == PRINT_NONE) {
        return;
    } else if (print_itemsets == PRINT_FIMI) {
        // FIMI style output
        for (int i=0; i!=nr_i; i++) {
            if (items[i].val() == 1)
                fprintf(solfile, "%i ", i);
        }
        if (classes.size() == 0) {
            unsigned int supp = 0;
            for (int t=0; t!=nr_t; t++)
                supp += transactions[t].val();
            fprintf(solfile, "(%i)\n", supp);
        } else { // labels are used
            int pos = 0; int neg = 0;
            for (int t=0; t!=nr_t; t++) {
                if (classes[t])
                    pos += transactions[t].val();
                else
                    neg += transactions[t].val();
            }
            fprintf(solfile, "(%i:+%i-%i)\n", (pos+neg), pos, neg);
        }
    } else if (print_itemsets == PRINT_FULL) {
        // FULL output, items and transactions
        for (int i=0; i!=nr_i; i++) {
            if (items[i].val() == 1)
                fprintf(solfile, "%i ", i);
        }
        if (classes.size() == 0) {
            unsigned int supp = 0;
            for (int t=0; t!=nr_t; t++)
                supp += transactions[t].val();
            fprintf(solfile, "(%i) < ", supp);
            for (int t=0; t!=nr_t; t++)
                if (transactions[t].val() == 1)
                    fprintf(solfile, "%i ", t);
            fprintf(solfile, ">\n");
        } else { // labels are used
            int pos = 0; int neg = 0;
            for (int t=0; t!=nr_t; t++) {
                if (classes[t])
                    pos += transactions[t].val();
                else
                    neg += transactions[t].val();
            }
            fprintf(solfile, "(%i:+%i-%i) < ", (pos+neg), pos, neg);
            for (int t=0; t!=nr_t; t++)
                if (transactions[t].val() == 1)
                    fprintf(solfile, "%i ", t);
            fprintf(solfile, ">\n");
        }
    } else if (print_itemsets == PRINT_CPVARS) {
        // Output CP variables (mostly for GIST)
        os << "\tI[] = " << items << std::endl;
        os << "\tT[] = " << transactions << std::endl;
    }
}

#endif
