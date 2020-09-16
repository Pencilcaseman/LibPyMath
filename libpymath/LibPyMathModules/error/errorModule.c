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

#include <libpymath/LibPyMathModules/internal.h>

// ************************************************************************************************************************* //
// ==================================================== Error Functions ==================================================== //
// ************************************************************************************************************************* //

static PyObject *errorRaiseArithmeticError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ArithmeticError, txt);
    return NULL;
}

static PyObject *errorRaiseAssertionError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_AssertionError, txt);
    return NULL;
}

static PyObject *errorRaiseAttributeError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_AttributeError, txt);
    return NULL;
}

static PyObject *errorRaiseBaseException(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_BaseException, txt);
    return NULL;
}

static PyObject *errorRaiseBlockingIOError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_BlockingIOError, txt);
    return NULL;
}

static PyObject *errorRaiseBrokenPipeError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_BrokenPipeError, txt);
    return NULL;
}

static PyObject *errorRaiseBufferError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_BufferError, txt);
    return NULL;
}

static PyObject *errorRaiseBytesWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_BytesWarning, txt);
    return NULL;
}

static PyObject *errorRaiseChildProcessError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ChildProcessError, txt);
    return NULL;
}

static PyObject *errorRaiseConnectionAbortedError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ConnectionAbortedError, txt);
    return NULL;
}

static PyObject *errorRaiseConnectionError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ConnectionError, txt);
    return NULL;
}

static PyObject *errorRaiseConnectionRefusedError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ConnectionRefusedError, txt);
    return NULL;
}

static PyObject *errorRaiseConnectionResetError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ConnectionResetError, txt);
    return NULL;
}

static PyObject *errorRaiseDeprecationWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_DeprecationWarning, txt);
    return NULL;
}

static PyObject *errorRaiseEOFError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_EOFError, txt);
    return NULL;
}

static PyObject *errorRaiseEnvironmentError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_EnvironmentError, txt);
    return NULL;
}

static PyObject *errorRaiseException(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_Exception, txt);
    return NULL;
}

static PyObject *errorRaiseFileExistsError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_FileExistsError, txt);
    return NULL;
}

static PyObject *errorRaiseFloatingPointError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_FloatingPointError, txt);
    return NULL;
}

static PyObject *errorRaiseFutureWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_FutureWarning, txt);
    return NULL;
}

static PyObject *errorRaiseGeneratorExit(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_GeneratorExit, txt);
    return NULL;
}

static PyObject *errorRaiseIOError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_IOError, txt);
    return NULL;
}

static PyObject *errorRaiseImportError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ImportError, txt);
    return NULL;
}

static PyObject *errorRaiseImportWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ImportWarning, txt);
    return NULL;
}

static PyObject *errorRaiseIndentationError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_IndentationError, txt);
    return NULL;
}

static PyObject *errorRaiseIndexError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_IndexError, txt);
    return NULL;
}

static PyObject *errorRaiseInterruptedError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_InterruptedError, txt);
    return NULL;
}

static PyObject *errorRaiseIsADirectoryError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_IsADirectoryError, txt);
    return NULL;
}

static PyObject *errorRaiseKeyError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_KeyError, txt);
    return NULL;
}

static PyObject *errorRaiseKeyboardInterrupt(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_KeyboardInterrupt, txt);
    return NULL;
}

static PyObject *errorRaiseLookupError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_LookupError, txt);
    return NULL;
}

static PyObject *errorRaiseMemoryError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_MemoryError, txt);
    return NULL;
}

static PyObject *errorRaiseNameError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_NameError, txt);
    return NULL;
}

static PyObject *errorRaiseNotADirectoryError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_NotADirectoryError, txt);
    return NULL;
}

static PyObject *errorRaiseNotImplementedError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_NotImplementedError, txt);
    return NULL;
}

static PyObject *errorRaiseOSError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_OSError, txt);
    return NULL;
}

static PyObject *errorRaiseOverflowError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_OverflowError, txt);
    return NULL;
}

static PyObject *errorRaisePendingDeprecationWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_PendingDeprecationWarning, txt);
    return NULL;
}

static PyObject *errorRaisePermissionError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_PermissionError, txt);
    return NULL;
}

