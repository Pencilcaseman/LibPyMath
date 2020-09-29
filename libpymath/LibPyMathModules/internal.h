/*
Copyright 2020 Toby Davis

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LIBPYMATHMODULES_INTERNAL_H
#define LIBPYMATHMODULES_INTERNAL_H

#include <Python.h>
#include <structmember.h>

#ifdef _OPENMP
#include <omp.h>
#endif

// Print to python stdout
void pythonPrint(const char *text) {
    PyObject *sysmod = PyImport_ImportModuleNoBlock("sys");
    PyObject *pystdout = PyObject_GetAttrString(sysmod, "stdout");
    PyObject *result = PyObject_CallMethod(pystdout, "write", "s", text);
    Py_XDECREF(result);
    Py_XDECREF(pystdout);
    Py_XDECREF(sysmod);
}

#define internalGet(i, j, r, c) ((j) * (c) + (i) * (r))

#ifndef _OPENMP
// A routine to give access to a high precision timer on most systems.
#if defined(_WIN32) || defined(__CYGWIN__)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

double seconds(void) {
    LARGE_INTEGER t;
    static double oofreq;
    static int checkedForHighResTimer;
    static BOOL hasHighResTimer;

    if (!checkedForHighResTimer) {
        hasHighResTimer = QueryPerformanceFrequency(&t);
        oofreq = 1.0 / (double) t.QuadPart;
        checkedForHighResTimer = 1;
    }

    if (hasHighResTimer) {
        QueryPerformanceCounter(&t);
        return (double) t.QuadPart * oofreq;
    } else {
        return (double) GetTickCount() * 1.0e-3;
    }
}

#elif defined(__linux__) || defined(__APPLE__)
#include <stddef.h>
#include <sys/time.h>
double seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}
#else
#error unsupported platform
#endif

#define TIME (seconds())
#else
#define TIME (omp_get_wtime())
#endif

// ****************************************************************************************************************************** //
// ==================================================== Function Definitions ==================================================== //
// ****************************************************************************************************************************** //

static double *allocateMemory(long long length) {
    double *res;

    if (length < 0) {
        PyErr_SetString(PyExc_ValueError, "Cannot allocate negative length");
        return NULL;
    }

    res = malloc(sizeof(double) * length);

    if (res == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    return res;
}

#endif //LIBPYMATHMODULES_INTERNAL_H
