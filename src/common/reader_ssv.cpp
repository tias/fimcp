/*
 *  Main authors:
 *      Siegfried Nijssen <siegfried.nijssen@cs.kuleuven.be>
 *      Tias Guns <tias.guns@cs.kuleuven.be>
 *
 *  Copyright:
 *      Tias Guns, 2009
 *
 *  Revision information:
 *      $Id: READERSSV.cpp 186 2009-01-30 13:00:16Z tias $
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
#ifndef __FIMCP_COMMON_READERSSV__
#define __FIMCP_COMMON_READERSSV__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * Class to read space separated values, like 0 1 12 5 8
 */
template <class Type>
class Reader_SSV {
  public:
    vector <vector<Type> > data;

    Reader_SSV () { }
    ~Reader_SSV () { }
    virtual void read ( string filename );

    inline bool is_space ( char pos ) {
        return pos == ' ' || pos == '\n' || pos == '\t' || pos == ',' || pos == '\r';
    }

    /// parse integer, c is first char, rest is in *in, space separated
    inline int parse_int(char &c, FILE *in);
};

template <class Type>
void Reader_SSV<Type>::read ( string filename ) {
    FILE *in = fopen(filename.c_str(),"rt");
    if (!in) {
        std::cerr << "\tError: file "<<filename<<" does not exist !"<<std::endl;
        exit(1);
    }

    do {
        char c = getc(in);
        if (c != '@' && c != '%' && c != ' ' && c != '\n' && !feof(in)) {
            // data: get elements
            vector<Type> temp;
            do {
                Type x = this->parse_int(c, in);
                temp.push_back(x);
            } while (c != '\n');
            data.push_back(temp);
        } else {
            // comment or whatever: skip this line
            while (c != '\n' && !feof(in))
                c = getc(in);
        }
    } while (!feof(in));
    fclose(in);

    /*
    for (unsigned int t=0; t!=data.size(); t++) {
        for (unsigned int i=0; i!=data[t].size(); i++) {
            int v = data[t][i];
            fprintf(stdout, "%i ", v);
        }
        fprintf(stdout, "\n");
    }
    */
}

template <class Type>
int Reader_SSV<Type>::parse_int(char &c, FILE *in) {
    int val = 0;
    int cval;
    do {
        cval = int(c)-int('0');
        if (cval < 0 || cval > 9) {
            fprintf(stderr, "\tError: unexpected input, non-digit '%c'=%i found !\n", c, cval);
            exit(1);
        }
        val *= 10;
        val += cval;
        c = getc(in);
    } while (!is_space(c));
    // skip all spaces except newlines
    while (c != '\n' && is_space(c))
        c = getc(in);
    return val;
}


/*
main () {
  Reader_SSV data;
  data.read("iris.txt"); 
}
*/

#endif
