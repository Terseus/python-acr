#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <winscard.h>

#include "pcsc_connection.h"
#include "pcsc_error.h"

PyObject *PcscError;

static PyMethodDef _LibacrMethods[] = {
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef _LibacrModule = {
    .m_base = PyModuleDef_HEAD_INIT, /* Always PyModuleDef_HEAD_INIT */
    .m_name = "_libacr",
    .m_doc = NULL,
    .m_size =
        -1,                      /* size for global state, -1 makes this module unsafe for use in
                                   sub-interpreters, see
                                   https://docs.python.org/3/c-api/module.html?highlight=pymoduledef#c.PyModuleDef.m_size
                                   */
    .m_methods = _LibacrMethods, /* Methods exported */
};

PyMODINIT_FUNC PyInit__libacr(void) {
    PyObject *module = NULL;

    if (PyType_Ready(&PcscConnectionType) < 0) {
        return NULL;
    }

    module = PyModule_Create(&_LibacrModule);
    if (module == NULL) {
        return NULL;
    }

    PcscError = PyErr_NewException("_libacr.PcscError", NULL, NULL);
    Py_XINCREF(PcscError);
    if (PyModule_AddObject(module, "PcscError", PcscError) < 0) {
        Py_XDECREF(PcscError);
        Py_CLEAR(PcscError);
        Py_DECREF(module);
        return NULL;
    }

    Py_INCREF(&PcscConnectionType);
    if (PyModule_AddObject(module, "PcscConnection", (PyObject *)&PcscConnectionType) < 0) {
        Py_DECREF(&PcscConnectionType);
        Py_DECREF(module);
        return NULL;
    }

    return module;
}
