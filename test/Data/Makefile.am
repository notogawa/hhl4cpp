AM_CPPFLAGS=\
-I $(top_srcdir)/include \
-I $(includedir)

noinst_LTLIBRARIES=\
test.la

test_la_SOURCES=\
List.cpp \
Maybe.cpp

test_la_CXXFLAGS=\
@CPPCUTTER_CFLAGS@

test_la_LDFLAGS=\
-module -rpath $(libdir) -avoid-version -no-undefined

test_la_LIBADD=\
@CPPCUTTER_LIBS@

check:
	env LD_LIBRARY_PATH=/usr/local/lib/ cutter .
