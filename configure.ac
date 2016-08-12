AC_INIT([SurfAlarm], [0.1], [benjamin.fankhauser@hotmail.ch])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11

#AC_CHECK_LIB([png++], [png_create_read_struct])
#PKG_CHECK_MODULES([libpng12])
AC_MSG_CHECKING([Check for libpng12:])
AC_CHECK_LIB([png], [png_create_write_struct_2])

AC_MSG_CHECKING([Check for png++:])
AC_CHECK_HEADERS([png++/png.hpp])

AC_MSG_CHECKING([Check for curl:])
AC_CHECK_TOOL([CURL], [curl])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
