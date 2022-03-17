/*
 *  Main authors:
 *      Siegfried Nijssen <siegfried.nijssen@cs.kuleuven.be>
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2009
 *
 *  Revision information:
 *      $Id: READERELIZ.cpp 186 2009-01-30 13:00:16Z tias $
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
#ifndef __FIMCP_COMMON_READERELIZ__
#define __FIMCP_COMMON_READERELIZ__

#include <iostream>
#include <vector>
#include <string>
#include "reader_ssv.cpp"

using namespace std;

/**
 * Class to read data in the format 0 2 3 6 8 1
 * first n-1 elements are items, last element represents the class (0 or 1)
 */
template <class Type>
class Reader_Eliz : public Reader_SSV<Type> {
  public:
    using Reader_SSV<Type>::data;
    vector<Type> classes;

    Reader_Eliz() : Reader_SSV<Type>() { }
    ~Reader_Eliz() { }
    virtual void read ( string filename );
};

template <class Type>
void Reader_Eliz<Type>::read ( string filename ) {
    FILE *in = fopen(filename.c_str(),"rt");
    if (!in) {
        fprintf(stderr, "\tError: file %s does not exist !\n", filename.c_str());
        exit(1);
    }
    // first pass, read data to know nr of items and nr of trans
    int nr_i = 0;
    int nr_t = 0;
    bool has_classes = true;
    do {
        char c = getc(in);
        if (feof(in) || c == '@' || c == '%' || c == ' ' || (c == '\n' && nr_t == 0)) {
            // comment or whatever: skip this line (only skip newlines before first datum)
            while (c != '\n' && !feof(in))
                c = getc(in);
        } else {
            // data: get items
            nr_t++;
            while (c != '\n') {
                int x = this->parse_int(c, in);
                if (x >= nr_i)
                    nr_i = x+1;
                if (c == '\n' && has_classes)
                    has_classes = (x <= 1); // last attribute can be class
            }
        }
    } while (!feof(in));

    //std::cerr << "Nr items is: "<<nr_i<<"\n";
    //std::cerr << "Nr trans is: "<<nr_t<<"\n";
    data.resize(nr_t, vector<Type>(nr_i,0));
    if (has_classes)
        classes.resize(nr_t, 0);

    // second pass, fill the vector
    rewind(in);
    int cur_t = 0;
    do {
        char c = getc(in);
        if (feof(in) || c == '@' || c == '%' || c == ' ' || (c == '\n' && cur_t == 0)) {
            // comment or whatever: skip this line (only skip newlines before first datum)
            while (c != '\n' && !feof(in))
                c = getc(in);
        } else {
            // data: get items
            while (c != '\n') {
                int x = this->parse_int(c, in);
                if (!(c == '\n' && has_classes)) {
                    // set item to 1
                    data[cur_t][x] = 1;
                } else {
                    // last one and class label (0 or 1)
                    classes[cur_t] = x;
                }
            }
            cur_t++;
        }
    } while (!feof(in));
    fclose(in);

    /*
    for (int t=0; t!=data.size(); t++) {
        for (int i=0; i!=data[t].size(); i++) {
            int v = data[t][i];
            fprintf(stdout, "%i ", v);
        }
        int v = classes[t];
        fprintf(stdout, "c:%i\n", v);
    }
    */
}

#endif
