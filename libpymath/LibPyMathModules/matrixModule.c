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

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

#ifdef _OPENMP
#include <omp.h>
#endif

static PyTypeObject MatrixCoreType;

// **************************************************************************************************************************** //
// ==================================================== Internal Functions ==================================================== //
// **************************************************************************************************************************** //

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

// ****************************************************************************************************************************** //
// ==================================================== Function Definitions ==================================================== //
// ****************************************************************************************************************************** //

static double *allocateMemory(long int length) {
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

// ********************************************************************************************************************** //
// ==================================================== Matrix Class ==================================================== //
// ********************************************************************************************************************** //

typedef struct {
    PyObject_HEAD

    long int rows;
    long int cols;
    long int rowStride;
    long int colStride;
    double *data;
} MatrixCoreObject;

static void matrixDealloc(MatrixCoreObject *self) {
    free(self->data);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *matrixNew(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    MatrixCoreObject *self;
    self = (MatrixCoreObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->rows = 0;
        self->cols = 0;
        self->rowStride = 0;
        self->colStride = 0;
        self->data = malloc(sizeof(double));

        if (self->data == NULL) {
            Py_DECREF(self);
            PyErr_SetString(PyExc_ValueError, "Out of memory");
            return NULL;
        }
    }

    return (PyObject *) self;
}

static int matrixInit(MatrixCoreObject *self, PyObject *args, PyObject *kwargs) {
    static char *kwlist[] = {"rows", "cols", NULL};
    long int r = -1;
    long int c = -1;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "l|l", kwlist, &r, &c))
        return -1;

    if (r == -1 && c == -1) {
        return -1;
    } else if (r != -1 && c == -1) {
        if (r <= 0)
            return -1;

        self->rows = r;
        self->cols = r;
        self->rowStride = c;
        self->colStride = 1;
        self->data = malloc(sizeof(double) * r * r);

        if (self->data == NULL) {
            PyErr_SetString(PyExc_MemoryError, "There was not enough memory to allocate an array of this size");
            return -1;
        }
    } else {
        if (r <= 0 || c <= 0)
            return -1;

        self->rows = r;
        self->cols = c;
        self->rowStride = c;
        self->colStride = 1;
        self->data = malloc(sizeof(double) * r * c);

        if (self->data == NULL) {
            PyErr_SetString(PyExc_MemoryError, "There was not enough memory to allocate an array of this size");
            return -1;
        }
    }

    return 0;
}

static PyObject *matrixGetVal(MatrixCoreObject *self, PyObject *index) {
    long int i;
    long int j;

    if (!PyArg_ParseTuple(index, "ll", &i, &j))
        return NULL;

    double res;

    if (i < self->rows && j < self->cols && i >= 0 && j >= 0) {
        res = self->data[internalGet(i, j, self->rowStride, self->colStride)];
    } else {
        PyErr_SetString(PyExc_IndexError, "Index out of range for matrix get");
        return NULL;
    }

    return Py_BuildValue("f", res);
}

static PyObject *matrixSetVal(MatrixCoreObject *self, PyObject *index) {
    long int i;
    long int j;
    double val;

    if (!PyArg_ParseTuple(index, "lld", &i, &j, &val))
        return NULL;

    if (i < self->rows && j < self->cols && i >= 0 && j >= 0) {
        self->data[internalGet(i, j, self->rowStride, self->colStride)] = val;
    } else {
        PyErr_SetString(PyExc_IndexError, "Index out of range for matrix set");
        return NULL;
    }

    Py_RETURN_NONE;
}

