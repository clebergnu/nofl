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

#include "certificate_obj.h"

#include <descrobject.h>	/* python header */

#define CERTIFICATE_GET_SET_METHODS(v) \
static PyObject *pynofl_certificate_get_##v(PyObject *self, void *data) { \
  nofl_certificate *cert;						\
  cert = (((pynofl_Certificate *)(self))->cert); \
  return PyString_FromString(nofl_certificate_get_##v(cert)); \
} \
static int pynofl_certificate_set_##v(PyObject *self, PyObject *args, void *data) { \
  char *value; \
  nofl_certificate *cert; \
  cert = (((pynofl_Certificate *)(self))->cert); \
  if (!PyArg_ParseTuple(args, "s", &value)) { \
    return -1; \
  } \
  nofl_certificate_set_##v(cert, value); \
  return 0; \
}

/* 
 * Macro that generates a get method for the certificate obj
 */
#define CERTIFICATE_GET_METHOD(v) \
static PyObject *pynofl_certificate_get_##v(pynofl_Certificate *self) { \
  return PyString_FromString(nofl_certificate_get_##v(self->cert)); \
} 

/* 
 * Macro that generates a set method for the certificate obj
 */
#define CERTIFICATE_SET_METHOD(v) \
static PyObject *pynofl_certificate_set_##v(pynofl_Certificate *self, PyObject *args) { \
  char *value; \
  if (!PyArg_ParseTuple(args, "s", &value)) { \
    return NULL; \
  } \
  nofl_certificate_set_##v(self->cert, value); \
  Py_INCREF(Py_None); \
  return Py_None; \
}

/* 
 * Macro generated methods
 */
CERTIFICATE_GET_METHOD(name);
CERTIFICATE_GET_METHOD(version);

CERTIFICATE_GET_METHOD(customer_name);
CERTIFICATE_SET_METHOD(customer_name);

CERTIFICATE_GET_METHOD(customer_id);
CERTIFICATE_SET_METHOD(customer_id);

CERTIFICATE_GET_METHOD(product_name);
CERTIFICATE_SET_METHOD(product_name);

CERTIFICATE_GET_METHOD(product_version);
CERTIFICATE_SET_METHOD(product_version);

CERTIFICATE_GET_METHOD(product_min_version);
CERTIFICATE_SET_METHOD(product_min_version);

CERTIFICATE_GET_METHOD(product_max_version);
CERTIFICATE_SET_METHOD(product_max_version);

CERTIFICATE_GET_METHOD(initial_date);
CERTIFICATE_SET_METHOD(initial_date);

//CERTIFICATE_GET_METHOD(expire_date);
//CERTIFICATE_SET_METHOD(expire_date);

//CERTIFICATE_GET_METHOD(expire_grace);
//CERTIFICATE_SET_METHOD(expire_grace);

CERTIFICATE_GET_SET_METHODS(expire_date);

static PyGetSetDef pynofl_certificate_getsetdefs[] = {

  {"expire_date",
   pynofl_certificate_get_expire_date,
   pynofl_certificate_set_expire_date,
   "Expire Date"},
  
  {NULL}
};

/* 
 * Private 
 */
static PyObject *pynofl_certificate_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int pynofl_certificate_init(pynofl_Certificate *self, PyObject *args);
static PyObject *pynofl_certificate_load(pynofl_Certificate *self, PyObject *args);
static PyObject *pynofl_certificate_getattr(PyObject *self, char *attrname);
static void pynofl_certificate_dealloc(PyObject *self);

/* Custom methods */
static PyObject *pynofl_certificate_verify(pynofl_Certificate *self);

