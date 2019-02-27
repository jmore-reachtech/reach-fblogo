ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -D_GNU_SOURCE -I$(top_srcdir)/include

bin_PROGRAMS =
lib_LIBRARIES =
noinst_LIBRARIES =

fblogo_SOURCES = \
	include/fblogo.h

GLOBAL_HEADER = \
	include

EXTRA_DIST = $(GLOBAL_HEADER)

include src/Makemodule.am

-include Makefile.local
