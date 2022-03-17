#
# Copyright (c) 2010 Tias Guns <tias@ulyssis.org>
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

AC_PREREQ([2.57])
AC_INIT(fimcp,[2.7],,, [http://dtai.cs.kuleuven.be/CP4IM/])
AM_INIT_AUTOMAKE([foreign dist-bzip2 subdir-objects])
AM_MAINTAINER_MODE

# From GECODE configure.ac
m4_include([gecode.m4])
AC_GECODE_GET_OS
if test "${CXX}x" = "x" -a "${CC}x" = "x" -a "${host_os}" = "windows"; then
  CC=cl
  CXX=cl
fi

AC_PROG_CC
AC_PROG_CXX



# C/P from Gecode's configure.ac
: ${CXXFLAGS=""} dnl we do not want autoconf's default
: ${CFLAGS=""}   dnl we do not want autoconf's default
: ${DLLFLAGS=""}   dnl we do not want autoconf's default
: ${GLDFLAGS=""}   dnl we do not want autoconf's default
dnl determine which compiler we are using
AC_CXX_COMPILER_VENDOR(ac_gecode_compiler_vendor)
case $ac_gecode_compiler_vendor in
gnu)
  AC_CHECK_GCC_VERSION(4,2)
  ;;
intel)
  ;;
microsoft)
  ;;
*)
  ;;
esac

case $ac_gecode_compiler_vendor in
gnu)
  dnl general compiler flags
  AC_GECODE_UNIX_PATHS
  AC_GECODE_GCC_GENERAL_SWITCHES
  dnl check whether we want to use visibility attributes with gcc
  AC_GECODE_GCC_VISIBILITY

  if test "${enable_debug:-no}" = "no" -a "${enable_gcov:-no}" = "no"; then
    dnl compiler flags for an optimized build
    AC_GECODE_GCC_OPTIMIZED_SWITCHES

    AC_GECODE_CHECK_COMPILERFLAG([-ffast-math])
    AC_GECODE_CHECK_COMPILERFLAG([-mthreads])
  else
    if test "${enable_debug:-no}" = "yes"; then
      dnl compiler flags for a debug build
      AC_GECODE_GCC_DEBUG_SWITCHES
    fi
  fi

  ;;
intel)
  dnl flags for creating dlls
  case $host_os in
     windows*)
       AC_GECODE_MSVC_SWITCHES
       ;;
     *)
       dnl check whether we want to use visibility attributes with gcc
       AC_GECODE_GCC_VISIBILITY
       dnl general compiler flags
       AC_GECODE_UNIX_PATHS
       AC_GECODE_GCC_GENERAL_SWITCHES

       if test "${enable_debug:-no}" = "no"; then
         dnl compiler flags for an optimized build
	 AC_GECODE_GCC_OPTIMIZED_SWITCHES
       else
         dnl compiler flags for a debug build
	 AC_GECODE_GCC_DEBUG_SWITCHES
       fi
       ;;
  esac
  ;;
microsoft)
  AC_GECODE_MSVC_SWITCHES
  ;;
*)
  AC_MSG_ERROR(Gecode currently only supports the GNU and Microsoft compilers.)
  ;;
esac
AC_SUBST(DLLFLAGS, ${DLLFLAGS})
AC_SUBST(GLDFLAGS, ${GLDFLAGS})

# Gecode gist support (the search tree visualiser)
AC_GECODE_QT
AC_GECODE_GIST
AM_CONDITIONAL([GECODE_WITH_GIST], [test "x$enable_gist" = xyes])


AC_MSG_CHECKING([whether to build the bundled Gecode framework])
AC_ARG_VAR([GECODE_INCLUDES], [DIR location of Gecode header files for inclusion (gecode will not be build if this variable is set)])
AC_ARG_VAR([GECODE_LIBRARIES], [DIR location of Gecode library files for linking (gecode will not be build if this variable is set)])
AS_IF([test "x$GECODE_INCLUDES" = x], [
    build_gecode="yes"
    GECODE_INCLUDES="../lib/gecode-3.7.1"
],[
    build_gecode="no"
    AS_IF([test "x$GECODE_LIBRARIES" = x], [GECODE_LIBRARIES="$GECODE_INCLUDES"])
])
AS_IF([test "x$GECODE_LIBRARIES" = x], [
    build_gecode="yes"
    GECODE_LIBRARIES="../lib/gecode-3.7.1"
],[
    AS_IF([test "x$build_gecode" = xyes], [GECODE_INCLUDES="$GECODE_LIBRARIES"])
    build_gecode="no"
])
AS_IF([test "x$build_gecode" = xyes], [
    AC_MSG_RESULT([yes])
],[
    AC_MSG_RESULT([no, includes at '$GECODE_INCLUDES' and libraries at '$GECODE_LIBRARIES'])
])
AM_CONDITIONAL([BUILD_GECODE], [test "x$build_gecode" = xyes])
AC_SUBST(GECODE_INCLUDES)
AC_SUBST(GECODE_LIBRARIES)


AC_SUBST(VERSION)

AC_OUTPUT([Makefile
           lib/Makefile
           src/Makefile
           src/more/Makefile])