static PyMethodDef pynofl_certificate_methods[] = {

  {"get_name",
   (PyCFunction) pynofl_certificate_get_name,
   METH_NOARGS,
   "Get the descriptive name of this certificate"},

  {"get_version",
   (PyCFunction) pynofl_certificate_get_version,
   METH_NOARGS,
   "Get the version of this certificate"},

  {"get_customer_name",
   (PyCFunction) pynofl_certificate_get_customer_name,
   METH_NOARGS,
   "Get the customer name in this certificate"},

  {"set_customer_name",
   (PyCFunction) pynofl_certificate_set_customer_name,
   METH_VARARGS,
   "Set the customer name in this certificate"},

  {"get_customer_id",
   (PyCFunction) pynofl_certificate_get_customer_id,
   METH_NOARGS,
   "Get the customer id in this certificate"},

  {"set_customer_id",
   (PyCFunction) pynofl_certificate_set_customer_id,
   METH_VARARGS,
   "Set the customer id in this certificate"},

  {"get_product_name",
   (PyCFunction) pynofl_certificate_get_product_name,
   METH_NOARGS,
   "Get the product name in this certificate"},

  {"set_product_name",
   (PyCFunction) pynofl_certificate_set_product_name,
   METH_VARARGS,
   "Set the product name in this certificate"},

  {"get_product_version",
   (PyCFunction) pynofl_certificate_get_product_version,
   METH_NOARGS,
   "Get the product version in this certificate"},

  {"set_product_version",
   (PyCFunction) pynofl_certificate_set_product_version,
   METH_VARARGS,
   "Set the product version in this certificate"},

  {"get_product_min_version",
   (PyCFunction) pynofl_certificate_get_product_min_version,
   METH_NOARGS,
   "Get the product min version in this certificate"},

  {"set_product_min_version",
   (PyCFunction) pynofl_certificate_set_product_min_version,
   METH_VARARGS,
   "Set the product min version in this certificate"},

  {"get_product_max_version",
   (PyCFunction) pynofl_certificate_get_product_max_version,
   METH_NOARGS,
   "Get the product max version in this certificate"},

  {"set_product_max_version",
   (PyCFunction) pynofl_certificate_set_product_max_version,
   METH_NOARGS,
   "Set the product max version in this certificate"},

  {"get_initial_date",
   (PyCFunction) pynofl_certificate_get_initial_date,
   METH_NOARGS,
   "Get the initial date in this certificate"},

  {"set_initial_date",
   (PyCFunction) pynofl_certificate_set_initial_date,
   METH_VARARGS,
   "Set the initial date in this certificate"},

  /* {"get_expire_date", */
  /*  (PyCFunction) pynofl_certificate_get_expire_date, */
  /*  METH_NOARGS, */
  /*  "Get the expire date in this certificate"}, */

  /* {"set_expire_date", */
  /*  (PyCFunction) pynofl_certificate_set_expire_date, */
  /*  METH_VARARGS, */
  /*  "Set the expire date in this certificate"}, */


  /* {"get_expire_grace", */
  /*  (PyCFunction) pynofl_certificate_get_expire_date, */
  /*  METH_NOARGS, */
  /*  "Get the expire grace in this certificate"}, */

  /* {"set_expire_grace", */
  /*  //(PyCFunction) pynofl_certificate_set_expire_grace, */
  /*  (PyCFunction) pynofl_certificate_set_expire_grace, */
  /*  METH_VARARGS, */
  /*  "Set the expire grace in this certificate"}, */

  {"verify",
   (PyCFunction) pynofl_certificate_verify,
   METH_NOARGS,
   "Verify this certificate"},

  {"sign",
   (PyCFunction) pynofl_certificate_verify,
   METH_NOARGS,
   "Signs this certificate"},

  {"load",
   (PyCFunction) pynofl_certificate_load,
   METH_VARARGS,
   "Loads a certificate from a file"},

  {NULL}
};

/* static PyMemberDef pynofl_certificate_members[] = { */

/*   {"name", T_STRING, offsetof(pynofl_Certificate, name), RO, "name"}, */

/*   {NULL} */
/* }; */

