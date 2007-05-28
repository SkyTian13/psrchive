#
# SWIN_LIB_PTHREAD([ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])
#
# This m4 macro checks availability of the GNU readline library
#
# PTHREAD_CFLAGS - autoconf variable with flags required for compiling
# PTHREAD_LIBS   - autoconf variable with flags required for linking
# HAVE_PTHREAD   - automake conditional
# HAVE_PTHREAD   - pre-processor macro in config.h
#
# This macro tries to link a test program using
#
#    -lreadline -ltermcap
#
#
#
# ----------------------------------------------------------
AC_DEFUN([SWIN_LIB_PTHREAD],
[
  AC_PROVIDE([SWIN_LIB_PTHREAD])

  AC_MSG_CHECKING([for posix threads library])

  PTHREAD_CFLAGS=""
  PTHREAD_LIBS="-lpthread"

  ac_save_CFLAGS="$CFLAGS"
  ac_save_LIBS="$LIBS"
  LIBS="$ac_save_LIBS $PTHREAD_LIBS"
  CFLAGS="$ac_save_CFLAGS $PTHREAD_CFLAGS"

  AC_RUN_IFELSE([AC_LANG_PROGRAM([[#include <pthread.h>
		void* test_thread (void* s) { pthread_exit(s); }]],
		[[pthread_t id;
		pthread_create (&id, 0, test_thread, 0);
		pthread_cancel (id);]])],
		have_pthread=yes, have_pthread=no)

  AC_MSG_RESULT($have_pthread)

  LIBS="$ac_save_LIBS"
  CFLAGS="$ac_save_CFLAGS"

  if test x"$have_pthread" = xyes; then
    AC_DEFINE([HAVE_PTHREAD], [1], [Define if POSIX threads are available])
    EXPORT_HAVE_PTHREAD=1
    [$1]
  else
    AC_MSG_WARN([POSIX threads will not be used])
    EXPORT_HAVE_PTRHEAD=0
    PTHREAD_CFLAGS=""
    PTHREAD_LIBS=""
    [$2]
  fi

  AC_SUBST(PTHREAD_CFLAGS)
  AC_SUBST(PTHREAD_LIBS)
  AC_SUBST(EXPORT_HAVE_PTHREAD)
  AM_CONDITIONAL(HAVE_PTHREAD, [test x"$have_pthread" = xyes])

])

