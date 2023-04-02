#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>

static PyObject *_libacr_show_hello_world(PyObject *self, PyObject *args) {
    fprintf(stdout, "Hello world!\n");
    Py_RETURN_NONE;
}

static PyMethodDef _LibacrMethods[] = {
    {
        "show_hello_world",       /* Name of the method once exported */
        _libacr_show_hello_world, /* Method */
        METH_NOARGS,              /* Type of args, see
                                     https://docs.python.org/3/c-api/structures.html?highlight=meth_varargs#METH_VARARGS
                                   */
        "Print the string 'Hello world!\n' in stdio." /* docstring? */
    },
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef _LibacrModule = {
    PyModuleDef_HEAD_INIT, /* Always PyModuleDef_HEAD_INIT */
    "_libacr",             /* name */
    NULL,                  /* docstring */
    0, /* size for global state, -1 makes this module unsafe for use in
          sub-interpreters, see
          https://docs.python.org/3/c-api/module.html?highlight=pymoduledef#c.PyModuleDef.m_size
        */
    _LibacrMethods /* Methods exported */
};

PyMODINIT_FUNC PyInit__libacr(void) { return PyModule_Create(&_LibacrModule); }
