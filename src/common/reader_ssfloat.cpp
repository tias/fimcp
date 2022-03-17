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
#ifndef __FIMCP_COMMON_READERSSFLOAT__
#define __FIMCP_COMMON_READERSSFLOAT__

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

/**
 * Class to read space separated floats, like -1 0.3 NaN .555
 * shares a ridiculous large amount of code with reader_ssv
 */
template <class Type>
class Reader_SSFloat {
  public:
    vector <vector<Type> > data;

    Reader_SSFloat () { }
    ~Reader_SSFloat () { }
    virtual void read ( string filename );

    inline bool is_space ( char pos ) {
        return pos == ' ' || pos == '\n' || pos == '\t' || pos == ',' || pos == '\r';
    }

    /// parse integer, c is first char, rest is in *in, space separated
    inline float parse_float(char &c, FILE *in);
};

template <class Type>
void Reader_SSFloat<Type>::read ( string filename ) {
    FILE *in = fopen(filename.c_str(),"rt");
    if (!in) {
        std::cerr << "\tError: file "<<filename<<" does not exist !"<<std::endl;
        exit(1);
    }

    unsigned int maxsize = 0;
    do {
        char c = getc(in);
        if (c != '@' && c != '%' && c != ' ' && c != '\n' && !feof(in)) {
            // data: get elements
            vector<Type> temp;
            //temp.resize(maxsize);
            do {
                Type x = this->parse_float(c, in);
                temp.push_back(x);
            } while (c != '\n');
            data.push_back(temp);

            if (temp.size() > maxsize)
                maxsize = temp.size(); // init next vector size
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
            Type v = data[t][i];
            if (abs(v) <= 0.009)
                fprintf(stderr, "%E ", v);
            else
                fprintf(stderr, "%f ", v);
        }
        fprintf(stderr, "\n");
    }
    */
}

template <class Type>
float Reader_SSFloat<Type>::parse_float(char &c, FILE *in) {
    Type val = 0;
    bool negative = false;
    int fractionals = -1;
    int cval;
    do {
        if (c == '-')
            negative = true;
        else if (c == '.')
            fractionals = 0;
        else if (c == 'E') {
            // exponent
            c = getc(in);
            float exponent = parse_float(c, in);
            val *= pow(10.f, exponent);
            // we already ate the space char
            break;
        } else if (c == 'N') {
            // parse NaN
            c = getc(in);
            if (!is_space(c) && c == 'a') {
                c = getc(in);
                if (!is_space(c) && c == 'N') {
                    c = getc(in);
                    while (c != '\n' && is_space(c))
                        c = getc(in);
                    return NAN;
                }
            }
            fprintf(stderr, "\tError: unexpexed input, N followed by '%c', expected 'a'\n", c);
            exit(1);
        } else {
            cval = int(c)-int('0');
            if (cval < 0 || cval > 9) {
                fprintf(stderr, "\tError: unexpected input, non-digit '%c'=%i found !\n", c, cval);
                exit(1);
            }
            val *= 10;
            if (fractionals >= 0)
                fractionals++;
            val += cval;
        }
        c = getc(in);
    } while (!is_space(c));
    // skip all spaces except newlines
    while (c != '\n' && is_space(c))
        c = getc(in);

    if (negative)
        val *= -1;
    if (fractionals > 0)
        val /= pow(10.f, fractionals);
    return val;
}


/*
main () {
  Reader_SSFloat data;
  data.read("iris.txt"); 
}
*/

#endif