static PyObject *errorRaiseProcessLookupError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ProcessLookupError, txt);
    return NULL;
}

static PyObject *errorRaiseRecursionError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_RecursionError, txt);
    return NULL;
}

static PyObject *errorRaiseReferenceError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ReferenceError, txt);
    return NULL;
}

static PyObject *errorRaiseResourceWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ResourceWarning, txt);
    return NULL;
}

static PyObject *errorRaiseRuntimeError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_RuntimeError, txt);
    return NULL;
}

static PyObject *errorRaiseRuntimeWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_RuntimeWarning, txt);
    return NULL;
}

static PyObject *errorRaiseStopAsyncIteration(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_StopAsyncIteration, txt);
    return NULL;
}

static PyObject *errorRaiseStopIteration(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_StopIteration, txt);
    return NULL;
}

static PyObject *errorRaiseSyntaxError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_SyntaxError, txt);
    return NULL;
}

static PyObject *errorRaiseSyntaxWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_SyntaxWarning, txt);
    return NULL;
}

static PyObject *errorRaiseSystemError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_SystemError, txt);
    return NULL;
}

static PyObject *errorRaiseSystemExit(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_SystemExit, txt);
    return NULL;
}

static PyObject *errorRaiseTabError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_TabError, txt);
    return NULL;
}

static PyObject *errorRaiseTimeoutError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_TimeoutError, txt);
    return NULL;
}

static PyObject *errorRaiseTypeError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_TypeError, txt);
    return NULL;
}

static PyObject *errorRaiseUnboundLocalError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_UnboundLocalError, txt);
    return NULL;
}

static PyObject *errorRaiseUnicodeDecodeError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_UnicodeDecodeError, txt);
    return NULL;
}

static PyObject *errorRaiseUnicodeEncodeError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_UnicodeEncodeError, txt);
    return NULL;
}

static PyObject *errorRaiseUnicodeError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_UnicodeError, txt);
    return NULL;
}

static PyObject *errorRaiseUnicodeTranslateError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_UnicodeTranslateError, txt);
    return NULL;
}

static PyObject *errorRaiseUnicodeWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_UnicodeWarning, txt);
    return NULL;
}

static PyObject *errorRaiseUserWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_UserWarning, txt);
    return NULL;
}

static PyObject *errorRaiseValueError(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_ValueError, txt);
    return NULL;
}

static PyObject *errorRaiseWarning(PyObject *self, PyObject *args) {
    const char *txt;
    if (!PyArg_ParseTuple(args, "s", &txt))
        return NULL;
    PyErr_SetString(PyExc_Warning, txt);
    return NULL;
}

// **************************************************************************************************************************** //
// ==================================================== Module Definitions ==================================================== //
// **************************************************************************************************************************** //

