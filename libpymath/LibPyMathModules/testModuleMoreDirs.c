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

static PyObject *testSystem_moreDirs(PyObject *self, PyObject *args) {
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return PyLong_FromLong(sts);
}

static PyObject *testFunction_moreDirs(PyObject *self, PyObject *args, PyObject *kwargs) {
    static char *arguments[] = {
            "first",
            "middle",
            "last",
            "age",
            "numb",
            NULL
    };

    const char *first = "";
    const char *middle = "";
    const char *last = "";
    const unsigned int age = 0;
    const double numb = 0.;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|ssid", arguments, &first, &middle, &last, &age, &numb))
        return NULL;

    printf("Your name is %s %s %s and you are %i years old. Your favorite number is %f\n", first, middle, last, age, numb);

    Py_RETURN_NONE;
}

static PyMethodDef SpamMethods_moreDirs[] = {
        {"system", testSystem_moreDirs, METH_VARARGS,
                "Execute a shell command."},
        {"test", (PyCFunction) testFunction_moreDirs, METH_VARARGS | METH_KEYWORDS, "Runs a test function"},
        {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef testModule_moreDirs = {
        PyModuleDef_HEAD_INIT,
        "testModule",   /* name of module */
        NULL, /* module documentation, may be NULL */
        -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
        SpamMethods_moreDirs
};

PyMODINIT_FUNC
PyInit_testModule2(void) {
    return PyModule_Create(&testModule_moreDirs);
}
