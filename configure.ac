#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([hhl4cpp], 0.1, n.ohkawa@gmail.com, [hhl4cpp])
AM_INIT_AUTOMAKE([-W -Wall foreign])
AC_CONFIG_SRCDIR([sample/sample.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for libraries.
m4_ifdef([AC_CHECK_CPPCUTTER], [AC_CHECK_CPPCUTTER], [ac_cv_use_cutter="no"])
AM_CONDITIONAL([HAVE_CUTTER], [test "$ac_cv_use_cutter" != "no"])
m4_ifdef([AC_CHECK_COVERAGE], [AC_CHECK_COVERAGE])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 sample/Makefile
                 test/Control/Makefile
                 test/Data/Makefile
                 test/Makefile])
AC_OUTPUT