static PyMethodDef errorFunctionMethods[] = {
        {"raiseArithmeticError",           (PyCFunction) errorRaiseArithmeticError,           METH_VARARGS, "Raise an ArithmeticError"},
        {"raiseAssertionError",            (PyCFunction) errorRaiseAssertionError,            METH_VARARGS, "Raise an AssertionError"},
        {"raiseAttributeError",            (PyCFunction) errorRaiseAttributeError,            METH_VARARGS, "Raise an AttributeError"},
        {"raiseBaseException",             (PyCFunction) errorRaiseBaseException,             METH_VARARGS, "Raise a BaseException"},
        {"raiseBlockingIOError",           (PyCFunction) errorRaiseBlockingIOError,           METH_VARARGS, "Raise a BlockingIOError"},
        {"raiseBrokenPipeError",           (PyCFunction) errorRaiseBrokenPipeError,           METH_VARARGS, "Raise a BrokenPipeError"},
        {"raiseBufferError",               (PyCFunction) errorRaiseBufferError,               METH_VARARGS, "Raise a BufferError"},
        {"raiseBytesWarning",              (PyCFunction) errorRaiseBytesWarning,              METH_VARARGS, "Raise a BytesWarning"},
        {"raiseChildProcessError",         (PyCFunction) errorRaiseChildProcessError,         METH_VARARGS, "Raise a ChildProcessError"},
        {"raiseConnectionAbortedError",    (PyCFunction) errorRaiseConnectionAbortedError,    METH_VARARGS, "Raise a ConnectionAbortedError"},
        {"raiseConnectionError",           (PyCFunction) errorRaiseConnectionError,           METH_VARARGS, "Raise a ConnectionError"},
        {"raiseConnectionRefusedError",    (PyCFunction) errorRaiseConnectionRefusedError,    METH_VARARGS, "Raise a ConnectionRefusedError"},
        {"raiseConnectionResetError",      (PyCFunction) errorRaiseConnectionResetError,      METH_VARARGS, "Raise a ConnectionResetError"},
        {"raiseDeprecationWarning",        (PyCFunction) errorRaiseDeprecationWarning,        METH_VARARGS, "Raise a DeprecationWarning"},
        {"raiseEOFError",                  (PyCFunction) errorRaiseEOFError,                  METH_VARARGS, "Raise an EOFError"},
        {"raiseEnvironmentError",          (PyCFunction) errorRaiseEnvironmentError,          METH_VARARGS, "Raise an EnvironmentError"},
        {"raiseException",                 (PyCFunction) errorRaiseException,                 METH_VARARGS, "Raise an Exception"},
        {"raiseFileExistsError",           (PyCFunction) errorRaiseFileExistsError,           METH_VARARGS, "Raise a FileExistsError"},
        {"raiseFloatingPointError",        (PyCFunction) errorRaiseFloatingPointError,        METH_VARARGS, "Raise a FloatingPointError"},
        {"raiseFutureWarning",             (PyCFunction) errorRaiseFutureWarning,             METH_VARARGS, "Raise a FutureWarning"},
        {"raiseGeneratorExit",             (PyCFunction) errorRaiseGeneratorExit,             METH_VARARGS, "Raise a GeneratorExit"},
        {"raiseIOError",                   (PyCFunction) errorRaiseIOError,                   METH_VARARGS, "Raise an IOError"},
        {"raiseImportError",               (PyCFunction) errorRaiseImportError,               METH_VARARGS, "Raise an ImportError"},
        {"raiseImportWarning",             (PyCFunction) errorRaiseImportWarning,             METH_VARARGS, "Raise an ImportWarning"},
        {"raiseIndentationError",          (PyCFunction) errorRaiseIndentationError,          METH_VARARGS, "Raise an IndentationError"},
        {"raiseIndexError",                (PyCFunction) errorRaiseIndexError,                METH_VARARGS, "Raise an IndexError"},
        {"raiseInterruptedError",          (PyCFunction) errorRaiseInterruptedError,          METH_VARARGS, "Raise an InterruptedError"},
        {"raiseIsADirectoryError",         (PyCFunction) errorRaiseIsADirectoryError,         METH_VARARGS, "Raise an IsADirectoryError"},
        {"raiseKeyError",                  (PyCFunction) errorRaiseKeyError,                  METH_VARARGS, "Raise a KeyError"},
        {"raiseKeyboardInterrupt",         (PyCFunction) errorRaiseKeyboardInterrupt,         METH_VARARGS, "Raise a KeyboardInterrupt"},
        {"raiseLookupError",               (PyCFunction) errorRaiseLookupError,               METH_VARARGS, "Raise a LookupError"},
        {"raiseMemoryError",               (PyCFunction) errorRaiseMemoryError,               METH_VARARGS, "Raise a MemoryError"},
        {"raiseNameError",                 (PyCFunction) errorRaiseNameError,                 METH_VARARGS, "Raise a NameError"},
        {"raiseNotADirectoryError",        (PyCFunction) errorRaiseNotADirectoryError,        METH_VARARGS, "Raise a NotADirectoryError"},
        {"raiseNotImplementedError",       (PyCFunction) errorRaiseNotImplementedError,       METH_VARARGS, "Raise a NotImplementedError"},
        {"raiseOSError",                   (PyCFunction) errorRaiseOSError,                   METH_VARARGS, "Raise an OSError"},
        {"raiseOverflowError",             (PyCFunction) errorRaiseOverflowError,             METH_VARARGS, "Raise an OverflowError"},
        {"raisePendingDeprecationWarning", (PyCFunction) errorRaisePendingDeprecationWarning, METH_VARARGS, "Raise a PendingDeprecationWarning"},
        {"raisePermissionError",           (PyCFunction) errorRaisePermissionError,           METH_VARARGS, "Raise a PermissionError"},
        {"raiseProcessLookupError",        (PyCFunction) errorRaiseProcessLookupError,        METH_VARARGS, "Raise a ProcessLookupError"},
        {"raiseRecursionError",            (PyCFunction) errorRaiseRecursionError,            METH_VARARGS, "Raise a RecursionError"},
        {"raiseReferenceError",            (PyCFunction) errorRaiseReferenceError,            METH_VARARGS, "Raise a ReferenceError"},
        {"raiseResourceWarning",           (PyCFunction) errorRaiseResourceWarning,           METH_VARARGS, "Raise a ResourceWarning"},
        {"raiseRuntimeError",              (PyCFunction) errorRaiseRuntimeError,              METH_VARARGS, "Raise a RuntimeError"},
        {"raiseRuntimeWarning",            (PyCFunction) errorRaiseRuntimeWarning,            METH_VARARGS, "Raise a RuntimeWarning"},
        {"raiseStopAsyncIteration",        (PyCFunction) errorRaiseStopAsyncIteration,        METH_VARARGS, "Raise a StopAsyncIteration"},
        {"raiseStopIteration",             (PyCFunction) errorRaiseStopIteration,             METH_VARARGS, "Raise a StopIteration"},
        {"raiseSyntaxError",               (PyCFunction) errorRaiseSyntaxError,               METH_VARARGS, "Raise a SyntaxError"},
        {"raiseSyntaxWarning",             (PyCFunction) errorRaiseSyntaxWarning,             METH_VARARGS, "Raise a SyntaxWarning"},
        {"raiseSystemError",               (PyCFunction) errorRaiseSystemError,               METH_VARARGS, "Raise a SystemError"},
        {"raiseSystemExit",                (PyCFunction) errorRaiseSystemExit,                METH_VARARGS, "Raise a SystemExit"},
        {"raiseTabError",                  (PyCFunction) errorRaiseTabError,                  METH_VARARGS, "Raise a TabError"},
        {"raiseTimeoutError",              (PyCFunction) errorRaiseTimeoutError,              METH_VARARGS, "Raise a TimeoutError"},
        {"raiseTypeError",                 (PyCFunction) errorRaiseTypeError,                 METH_VARARGS, "Raise a TypeError"},
        {"raiseUnboundLocalError",         (PyCFunction) errorRaiseUnboundLocalError,         METH_VARARGS, "Raise an UnboundLocalError"},
        {"raiseUnicodeDecodeError",        (PyCFunction) errorRaiseUnicodeDecodeError,        METH_VARARGS, "Raise an UnicodeDecodeError"},
        {"raiseUnicodeEncodeError",        (PyCFunction) errorRaiseUnicodeEncodeError,        METH_VARARGS, "Raise an UnicodeEncodeError"},
        {"raiseUnicodeError",              (PyCFunction) errorRaiseUnicodeError,              METH_VARARGS, "Raise an UnicodeError"},
        {"raiseUnicodeTranslateError",     (PyCFunction) errorRaiseUnicodeTranslateError,     METH_VARARGS, "Raise an UnicodeTranslateError"},
        {"raiseUnicodeWarning",            (PyCFunction) errorRaiseUnicodeWarning,            METH_VARARGS, "Raise an UnicodeWarning"},
        {"raiseUserWarning",               (PyCFunction) errorRaiseUserWarning,               METH_VARARGS, "Raise an UserWarning"},
        {"raiseValueError",                (PyCFunction) errorRaiseValueError,                METH_VARARGS, "Raise a ValueError"},
        {"raiseWarning",                   (PyCFunction) errorRaiseWarning,                   METH_VARARGS, "Raise a Warning"},
        {NULL}
};

static PyModuleDef errorModule = {
        PyModuleDef_HEAD_INIT,
        .m_name = "error",
        .m_doc = "Error library for Python",
        .m_size = -1,
        errorFunctionMethods
};

PyMODINIT_FUNC
PyInit_error(void) {
    return PyModule_Create(&errorModule);
}
