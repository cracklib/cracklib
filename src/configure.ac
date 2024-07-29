dnl Process this file with autoconf to produce a configure script.
AC_INIT([cracklib],[2.10.2],[cracklib-devel@lists.sourceforge.net])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

dnl Set of available languages.
ALL_LINGUAS="as be bn_IN ca cs da de el es eu fi fr gu he hi hu id it ja kk kn ko lt ml mr nb nl or pa pl pt pt_BR ru sk sl sq sv ta te tr uk zh_CN zh_TW"

dnl Checks for programs.
LT_INIT
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_CHECK_HEADERS([unistd.h fcntl.h inttypes.h stdint.h pthread.h endian.h])

AC_ARG_WITH([zlib],
   [AS_HELP_STRING([--without-zlib], [build without zlib support @<:@default=auto@:>@])])
AS_IF([test "$with_zlib" != "no"], [dnl
   found_zlib=yes
   AC_CHECK_HEADERS([zlib.h],
      [AC_DEFINE(HAVE_ZLIB_H, [], [found zlib])],
      [found_zlib=no])
   AC_SEARCH_LIBS([gzopen], [z], [], [found_zlib=no])
   AS_IF([test "$with_zlib$found_zlib" = "yesno"],
      [AC_MSG_ERROR([zlib not found])])
])

dnl Cygwin workaround
AC_MSG_CHECKING(if LINE_MAX is defined)
AC_EGREP_CPP(yes,
[#include <limits.h>
#ifdef LINE_MAX
yes
#endif
], line_max_defined=yes, line_max_defined=no)
AC_MSG_RESULT($line_max_defined)

if test "$line_max_defined" = "no"; then
   CFLAGS="$CFLAGS -DLINE_MAX=2048"
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Check for utility functions that may need to be replaced
AC_CHECK_FUNCS(strdup)
AC_CHECK_FUNCS(getpwuid_r)

dnl internationalization macros
AM_GNU_GETTEXT_VERSION([0.17])
AM_GNU_GETTEXT([external])

dnl Control default dictname
AC_MSG_CHECKING(default dict filename)
AC_ARG_WITH(default-dict,
AS_HELP_STRING([--with-default-dict],[Specify default dict filename]),
default_cracklib_dict=$withval,
default_cracklib_dict='$(pkgdatadir)/pw_dict')
AC_SUBST(DEFAULT_CRACKLIB_DICT, $default_cracklib_dict)
AC_MSG_RESULT($default_cracklib_dict)

dnl Check for python, unless we were told to not try to build a python module
AC_ARG_WITH(python,
AS_HELP_STRING([--without-python],[Build a python module @<:@default=auto@:>@]),
build_python=$withval,build_python=auto)
if test "$build_python" != no ; then
   AM_PATH_PYTHON(,,
                  [if test "$build_python" != yes ; then
                      AC_MSG_WARN([python was not found, continuing])
                      build_python=no
                   else
                      AC_MSG_ERROR([python was required but not found])
                   fi])
   if test "$build_python" != no ; then
      AC_CHECK_HEADERS(python${PYTHON_VERSION}/Python.h,,
		       [if test "$build_python" != yes ; then
		           AC_MSG_WARN([python headers not found, continuing])
			   build_python=no
			else
		           AC_MSG_ERROR([python headers not found])
			fi])
   fi
   if test "$build_python" != no ; then
      build_python=yes
   fi
fi
AM_CONDITIONAL(BUILD_PYTHON,[test "$build_python" = "yes"])

dnl Handle local dict compiling properly
AC_SUBST(CROSS_COMPILING, $cross_compiling)
AM_CONDITIONAL([NOT_CROSS_COMPILING], [test "$cross_compiling" = "no"])

AC_CONFIG_FILES([util/Makefile lib/Makefile doc/Makefile python/Makefile Makefile \
		python/setup.py \
		po/Makefile.in dicts/Makefile cracklib.spec])
AC_OUTPUT

