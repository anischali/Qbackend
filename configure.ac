AC_INIT([Qbackend], [0.0.1])
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([subdir-objects])

dnl Trouver le compilateur C approprie
CXXFLAGS="$CXXFLAGS -std=c++0x"
AC_PROG_CXX
AC_LANG([C++], [C])
AC_PROG_RANLIB
AM_PROG_AR
AC_HEADER_STDC

AC_ENABLE_SHARED
AC_DISABLE_STATIC
AC_PROG_LIBTOOL(libtool)

PKG_CHECK_MODULES([json_c], [json-c])
PKG_CHECK_MODULES([libcurl], [libcurl])
AC_CHECK_DECLS([curl_global_init], [], [], [#include <curl/curl.h>])
AC_CHECK_DECLS([curl_easy_init], [], [], [#include <curl/curl.h>])
AC_CHECK_DECLS([curl_easy_perform], [], [], [#include <curl/curl.h>])
AC_CHECK_DECLS([curl_easy_setopt], [], [], [#include <curl/curl.h>])
AC_CHECK_DECLS([curl_easy_cleanup], [], [], [#include <curl/curl.h>])
AC_CHECK_DECLS([curl_global_cleanup], [], [], [#include <curl/curl.h>])

PKG_CHECK_MODULES([libssl], [libssl])
AC_CHECK_DECLS([X509_STORE_new], [], [], [#include <openssl/x509.h>])
AC_CHECK_DECLS([PEM_SignInit], [], [], [#include <openssl/pem.h>])
AC_CHECK_DECLS([RSA_public_encrypt], [], [], [#include <openssl/rsa.h>])
AC_CHECK_DECLS([OPENSSL_init_ssl], [], [], [#include <openssl/ssl.h>])
AC_CHECK_DECLS([EVP_PKEY_new], [], [], [#include <openssl/evp.h>])


AC_CONFIG_HEADERS([config.h:config.h.in])

# Check whether exec_prefix=/usr:
AS_CASE([$exec_prefix:$prefix],
[NONE:NONE | NONE:/usr | /usr:*],
  [AC_MSG_NOTICE([Default --exec-prefix detected.])
   AS_CASE([$bindir],  ['${exec_prefix}/bin'],  [bindir=/bin;   AC_MSG_NOTICE([  --bindir defaults to /bin])  ])
   AS_CASE([$sbindir], ['${exec_prefix}/sbin'], [sbindir=/sbin; AC_MSG_NOTICE([  --sbindir defaults to /sbin])])
   AS_CASE([$libdir],  ['${exec_prefix}/lib'],  [libdir=/lib;   AC_MSG_NOTICE([  --libdir defaults to /lib])  ])
  ]
)




echo "	***	Configuring for ... $host	***"

AC_OUTPUT([Makefile] [src/Makefile])