PyTypeObject pynofl_Certificate_type = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /*ob_size*/
  "nofl.Certificate",                       /*tp_name*/     
  sizeof(pynofl_Certificate),  		    /*tp_basicsize*/
  0,                         		    /*tp_itemsize*/ 
  pynofl_certificate_dealloc,  		    /*tp_dealloc*/  
  0,                         	 	    /*tp_print*/    
  pynofl_certificate_getattr,               /*tp_getattr*/
  0,                                        /*tp_setattr*/	
  0,                         		    /*tp_compare*/	
  0,                                        /*tp_repr*/	
  0,                                        /*tp_as_number*/	
  0,                                        /*tp_as_sequence*/	
  0,                                        /*tp_as_mapping*/    
  0,                                        /*tp_hash */     
  0,                                        /*tp_call*/       
  0,                                        /*tp_str*/        
  0,                                        /*tp_getattro*/  
  0,                                        /*tp_setattro*/  
  0,                                        /*tp_as_buffer*/ 
  Py_TPFLAGS_DEFAULT,                       /*tp_flags*/ 
  "",					    /*tp_doc*/
  0,		                            /*tp_traverse*/
  0,		                            /*tp_clear*/
  0,		                            /*tp_richcompare*/
  0,		                            /*tp_weaklistoffset*/
  0,		                            /*tp_iter*/
  0,		                            /*tp_iternext*/
  pynofl_certificate_methods,               /*tp_methods*/
  0,                                        /*tp_members*/
  pynofl_certificate_getsetdefs,            /*tp_getset*/
  0,                                        /*tp_base*/
  0,                                        /*tp_dict*/
  0,                                        /*tp_descr_get*/
  0,                                        /*tp_descr_set*/
  0,                                        /*tp_dictoffset*/
  (initproc)pynofl_certificate_init,        /*tp_init*/
  0,                                        /*tp_alloc*/
  pynofl_certificate_new,                   /*tp_new*/
};

#define Certificate_Check(v)      ((v)->ob_type == &pynofl_Certificate)
#define Certificate_Cert(v)       (((pynofl_Certificate *)(v))->cert)
#define Certificate_Name(v)       (((pynofl_Certificate *)(v))->cert->name)
#define Certificate_Key_Path(v)   (((pynofl_Certificate *)(v))->cert->key_path)
#define Certificate_Ouput_Path(v) (((pynofl_Certificate *)(v))->cert->output_path)

static PyObject *pynofl_certificate_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  pynofl_Certificate *self;

  self = (pynofl_Certificate *)type->tp_alloc(type, 0);
  self->name = NULL;
  self->key_path = NULL;
  self->output_path = NULL;

  return (PyObject *) self;
}

static int pynofl_certificate_init(pynofl_Certificate *self, PyObject *args) {
  char *name;

  if (!PyArg_ParseTuple(args, "s", &name))
    return -1;

  self->cert = nofl_certificate_create(name);
  self->name = PyString_FromString(self->cert->name);
  self->key_path = PyString_FromString("");
  self->output_path = PyString_FromString("");

  return 0;
}

static PyObject *pynofl_certificate_load(pynofl_Certificate *self, PyObject *args) {
  char *path;

  if (!PyArg_ParseTuple(args, "s", &path))
    return NULL;

  self->cert = nofl_certificate_load(path);
  if (self->cert == NULL)
    return NULL;

  Py_INCREF(Py_None);
  return Py_None;
}

static void pynofl_certificate_dealloc(PyObject *self) {

  nofl_certificate_destroy(Certificate_Cert(self));

  Py_XDECREF(((pynofl_Certificate *)self)->name);
  Py_XDECREF(((pynofl_Certificate *)self)->key_path);
  Py_XDECREF(((pynofl_Certificate *)self)->output_path);

  self->ob_type->tp_free(self);
}

static PyObject *pynofl_certificate_verify(pynofl_Certificate *self) {

  nofl_certificate *cert;

  cert = self->cert;

  /* FIXME: implement certificate verification */
  return PyBool_FromLong(1);
}

static PyObject *pynofl_certificate_getattr(PyObject *self, char *attrname) {

  //if (!strcmp(attrname, "expire_date"))
  //  return PyDescr_NewGetSet(&pynofl_Certificate_type, &expire_date_getsetdef);

  return Py_FindMethod(pynofl_certificate_methods, self, attrname);
}