static MatrixCoreObject *matrixNewC(double *data, long int rows, long int cols, int t) {
    MatrixCoreObject *res;

    double *resData = allocateMemory(rows * cols);
    if (resData == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    memcpy(resData, data, sizeof(double) * rows * cols);

    if (PyType_Ready(&MatrixCoreType) < 0) {
        free(resData);
        return NULL;
    }

    res = PyObject_New(MatrixCoreObject, &MatrixCoreType);
    if (res == NULL) {
        free(resData);
        return NULL;
    }

    res->rows = rows;
    res->cols = cols;
    res->rowStride = (t == 0) ? cols : 1;
    res->colStride = (t == 0) ? 1 : cols;
    res->data = resData;

    return res;
}

static PyObject *matrixCopy(MatrixCoreObject *self) {
    double *res = allocateMemory(self->rows * self->cols);
    if (res == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory");
        return NULL;
    }

    memcpy(res, self->data, sizeof(double) * self->rows * self->cols);

    // TODO: Ensure this does the correct thing...
    return (PyObject *) matrixNewC(res, self->rows, self->cols, self->colStride != 1);
}

static PyMemberDef matrixMembers[] = {
        {NULL}
};

static PyObject *matrixToString(MatrixCoreObject *self, PyObject *Py_UNUSED(ignored)) {
    return PyUnicode_FromFormat("%s%i%s%i%s", "Matrix(rows = ", self->rows, ", cols = ", self->cols, ")");
}

static PyObject *matrixGetRows(MatrixCoreObject *self, void *closure) {
    return PyLong_FromLong(self->rows);
}

static PyObject *matrixGetCols(MatrixCoreObject *self, void *closure) {
    return PyLong_FromLong(self->cols);
}

static PyObject *matrixGetRowStride(MatrixCoreObject *self, void *closure) {
    return PyLong_FromLong(self->rowStride);
}

static PyObject *matrixGetColStride(MatrixCoreObject *self, void *closure) {
    return PyLong_FromLong(self->colStride);
}

static PyObject *matrixTransposeMagic(MatrixCoreObject *self) {
    long int tmp;
    tmp = self->rowStride;

    self->rowStride = self->colStride;
    self->colStride = tmp;

    tmp = self->rows;
    self->rows = self->cols;
    self->cols = tmp;

    Py_RETURN_NONE;
}

// ************************************************************************************************************************** //
// ==================================================== Matrix Functions ==================================================== //
// ************************************************************************************************************************** //

static PyObject *matrixFromData2D(MatrixCoreObject *self, PyObject *args) {
    PyObject *matrix;
    double *matrixData;
    long int rows = -1;
    long int cols = -1;

    if (!PyArg_ParseTuple(args, "Oll", &matrix, &rows, &cols))
        return NULL;

    if (rows < 0 || cols < 0)
        return NULL;

    matrixData = allocateMemory(rows * cols);

    if (!matrixData) {
        return NULL;
    }

    for (long int i = 0; i < rows; i++) {
        PyObject *row;
        row = PyList_GetItem(matrix, i);

        for (long int j = 0; j < cols; j++) {
            PyObject *element;
            element = PyList_GetItem(row, j);

            if (PyFloat_Check(element))
                matrixData[internalGet(i, j, cols, 1L)] = PyFloat_AsDouble(element);
            else if (PyLong_Check(element))
                matrixData[internalGet(i, j, cols, 1L)] = PyLong_AsDouble(element);
            else {
                PyErr_SetString(PyExc_TypeError, "Invalid type for matrix initialization. Must be int or float");
                return NULL;
            }
        }
    }

    return (PyObject *) matrixNewC(matrixData, rows, cols, 0);
}

static PyObject *matrixFromData1D(MatrixCoreObject *self, PyObject *args) {
    PyObject *matrix;
    double *matrixData;
    long int rows = -1;
    long int cols = -1;

    if (!PyArg_ParseTuple(args, "Oll", &matrix, &rows, &cols))
        return NULL;

    if (rows < 0 || cols < 0)
        return NULL;

    matrixData = allocateMemory(rows * cols);

    if (!matrixData) {
        return NULL;
    }

    for (long int i = 0; i < rows * cols; i++) {
        PyObject *element;
        element = PyList_GetItem(matrix, i);

        if (PyFloat_Check(element))
            matrixData[i] = PyFloat_AsDouble(element);
        else if (PyLong_Check(element))
            matrixData[i] = PyLong_AsDouble(element);
        else {
            PyErr_SetString(PyExc_TypeError, "Invalid type for matrix initialization. Must be int or float");
            return NULL;
        }
    }

    return (PyObject *) matrixNewC(matrixData, rows, cols, 0);
}

// **************************************************************************************************************************** //
// ==================================================== Module Definitions ==================================================== //
// **************************************************************************************************************************** //

static PyGetSetDef matrixGetSet[] = {
        {"rows",      (getter) matrixGetRows,      NULL, "Rows of matrix",          NULL},
        {"cols",      (getter) matrixGetCols,      NULL, "Columns of matrix",       NULL},
        {"rowStride", (getter) matrixGetRowStride, NULL, "Row stride of matrix",    NULL},
        {"colStride", (getter) matrixGetColStride, NULL, "Column stride of matrix", NULL},
        {NULL}
};

static PyMethodDef matrixMethods[] = {
        {"get",            (PyCFunction) matrixGetVal,         METH_VARARGS, "Set a value in the matrix"},
        {"set",            (PyCFunction) matrixSetVal,         METH_VARARGS, "Get a value in the matrix"},
        {"toString",       (PyCFunction) matrixToString,       METH_NOARGS,  "Give the matrix object as a string"},
        {"copy",           (PyCFunction) matrixCopy,           METH_NOARGS,  "Return an exact copy of a matrix"},
        {"transposeMagic", (PyCFunction) matrixTransposeMagic, METH_NOARGS,  "Transpose the matrix instantly by swapping the rows and colums and the row and column stride"},
        {NULL}
};

static PyMethodDef matrixFunctionMethods[] = {
        {"matrixFromData2D", (PyCFunction) matrixFromData2D, METH_VARARGS, "Create a new matrix from a 2D list of data"},
        {"matrixFromData1D", (PyCFunction) matrixFromData1D, METH_VARARGS, "Create a new matrix from a 1D list of data"},
        {NULL}
};

static PyTypeObject MatrixCoreType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "matrix.Matrix",
        .tp_doc = "Matrix module for Python",
        .tp_basicsize = sizeof(MatrixCoreObject),
        .tp_itemsize = 0,
        .tp_repr = (reprfunc) matrixToString,
        .tp_flags = Py_TPFLAGS_DEFAULT,
        .tp_new = matrixNew,
        .tp_init = (initproc) matrixInit,
        .tp_dealloc = (destructor) matrixDealloc,
        .tp_members = matrixMembers,
        .tp_getset = matrixGetSet,
        .tp_methods = matrixMethods,
};

static PyModuleDef matrixCoreModule = {
        PyModuleDef_HEAD_INIT,
        .m_name = "matrix",
        .m_doc = "Matrix module for Python",
        .m_size = -1,
        matrixFunctionMethods
};

PyMODINIT_FUNC
PyInit_matrix(void) {
    PyObject *m;
    if (PyType_Ready(&MatrixCoreType) < 0)
        return NULL;

    m = PyModule_Create(&matrixCoreModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&MatrixCoreType);
    if (PyModule_AddObject(m, "Matrix", (PyObject *) &MatrixCoreType) < 0) {
        Py_DECREF(&MatrixCoreType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
