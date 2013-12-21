# ===========================================================================
#      http://www.gnu.org/software/autoconf-archive/ax_swig_python.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_SWIG_PYTHON([use-shadow-classes = {no, yes}])
#
# DESCRIPTION
#
#   Checks for Python and provides the $(AX_SWIG_PYTHON_CPPFLAGS), and
#   $(AX_SWIG_PYTHON_OPT) output variables.
#
#   $(AX_SWIG_PYTHON_OPT) contains all necessary SWIG options to generate
#   code for Python. Shadow classes are enabled unless the value of the
#   optional first argument is exactly 'no'. If you need multi module
#   support (provided by the AX_SWIG_MULTI_MODULE_SUPPORT macro) use
#   $(AX_SWIG_PYTHON_LIBS) to link against the appropriate library. It
#   contains the SWIG Python runtime library that is needed by the type
#   check system for example.
#
# LICENSE
#
