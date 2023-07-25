lib_LTLIBRARIES = libqbackend.la

EXTRA_CPPFLAGS=-std=c++11 -pthread \
    -I. \
    $(json_c_CFLAGS) \
    $(libcurl_CFLAGS)
    


EXTRA_LIBS= $(json_c_LIBS) $(libcurl_LIBS) -lm -lrt

libqbackend_la_CPPFLAGS = $(CPPFLAGS) $(EXTRA_CPPFLAGS) -fPIC

libqbackend_la_LDFLAGS = $(LDFLAGS) -shared -fPIC -module -avoid-version -export-dynamic

libqbackend_la_LIBADD = $(LIBLTDL) $(EXTRA_LIBS)

libqbackend_la_SOURCES = \
    json/json_engine.cpp


pkgconfigdir = $(libdir)/pkgconfig




distclean-local:
	-rm -rf .deps */.deps
	-rm -f Makefile.in
	-rm -f Makefile