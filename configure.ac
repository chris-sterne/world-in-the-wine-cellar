dnl Process this file with autoconf to produce a configure script.

AC_INIT([world-in-the-wine-cellar], 0.1.0, [chris_sterne@hotmail.com])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CXX

PKG_CHECK_MODULES(WORLD_IN_THE_WINE_CELLAR, [gtkmm-3.0 >= 3.24.0])

AC_OUTPUT([Makefile src/Makefile])
