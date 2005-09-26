if BUILD_PYTHON
pyexec_LTLIBRARIES = cracklibmodule.la
cracklibmodule_la_LDFLAGS = -module -avoid-version
DEFS += '-DDEFAULT_CRACKLIB_DICT="$(pkgdatadir)/pw_dict"'
DEFS += '-DPYTHON_H="python@PYTHON_VERSION@/Python.h"'
endif

