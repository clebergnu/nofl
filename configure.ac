dnl 
dnl This file is part of nofl, a Open Source subscription management system
dnl

m4_define(nss_require, 3.12)

dnl default output files


dnl Autoconf version requirement
AC_PREREQ([2.61])

dnl Mandatory AC_INIT line
AC_INIT([NOFL], 
	[0.0],
        [http://nofl.sourceforge.net],
	[nofl])

dnl Auxiliary build files - using the GNU naming convetion
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])

dnl Check for pkgconfig
PKG_PROG_PKG_CONFIG([0.21])

dnl
dnl Options for modular building
dnl
AC_ARG_WITH(python, [  --with-python           enable building Python modules],
	NOFL_PYTHON="$withval",
	NOFL_PYTHON="")

AC_DEFINE_UNQUOTED(NOFL_PYTHON, "$NOFL_PYTHON")

dnl if test "x$NOFL_PYTHON" != x; then
dnl 	AC_DEFINE(NOFL_PYTHON)
dnl fi

AC_ARG_WITH(php, [  --with-php              enable building PHP modules],
	NOFL_PHP="$withval",
	NOFL_PHP="")

AC_DEFINE_UNQUOTED(NOFL_PHP, "$NOFL_PHP")

if test "x$NOFL_PHP" != x; then
	AC_DEFINE(NOFL_PHP)
fi

dnl Check for libxml2
PKG_CHECK_MODULES(XMLSEC, xmlsec1-openssl >= 1.2.9 libxml-2.0,,exit)
CFLAGS="$CFLAGS $XMLSEC_CFLAGS"
CPPFLAGS="$CPPFLAGS $XMLSEC_CFLAGS"
LDFLAGS="$LDFLAGS $XMLSEC_LIBS"

dnl Debug support
AC_MSG_CHECKING(whether to enable debugging)
debug_default="yes"
AC_ARG_ENABLE(debug, [  --enable-debug=[no/yes] turn on debugging
                       [default=$debug_default]],, enable_debug=$debug_default)
if test "x$enable_debug" = "xyes"; then
         CFLAGS="$CFLAGS -g -DDEBUG"
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
fi

dnl Unique source file in source tree
AC_CONFIG_SRCDIR([src/init.c])

dnl Depend upon libtool
AC_PROG_LIBTOOL

dnl Require a C Compiler
AC_PROG_CC

dnl Automake support for object extension
AM_PROG_CC_C_O

dnl Almost all code includes <stdlib.h>
AC_CHECK_HEADERS([stdlib.h])

dnl Check for python using Autoconf Macro Archive's "ac_python_devel.m4"
if test "x$NOFL_PYTHON" != x; then
   AC_PYTHON_DEVEL([>= '2.4.0'])
fi

dnl Check support for C constants
AC_C_CONST

dnl Files to output
AC_CONFIG_FILES([Makefile src/Makefile])
if test "x$NOFL_PYTHON" != x; then
   AC_CONFIG_FILES([python/Makefile])
fi
AC_OUTPUT
