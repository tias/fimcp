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

AM_CPPFLAGS = -I${GECODE_INCLUDES} -I../
LDADD = -L${GECODE_LIBRARIES} \
		-lgecodekernel -lgecodedriver \
		-lgecodesearch -lgecodeint \
		-lgecodesupport
if GECODE_WITH_GIST
LDADD += -lgecodegist
endif

COMMON = ../common/reader_ssv.cpp ../common/reader_eliz.cpp ../common/floatoption.cpp ../common/stringuintoption.cpp ../common/textoption.cpp
FIMCP = $(COMMON) ../common/options_fimcp.cpp ../common/fimcp_basic.cpp


bin_PROGRAMS = \
	fimcp_emerging \
	fimcp_closed+cost_dependent \
	fimcp_discriminating+deltaclosed+size_independent \
	fimcp_discriminating+deltaclosed+size_dependent \
	fimcp_standardPlus fimcp_closedPlus \
	fimcp_maximal+closed \
	fimcp_standardNoreif fimcp_costReif
AM_DEFAULT_SOURCE_EXT = .cpp

fimcp_emerging_SOURCES = $(FIMCP) fimcp_emerging.cpp
fimcp_closed_cost_dependent_SOURCES = $(FIMCP) fimcp_closed+cost_dependent.cpp
fimcp_discriminating_deltaclosed_size_independent_SOURCES = $(FIMCP) fimcp_discriminating+deltaclosed+size_independent.cpp
fimcp_discriminating_deltaclosed_size_dependent_SOURCES = $(FIMCP) fimcp_discriminating+deltaclosed+size_dependent.cpp
fimcp_standardPlus_SOURCES = $(FIMCP) ../common/constraint_linearPlus.cpp fimcp_standardPlus.cpp
fimcp_closedPlus_SOURCES = $(FIMCP) ../common/constraint_linearPlus.cpp fimcp_closedPlus.cpp
fimcp_maximal_closed_SOURCES = $(FIMCP) fimcp_maximal+closed.cpp
fimcp_standardNoreif_SOURCES = $(FIMCP) fimcp_standardNoreif.cpp
fimcp_costReif_SOURCES = $(FIMCP) fimcp_costReif.cpp

EXTRA_DIST = README RELEASE
