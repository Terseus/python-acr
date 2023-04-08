#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <winscard.h>

#define PCSC_ERROR(source, err)                                                                    \
    PyErr_Format(PcscError, "PCSC error from " #source ": %ld: %s", err, pcsc_stringify_error(err))

typedef struct {
    // clang-format off
    PyObject_HEAD
    SCARDCONTEXT raw_context;
    // clang-format on
} PcscConnection;

static PyObject *PcscError;

void PcscConnection_dealloc(PcscConnection *self) {
    if (SCardIsValidContext(self->raw_context) == SCARD_S_SUCCESS) {
        goto type_free;
    }

    LONG err = SCardReleaseContext(self->raw_context);
    if (err != SCARD_S_SUCCESS) {
        PCSC_ERROR("SCardReleaseContext", err);
    }
type_free:
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *PcscConnection_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
    PcscConnection *self = NULL;
    self = (PcscConnection *)type->tp_alloc(type, 0);
    if (self == NULL) {
        return NULL;
    }

    self->raw_context = 0;
    LONG err = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &self->raw_context);
    if (err != SCARD_S_SUCCESS) {
        Py_DECREF(self);
        PCSC_ERROR("SCardEstablishContext", err);
        return NULL;
    }

    return (PyObject *)self;
}

static PyObject *PcscConnetion_show_hello_world(PyObject *self, PyObject *Py_UNUSED(ignored)) {
    fprintf(stdout, "Hello world!\n");
    Py_RETURN_NONE;
}

static PyMethodDef PcscConnection_methods[] = {
    {
        .ml_name = "show_hello_world",
        .ml_meth = (PyCFunction)PcscConnetion_show_hello_world,
        .ml_flags = METH_NOARGS,
        .ml_doc = "Print the string 'Hello world!\n' in stdio.",
    },
    {NULL}, /* Sentinel */
};

static PyTypeObject PcscConnectionType = {
    // clang-format off
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_libacr.PcscConnection",
    // clang-format on
    .tp_doc = PyDoc_STR("Pcsc connection"),
    .tp_basicsize = sizeof(PcscConnection),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PcscConnection_new,
    .tp_dealloc = (destructor)PcscConnection_dealloc,
    .tp_methods = PcscConnection_methods,
};

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
