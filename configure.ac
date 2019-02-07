AC_PREREQ([2.60])

AC_CONFIG_MACRO_DIRS([m4])

m4_define([RELEASE], 1.0.0)
AC_INIT([fblogo], [RELEASE], [jeff.horn@reachtech.com])

AC_CONFIG_SRCDIR([src/main.c])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_RANLIB

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug], [Enable debug])],
	[echo "Debugging Enabled: ${enableval}"],
	[echo "Debugging Disabled"])

if test "${enable_debug}" = "yes" ; then
    AC_DEFINE([DEBUG_ENABLED], [1], [Whether debugging is enabled])
fi

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
