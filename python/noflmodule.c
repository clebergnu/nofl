/*
 * This file is part of nofl
 * Copyright (C) 2009 Cleber Rodrigues <cleber.gnu@gmail.com>
 * All rights reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 * 
 * Author(s): Cleber Rodrigues <cleber.gnu@gmail.com>
 */

#include <Python.h>
#include "../src/init.h"
#include "certificate_obj.h"

/* 
 * Certificate type declared in certificate_obj.c
 */
extern PyTypeObject pynofl_Certificate_type;

/* 
 * Bindings of methods from init.[ch]
 */
static PyObject *pynofl_initialize(void) {

  return PyInt_FromLong(nofl_initialize());
}

static PyObject *pynofl_shutdown(void) {

  return PyInt_FromLong(nofl_shutdown());
}

static PyObject *pynofl_initialized(void) {

  return PyBool_FromLong(nofl_initialized());
}

static PyMethodDef NoflMethods[] = {

  {"initialize", 
   (PyCFunction) pynofl_initialize, 
   METH_NOARGS, 
   "Initialize nofl"},

  {"shutdown", 
   (PyCFunction) pynofl_shutdown, 
   METH_NOARGS, 
   "Shutdown nofl"},
  
  {"initialized", 
   (PyCFunction) pynofl_initialized, 
   METH_NOARGS, 
   "Check whether nofl is initialized"},

  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initnofl(void)
{
  PyObject *m;

  if (PyType_Ready(&pynofl_Certificate_type) < 0)
    return;

  m = Py_InitModule("nofl", NoflMethods);

  PyModule_AddObject(m, "Certificate", (PyObject *)&pynofl_Certificate_type);
  Py_INCREF(&pynofl_Certificate_type);

  return;
}

