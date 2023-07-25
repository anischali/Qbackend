bin_PROGRAMS = backend_test

EXTRA_CPPFLAGS=-std=c++11 -pthread \
    -I. \
    -I../src/audio \
    -I../src/curl \
    -I../src/json \
    -I../src/xml \
    $(nlohmann_json_CFLAGS) \
    $(libcurl_CFLAGS)    


EXTRA_LIBS= $(nlohmann_json_LIBS) $(libcurl_LIBS) -lm -lrt

backend_test_CPPFLAGS = $(CPPFLAGS) $(EXTRA_CPPFLAGS) -fPIC

backend_test_LDFLAGS = $(LDFLAGS)

backend_test_LDADD = $(LDLIB) $(EXTRA_LIBS) -lm -lrt ../src/.libs/libqbackend.la

backend_test_SOURCES = \
    backend_test.cpp


pkgconfigdir = $(libdir)/pkgconfig




distclean-local:
	-rm -rf .deps */.deps
	-rm -f Makefile.in
	-rm -f Makefile