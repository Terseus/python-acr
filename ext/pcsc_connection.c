#include "pcsc_connection.h"
#include "pcsc_error.h"
#include <string.h>

struct PcscConnection {
    // clang-format off
    PyObject_HEAD
    SCARDCONTEXT raw_context;
    PyObject *readers;
    Py_ssize_t readers_count;
    // clang-format on
};

PyObject *get_readers(PcscConnection *conn) {
    DWORD pcchReaders = SCARD_AUTOALLOCATE;
    LPSTR ptr = NULL;
    LONG err = SCardListReaders(conn->raw_context, NULL, (LPSTR)&ptr, &pcchReaders);

    if (err != SCARD_S_SUCCESS) {
        PCSC_ERROR("SCardListReaders", err);
        return NULL;
    }

    PyObject *pylist = PyList_New(0);
    if (pylist == NULL) {
        goto error_cleanup;
    }

    char *reader = ptr;
    while (*reader) {
        PyObject *item = PyUnicode_FromString(reader);
        if (item == NULL) {
            goto error_cleanup;
        }
        if (PyList_Append(pylist, item) != 0) {
            goto error_cleanup;
        }
        reader += strlen(reader) + 1;
    }

    err = SCardFreeMemory(conn->raw_context, ptr);
    ptr = NULL;
    if (err != SCARD_S_SUCCESS) {
        PCSC_ERROR("SCardFreeMemory", err);
        goto error_cleanup;
    }

    PyObject *pytuple = PyList_AsTuple(pylist);
    if (pytuple == NULL) {
        goto error_cleanup;
    }

    Py_DECREF(pylist);
    return pytuple;

error_cleanup:
    Py_XDECREF(pylist);
    if (ptr != NULL) {
        SCardFreeMemory(conn->raw_context, ptr);
    }
    return NULL;
}

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

    self->readers = get_readers(self);
    if (self->readers == NULL) {
        Py_DECREF(self);
        return NULL;
    }
    self->readers_count = PyTuple_Size(self->readers);

    return (PyObject *)self;
}

PyObject *PcscConnection_is_valid(PcscConnection *self, PyObject *Py_UNUSED(ignored)) {
    return PyBool_FromLong(SCardIsValidContext(self->raw_context) == SCARD_S_SUCCESS);
}

PyObject *PcscConnection_list_readers(PcscConnection *self, PyObject *Py_UNUSED(ignored)) {
    Py_INCREF(self->readers);
    return self->readers;
}

static PyMethodDef PcscConnection_methods[] = {
    {
        .ml_name = "is_valid",
        .ml_meth = (PyCFunction)PcscConnection_is_valid,
        .ml_flags = METH_NOARGS,
        .ml_doc = "Returns if this connection context is valid; it should always return True; used "
                  "for testing.",
    },
    {
        .ml_name = "list_readers",
        .ml_meth = (PyCFunction)PcscConnection_list_readers,
        .ml_flags = METH_NOARGS,
        .ml_doc = "Returns a list of currently available readers on the system.",
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
