if BUILD_PYTHON
pyexec_LTLIBRARIES = cracklibmodule.la
cracklibmodule_la_LDFLAGS = -module -avoid-version $(top_builddir)/lib/libcrack.la
DEFS += '-DDEFAULT_CRACKLIB_DICT="$(DEFAULT_CRACKLIB_DICT)"'
DEFS += '-DPYTHON_H="python@PYTHON_VERSION@/Python.h"'
endif

