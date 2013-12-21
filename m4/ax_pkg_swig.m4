# ===========================================================================
#        http://www.gnu.org/software/autoconf-archive/ax_pkg_swig.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_PKG_SWIG([major.minor.micro], [action-if-found], [action-if-not-found])
#
# DESCRIPTION
#
#   This macro searches for a SWIG installation on your system. If found,
#   then SWIG is AC_SUBST'd; if not found, then $SWIG is empty.  If SWIG is
#   found, then SWIG_LIB is set to the SWIG library path, and AC_SUBST'd.
#
#   You can use the optional first argument to check if the version of the
#   available SWIG is greater than or equal to the value of the argument. It
#   should have the format: N[.N[.N]] (N is a number between 0 and 999. Only
#   the first N is mandatory.) If the version argument is given (e.g.
#   1.3.17), AX_PKG_SWIG checks that the swig package is this version number
#   or higher.
#
#   As usual, action-if-found is executed if SWIG is found, otherwise
#   action-if-not-found is executed.
#
#   In configure.in, use as:
#
#     AX_PKG_SWIG(1.3.17, [], [ AC_MSG_ERROR([SWIG is required to build..]) ])
#     AX_SWIG_ENABLE_CXX
#     AX_SWIG_MULTI_MODULE_SUPPORT
#     AX_SWIG_PYTHON
#
# LICENSE
#
