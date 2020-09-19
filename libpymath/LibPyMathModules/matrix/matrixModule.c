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

#include <libpymath/LibPyMathModules/internalFunctions.h>
#include <libpymath/LibPyMathModules/BLAS/dgemm.h>
#include <libpymath/LibPyMathModules/matrix/doubleFunctions.h>

static PyTypeObject MatrixCoreType;

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

static PyObject *matrixNew(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
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
    long int r = -1;
    long int c = -1;

    if (!PyArg_ParseTuple(args, "ll", &r, &c))
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

    /*
    double *resData = allocateMemory(rows * cols);
    if (resData == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    memcpy(resData, data, sizeof(double) * rows * cols);
     */

    // if (PyType_Ready(&MatrixCoreType) < 0) {
    // free(resData);
    //     return NULL;
    // }

    res = PyObject_New(MatrixCoreObject, &MatrixCoreType);
    if (res == NULL) {
        // free(resData);
        return NULL;
    }

    if (!t) {
        res->rows = rows;
        res->cols = cols;
        res->rowStride = cols;
        res->colStride = 1;
        res->data = data;
    } else {
        res->rows = cols;
        res->cols = rows;
        res->rowStride = 1;
        res->colStride = cols;
        res->data = data;
    }

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

static PyObject *matrixTransposeReturn(MatrixCoreObject *self) {
    double *res = allocateMemory(self->rows * self->cols);
    long int rows, cols, i, j, rs, cs;
    rows = self->rows;
    cols = self->cols;
    rs = self->rowStride;
    cs = self->colStride;
    double *data = self->data;

#   pragma omp parallel for private(i, j) shared(rows, cols, rs, cs, data, res)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            res[i + j * rows] = data[internalGet(i, j, rs, cs)];
        }
    }

    return (PyObject *) matrixNewC(res, self->cols, self->rows, 0);
}

static PyObject *matrixProduct(MatrixCoreObject *self, PyObject *args) {
    MatrixCoreObject *other;
    double *resData;
    int threads = 1;

    if (!PyArg_ParseTuple(args, "O|i", &other, &threads)) {
        return NULL;
    }

    resData = allocateMemory(self->rows * other->cols);
    long int M = self->rows;
    long int N = self->cols;
    long int K = other->cols;
    double alpha = 1.0;
    double beta = 0.0;
    const double *a = self->data;
    const double *b = other->data;
    dgemm("T", "T", &M, &K, &N, &alpha, a, &N, b, &K, &beta, resData, &M);

    PyObject *res = (PyObject *) matrixNewC(resData, other->cols, self->rows, 1);

    return res;
}

static PyObject *matrixAddMatrixReturn(MatrixCoreObject *self, PyObject *args) {
    MatrixCoreObject *other;
    double *resData;
    int threads = 1;

    if (!PyArg_ParseTuple(args, "O|i", &other, &threads)) {
        return NULL;
    }

    resData = allocateMemory(self->rows * self->cols);
    doubleMatrixAddMatrix(self->data, other->data, resData, self->rows, self->cols, self->rowStride, self->colStride, other->rowStride, other->colStride, threads);

    PyObject *res = (PyObject *) matrixNewC(resData, self->rows, self->cols, self->colStride != 1);

    return res;
}

static PyObject *matrixSubMatrixReturn(MatrixCoreObject *self, PyObject *args) {
    MatrixCoreObject *other;
    double *resData;
    int threads = 8;

    if (!PyArg_ParseTuple(args, "O|i", &other, &threads)) {
        return NULL;
    }

    resData = allocateMemory(self->rows * self->cols);
    doubleMatrixSubMatrix(self->data, other->data, resData, self->rows, self->cols, self->rowStride, self->colStride, other->rowStride, other->colStride, threads);

    PyObject *res = (PyObject *) matrixNewC(resData, self->rows, self->cols, self->colStride != 1);

    return res;
}

static PyObject *matrixMulMatrixReturn(MatrixCoreObject *self, PyObject *args) {
    MatrixCoreObject *other;
    double *resData;
    int threads = 8;

    if (!PyArg_ParseTuple(args, "O|i", &other, &threads)) {
        return NULL;
    }

    resData = allocateMemory(self->rows * self->cols);
    doubleMatrixMulMatrix(self->data, other->data, resData, self->rows, self->cols, self->rowStride, self->colStride, other->rowStride, other->colStride, threads);

    PyObject *res = (PyObject *) matrixNewC(resData, self->rows, self->cols, self->colStride != 1);

    return res;
}

static PyObject *matrixDivMatrixReturn(MatrixCoreObject *self, PyObject *args) {
    MatrixCoreObject *other;
    double *resData;
    int threads = 8;

    if (!PyArg_ParseTuple(args, "O|i", &other, &threads)) {
        return NULL;
    }

    resData = allocateMemory(self->rows * self->cols);
    doubleMatrixDivMatrix(self->data, other->data, resData, self->rows, self->cols, self->rowStride, self->colStride, other->rowStride, other->colStride, threads);

    PyObject *res = (PyObject *) matrixNewC(resData, self->rows, self->cols, self->colStride != 1);

    return res;
}

static PyObject *matrixToList(MatrixCoreObject *self, PyObject *args) {
    PyObject *res = PyList_New(self->rows);
    if (res != NULL) {
        for (long int i = 0; i < self->rows; i++) {
            PyObject *row = PyList_New(self->cols);

            if (row == NULL) {
                return row;
            }

            for (long int j = 0; j < self->cols; j++) {
                PyList_SET_ITEM(row, j, Py_BuildValue("f", self->data[internalGet(i, j, self->rowStride, self->colStride)]));
            }

            PyList_SET_ITEM(res, i, row);
        }
    }

    return res;
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
            PyObject *element = PyList_GetItem(row, j);

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
        {"get",                   (PyCFunction) matrixGetVal,          METH_VARARGS, "Set a value in the matrix"},
        {"set",                   (PyCFunction) matrixSetVal,          METH_VARARGS, "Get a value in the matrix"},
        {"toString",              (PyCFunction) matrixToString,        METH_NOARGS,  "Give the matrix object as a string"},
        {"copy",                  (PyCFunction) matrixCopy,            METH_NOARGS,  "Return an exact copy of a matrix"},
        {"transposeMagic",        (PyCFunction) matrixTransposeMagic,  METH_NOARGS,  "Transpose the matrix instantly by swapping the rows and columns and the row and column stride"},
        {"transpose",             (PyCFunction) matrixTransposeReturn, METH_NOARGS,  "Transpose the matrix and return the result. This function actually swaps the data around"},
        {"matrixProduct",         (PyCFunction) matrixProduct,         METH_VARARGS, "Calculate the matrix product between two matrices and return the result"},
        {"matrixAddMatrixReturn", (PyCFunction) matrixAddMatrixReturn, METH_VARARGS, "Add one matrix to another and return the result"},
        {"matrixSubMatrixReturn", (PyCFunction) matrixSubMatrixReturn, METH_VARARGS, "Subtract one matrix from another and return the result"},
        {"matrixMulMatrixReturn", (PyCFunction) matrixMulMatrixReturn, METH_VARARGS, "Multiply one matrix by another and return the result"},
        {"matrixDivMatrixReturn", (PyCFunction) matrixDivMatrixReturn, METH_VARARGS, "Divide one matrix by another and return the result"},
        {"matrixToList",          (PyCFunction) matrixToList,          METH_NOARGS,  "Return the matrix represented as a 2D python list"},
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
