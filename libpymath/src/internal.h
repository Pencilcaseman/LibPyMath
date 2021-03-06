#ifndef LIBPYMATHMODULES_INTERNAL_H
#define LIBPYMATHMODULES_INTERNAL_H

#include <Python.h>
#include <structmember.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#ifdef _OPENMP
#include <omp.h>
#endif

static int firstRand = 1;

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

double randomRange(double min, double max) {
    if (firstRand == 1) {
        srand((unsigned int) (TIME * 10000));
        firstRand--;
    }

    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

#endif //LIBPYMATHMODULES_INTERNAL_H
