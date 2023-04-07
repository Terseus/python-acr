#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>

static PyObject *_libacr_show_hello_world(PyObject *self, PyObject *args) {
    fprintf(stdout, "Hello world!\n");
    Py_RETURN_NONE;
}

typedef struct {
    PyObject_HEAD
} PcscConnection;

static PyTypeObject PcscConnectionType = {
    // clang-format off
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_libacr.PcscConnection",
    // clang-format on
    .tp_doc = PyDoc_STR("Pcsc connection"),
    .tp_basicsize = sizeof(PcscConnection),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

static PyMethodDef _LibacrMethods[] = {
    {
        .ml_name = "show_hello_world",
        .ml_meth = _libacr_show_hello_world,
        .ml_flags =
            METH_NOARGS, /* see
                            https://docs.python.org/3/c-api/structures.html?highlight=meth_varargs#METH_VARARGS
                          */
        .ml_doc = "Print the string 'Hello world!\n' in stdio.",
    },
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef _LibacrModule = {
    .m_base = PyModuleDef_HEAD_INIT, /* Always PyModuleDef_HEAD_INIT */
    .m_name = "_libacr",
    .m_doc = NULL,
    .m_size =
        -1, /* size for global state, -1 makes this module unsafe for use in
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

    Py_INCREF(&PcscConnectionType);
    if (PyModule_AddObject(module, "PcscConnection",
                           (PyObject *)&PcscConnectionType) < 0) {
        Py_DECREF(&PcscConnectionType);
        Py_DECREF(module);
        return NULL;
    }

    return module;
}
