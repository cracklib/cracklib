if BUILD_PYTHON
python_PYTHON = cracklib.py
pyexec_LTLIBRARIES = _cracklibmodule.la
AM_CFLAGS = -I$(top_builddir)/lib
_cracklibmodule_la_LDFLAGS = -module -avoid-version $(top_builddir)/lib/libcrack.la
DEFS += '-DDEFAULT_CRACKLIB_DICT="$(DEFAULT_CRACKLIB_DICT)"'
DEFS += '-DPYTHON_H="python@PYTHON_VERSION@/Python.h"'
endif

