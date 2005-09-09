if BUILD_PYTHON
pyexec_LTLIBRARIES = cracklibmodule.la
cracklibmodule_la_LDFLAGS = -module -avoid-version
INCLUDES = -I${PYTHON_PREFIX}/include/python${PYTHON_VERSION} -I/usr/include/python${PYTHON_VERSION}
DEFS += '-DDEFAULT_CRACKLIB_DICT="$(pkgdatadir)/pw_dict"'
endif

