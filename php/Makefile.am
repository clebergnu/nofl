py_libdir = $(PYTHON_SITE_PKG)
py_lib_LTLIBRARIES = noflmodule.la

noflmodule_la_SOURCES = noflmodule.c certificate_obj.c certificate_obj.h
noflmodule_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
noflmodule_la_LDFLAGS = -module
noflmodule_la_LIBADD = $(top_builddir)/src/libnofl.la

