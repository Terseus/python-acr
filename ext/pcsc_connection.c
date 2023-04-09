#include "pcsc_connection.h"
#include "pcsc_error.h"

void PcscConnection_dealloc(PcscConnection *self) {
    if (SCardIsValidContext(self->raw_context) != SCARD_S_SUCCESS) {
        goto type_free;
    }

    LONG err = SCardReleaseContext(self->raw_context);
    if (err != SCARD_S_SUCCESS) {
        PCSC_ERROR("SCardReleaseContext", err);
    }
type_free:
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *PcscConnection_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
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

PyObject *PcscConnetion_show_hello_world(PyObject *self, PyObject *Py_UNUSED(ignored)) {
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

PyTypeObject PcscConnectionType = {
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
