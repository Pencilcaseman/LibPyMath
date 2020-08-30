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

static PyTypeObject MatrixCoreType;

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

        if (!self->data) {
            Py_DECREF(self);
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

        if (!self->data) {
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

        if (!self->data) {
            PyErr_SetString(PyExc_MemoryError, "There was not enough memory to allocate an array of this size");
            return -1;
        }
    }

    return 0;
}

static PyObject *matrixGetVal(MatrixCoreObject *self, PyObject *index) {
    long int i;

    if (!PyArg_ParseTuple(index, "l", &i))
        return NULL;

    double res;

    if (i < self->rows * self->cols && i >= 0) {
        res = self->data[i];
    } else {
        PyErr_SetString(PyExc_IndexError, "Index out of range for matrix get");
        return NULL;
    }

    return Py_BuildValue("f", res);
}

static PyObject *matrixSetVal(MatrixCoreObject *self, PyObject *index) {
    long int i;
    double val;

    if (!PyArg_ParseTuple(index, "ld", &i, &val))
        return NULL;

    if (i < self->rows * self->cols && i >= 0) {
        self->data[i] = val;
    } else {
        PyErr_SetString(PyExc_IndexError, "Index out of range for matrix set");
        return NULL;
    }

    Py_RETURN_NONE;
}

static double *allocateMemory(unsigned long length) {
    double *res;

    if (length < 0) {
        PyErr_NoMemory();
        return NULL;
    }

    res = malloc(sizeof(double) * length);

    if (!res) {
        PyErr_NoMemory();
        return NULL;
    }

    return res;
}

static MatrixCoreObject *matrixNewC(double *data, long int rows, long int cols) {
    MatrixCoreObject *res;

    double *resData = allocateMemory(rows * cols);
    if (!resData) {
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
    res->rowStride = 1;
    res->colStride = cols;
    res->data = resData;

    return res;
}

static PyObject *matrixFromData(MatrixCoreObject *self, PyObject *args) {
    PyObject *matrix;
    double *matrixData;
    long int rows;
    long int cols;

    if (!PyArg_ParseTuple(args, "Oii", &matrix, &rows, &cols))
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
                matrixData[j + i * cols] = PyFloat_AsDouble(element);
            else if (PyLong_Check(element))
                matrixData[j + i * cols] = PyLong_AsDouble(element);
            else {
                PyErr_SetString(PyExc_TypeError, "Invalid type for matrix initialization. Must be int or float");
                return NULL;
            }
        }
    }

    return (PyObject *) matrixNewC(matrixData, rows, cols);
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

static PyGetSetDef matrixGetSet[] = {
        {"rows",      (getter) matrixGetRows,      NULL, "Rows of matrix",          NULL},
        {"cols",      (getter) matrixGetCols,      NULL, "Columns of matrix",       NULL},
        {"rowStride", (getter) matrixGetRowStride, NULL, "Row stride of matrix",    NULL},
        {"colStride", (getter) matrixGetColStride, NULL, "Column stride of matrix", NULL},
        {NULL}
};

static PyMethodDef matrixMethods[] = {
        {"get",      (PyCFunction) matrixGetVal,   METH_VARARGS, "Set a value in the matrix"},
        {"set",      (PyCFunction) matrixSetVal,   METH_VARARGS, "Get a value in the matrix"},
        {"toString", (PyCFunction) matrixToString, METH_NOARGS,  "Give the matrix object as a string"},
        {NULL}
};

static PyMethodDef matrixFunctionMethods[] = {
        {"matrixFromData2D", (PyCFunction) matrixFromData, METH_VARARGS, "Create a new matrix from a 2D list of data"},
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