#ifndef PCSC_ERROR_H
#define PCSC_ERROR_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define PCSC_ERROR(source, err)                                                                    \
    PyErr_Format(PcscError, "PCSC error from " #source ": %ld: %s", err, pcsc_stringify_error(err))

static PyObject *PcscError;

#endif // PCSC_ERROR_H
