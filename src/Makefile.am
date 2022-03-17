#
# Copyright (c) 2010 Tias Guns <tias.guns@cs.kuleuven.be>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#

AM_CPPFLAGS = -I${GECODE_INCLUDES}
LDADD = -L${GECODE_LIBRARIES} \
		-lgecodekernel -lgecodedriver \
		-lgecodesearch -lgecodeint \
		-lgecodesupport
if GECODE_WITH_GIST
LDADD += -lgecodegist
endif

COMMON = common/reader_ssv.cpp common/reader_eliz.cpp common/floatoption.cpp common/stringuintoption.cpp common/textoption.cpp
FIMCP = $(COMMON) common/options_fimcp.cpp common/fimcp_basic.cpp


bin_PROGRAMS = \
	fimcp_standard fimcp_closed fimcp_maximal \
	fimcp_size fimcp_cost fimcp_avgcost \
	fimcp_closed+cost fimcp_deltaclosed \
	fimcp_discriminating fimcp_discriminating+deltaclosed
AM_DEFAULT_SOURCE_EXT = .cpp

fimcp_standard_SOURCES = $(FIMCP) fimcp_standard.cpp
fimcp_closed_SOURCES = $(FIMCP) fimcp_closed.cpp
fimcp_maximal_SOURCES = $(FIMCP) fimcp_maximal.cpp
fimcp_size_SOURCES = $(FIMCP) fimcp_size.cpp
fimcp_cost_SOURCES = $(FIMCP) fimcp_cost.cpp
fimcp_avgcost_SOURCES = $(FIMCP) fimcp_avgcost.cpp
fimcp_closed_cost_SOURCES = $(FIMCP) fimcp_closed+cost.cpp
fimcp_deltaclosed_SOURCES = $(FIMCP) fimcp_deltaclosed.cpp
fimcp_discriminating_SOURCES = $(FIMCP) fimcp_discriminating.cpp
fimcp_discriminating_deltaclosed_SOURCES = $(FIMCP) fimcp_discriminating+deltaclosed.cpp

EXTRA_DIST = README RELEASE
