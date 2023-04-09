#ifndef PCSC_CONNECTION_H
#define PCSC_CONNECTION_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <winscard.h>

typedef struct {
    // clang-format off
    PyObject_HEAD
    SCARDCONTEXT raw_context;
    // clang-format on
} PcscConnection;

extern PyTypeObject PcscConnectionType;

#endif // PCSC_CONNECTION_H
