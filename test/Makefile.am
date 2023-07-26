bin_PROGRAMS = backend_test

EXTRA_CPPFLAGS=-std=c++11 -pthread -g \
    -I. \
    -I./model \
    -I../src/audio \
    -I../src/curl \
    -I../src/json \
    -I../src/xml \
    $(nlohmann_json_CFLAGS) \
    $(libcurl_CFLAGS) \
    $(libfmt_CFLAGS)    


EXTRA_LIBS= $(nlohmann_json_LIBS) $(libcurl_LIBS) $(libfmt_LIBS) -lm -lrt

backend_test_CPPFLAGS = $(CPPFLAGS) $(EXTRA_CPPFLAGS) -fPIC

backend_test_LDFLAGS = $(LDFLAGS)

backend_test_LDADD = $(LDLIB) $(EXTRA_LIBS) -lm -lrt ../src/.libs/libqbackend.la

backend_test_SOURCES = \
    model/translation.hpp \
    model/translation.cpp \
    model/lang_translation.hpp \
    model/lang_translation.cpp \
    model/translations.hpp \
    model/translations.cpp \
    backend_test.cpp


pkgconfigdir = $(libdir)/pkgconfig




distclean-local:
	-rm -rf .deps */.deps
	-rm -f Makefile.in
	-rm -f Makefile