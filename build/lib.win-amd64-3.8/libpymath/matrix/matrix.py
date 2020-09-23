"""
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
"""

from . import _threadSetup
from . import _threadInfo
import libpymath.core.matrix as _matrix
import sys
import os
from time import time


class Matrix:
    def __init__(self, rows=None, cols=None, data=None, fastTranspose=None, dtype="float64", threads=None, internal_new=False):
        self.__safe_for_unpickling__ = True

        if internal_new:
            self.matrix = None
            self.dtype = None
            self._threads = _threadInfo.LPM_OPTIMAL_MATRIX_THREADS
            self._fastTranspose = None
        else:
            self.matrix = None
            self.dtype = None
            self._threads = None
            self._fastTranspose = None

            if fastTranspose is None:
                self._fastTranspose = True
            else:
                if isinstance(fastTranspose, bool):
                    self._fastTranspose = fastTranspose
                else:
                    try:
                        self._fastTranspose = bool(fastTranspose)
                    except TypeError:
                        raise Exception("Invalid value for fastTranspose") from TypeError

            if threads is not None:
                if isinstance(threads, int):
                    if threads > 0:
                        self._threads = threads
                    else:
                        raise Exception("Invalid number of threads. Must be >= 1") from ValueError
                else:
                    raise Exception("Invalid thread type. Must be of type <int>") from TypeError
            else:
                self._threads = _threadInfo.LPM_OPTIMAL_MATRIX_THREADS

            if dtype is not None:
                if dtype in ("float64", "float32", "int64", "int32", "int16"):
                    self.dtype = dtype
                else:
                    raise Exception("Matrix datatype is not valid") from TypeError
            else:
                raise Exception("Matrix datatype must not be None") from TypeError

            if data is not None:
                # Create matrix from array

                # Ensure data is a list with length > 0
                if isinstance(data, list):
                    if len(data) > 0:
                        # First check for a 2d array
                        if isinstance(data[0], list):
                            # List is *probably* 2d so check remaining elements
                            dLen = len(data[0])
                            for val in data:
                                if isinstance(val, list):
                                    if len(val) == dLen:
                                        # Value is a valid row
                                        pass
                                    else:
                                        # Row was invalid
                                        raise Exception("Invalid matrix shape") from ValueError
                                else:
                                    raise Exception("Invalid matrix shape") from ValueError
                            # At this point the data is a valid 2d array of any datatype,
                            # so while filling the matrix ensure the data is the correct
                            # type.

                            # Matrix type is based on dtype
                            if dtype == "float64":
                                self.matrix = _matrix.matrixFromData2D(data, len(data), dLen)
                            else:
                                raise Exception("Invalid matrix type -- this type may not yet be supported") from TypeError
                        elif isinstance(data[0], (float, int)):
                            # List is *probably* a 1d array to check remaining elements
                            _rows = 0
                            _cols = 0
                            if rows is None:
                                # Matrix is 1 x ? or 1 x cols
                                if cols is None:
                                    _rows = 1
                                    _cols = len(data)
                                else:
                                    if cols != len(data):
                                        raise Exception("Invalid number of columns for matrix") from TypeError

                                    _rows = 1
                                    _cols = cols
                            elif rows is not None and cols is None:
                                # Matrix is rows x 1
                                if rows != len(data):
                                    raise Exception("Invalid number of rows for matrix") from TypeError

                                _rows = rows
                                _cols = 1
                            else:
                                # Matrix is rows x cols
                                if rows * cols != len(data):
                                    raise Exception("Invalid size for matrix") from TypeError

                                _rows = rows
                                _cols = cols

                            # Matrix type is based on dtype
                            if dtype == "float64":
                                self.matrix = _matrix.matrixFromData1D(data, _rows, _cols)
                            else:
                                raise Exception("Invalid matrix type -- this type may not yet be supported") from TypeError
                    else:
                        raise Exception("Matrix initialization data cannot have length of zero") from TypeError
                else:
                    raise Exception("Creating a matrix filled with a value is not yet a feature") from NotImplementedError
            else:
                if isinstance(rows, int) and isinstance(cols, int):
                    # Data was not given so create a matrix from rows and columns alone
                    _rows = rows if rows is not None else 1
                    _cols = cols if cols is not None else 1

                    self.matrix = _matrix.Matrix(_rows, _cols)
                elif isinstance(rows, list):
                    self.__init__(data=rows)

    @staticmethod
    def _internal_new(matrix, dtype="float64", threads=_threadInfo.LPM_OPTIMAL_MATRIX_THREADS):
        res = Matrix(internal_new=True)
        res.matrix = matrix
        res.dtype = dtype
        res._threads = threads

        return res

    @property
    def rows(self):
        return self.matrix.rows

    @property
    def cols(self):
        return self.matrix.cols

    @property
    def rowStride(self):
        return self.matrix.rowStride

    @property
    def colStride(self):
        return self.matrix.colStride

    @property
    def threads(self):
        return self._threads

    @property
    def fastTranspose(self):
        return self._fastTranspose

    @fastTranspose.setter
    def fastTranspose(self, val):
        if val is None:
            self._fastTranspose = True
        else:
            if isinstance(val, bool):
                self._fastTranspose = val
            else:
                try:
                    self._fastTranspose = bool(val)
                except TypeError:
                    raise Exception("Invalid value for fastTranspose") from TypeError    

    def transpose(self):
        if self._fastTranspose:
            self.matrix.transposeMagic()
        else:
            self.matrix = self.matrix.transpose()

    def transposed(self):
        if self._fastTranspose:
            res = self.copy()
            res.transpose()
            return res
        else:
            res = Matrix(self.rows, self.cols, fastTranspose=True, internal_new=True)
            res.matrix = self.matrix.transpose()
            return res

    @property
    def T(self):
        if self._fastTranspose:
            res = self.copy()
            res.transpose()
            return res
        else:
            res = Matrix(self.rows, self.cols, fastTranspose=True, internal_new=True)
            res.matrix = self.matrix.transpose()
            return res

    def dot(self, other):
        if isinstance(other, Matrix) and self.matrix.cols == other.matrix.rows:
            return Matrix._internal_new(self.matrix.matrixProduct(other.matrix, self.threads), self.dtype, self.threads)
        else:
            raise Exception("Invalid matrix size for matrix addition") from TypeError

    def __matmul__(self, other):
        return self.dot(other)

    def __add__(self, other):
        if isinstance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixAddMatrixReturn(other.matrix, self.threads), self.dtype, self.threads)
        else:
            raise Exception("Invalid matrix size for matrix addition") from TypeError

    def __sub__(self, other):
        if instance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixSubMatrixReturn(other.matrix, self.threads), self.dtype, self.threads)
        else:
            raise Exception("Invalid matrix size for matrix subtraction") from TypeError

    def __mul__(self, other):
        if instance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixMulMatrixReturn(other.matrix, self.threads), self.dtype, self.threads)
        else:
            raise Exception("Invalid matrix size for matrix multiplication") from TypeError

    def __truediv__(self, other):
        if instance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixDivMatrixReturn(other.matrix, self.threads), self.dtype, self.threads)
        else:
            raise Exception("Invalid matrix size for matrix division") from TypeError

    # TODO: Make this return a vector of the relevant row -- i.e. matrix[0] -> [1, 2, 3]
    def __getitem__(self, pos):
        if isinstance(pos, tuple):
            i, j = pos
            return self.matrix.get(i, j)
        else:
            raise Exception("Indexing must take two values in this version of libpymath") from NotImplementedError

    def __setitem__(self, pos, val):
        if isinstance(pos, tuple):
            i, j = pos
            self.matrix.set(i, j, val)
        else:
            raise Exception("Indexing must take two values in this version of libpymath") from NotImplementedError

    def toList(self):
        return self.matrix.matrixToList()

    def __str__(self):
        skipRows = self.matrix.rows >= 32
        skipCols = self.matrix.cols >= 32

        longestBeforeDecimal = [0 for _ in range(self.matrix.cols if not skipCols else 6)]
        longestAfterDecimal = [0 for _ in range(self.matrix.cols if not skipCols else 6)]

        i = 0
        while i < self.matrix.rows:
            if skipRows and i == 3:
                i = self.matrix.rows - 3

            j = 0
            while j < self.matrix.cols:
                if skipCols and j == 3:
                    j = self.matrix.cols - 3

                tmp = str(self.matrix.get(i, j))

                if "." in tmp:
                    index = tmp.index(".")
                    jj = j if not skipCols or j < 3 else (j - self.matrix.cols + 6)

                    longestBeforeDecimal[jj] = max([longestBeforeDecimal[jj], index])
                    longestAfterDecimal[jj] = max([longestAfterDecimal[jj], len(tmp) - index])

                else:
                    jj = j if not skipCols or j < 3 else (j - self.matrix.cols + 6)
                    longestBeforeDecimal[jj] = max([longestBeforeDecimal[jj], len(tmp)])

                j += 1
            i += 1

        res = "["
        i = 0

        while i < self.matrix.rows:
            if skipRows and i == 3:
                i = self.matrix.rows - 3

                for x in range(6):
                    res += ("  " if x == 0 else "") + " " * (longestBeforeDecimal[x] - 1) + "***" + (" " * longestAfterDecimal[x])
                    if x == 2:
                        res += "     "

                res += "\n"

            res += "[" if i == 0 else " ["

            j = 0
            while j < self.matrix.cols:
                if skipCols and j == 3:
                    j = self.matrix.cols - 3
                    res += "  ***  "

                tmp = str(self.matrix.get(i, j))
                jj = j if not skipCols or j < 3 else (j - self.matrix.cols + 6)

                length = tmp.index(".") if "." in tmp else len(tmp)
                for k in range(longestBeforeDecimal[jj] - length):
                    res += " "

                res += tmp

                length = (len(tmp) - tmp.index(".")) if "." in tmp else len(tmp)
                for k in range(longestAfterDecimal[jj] - length):
                    res += " "

                if j + 1 < self.matrix.cols and not (skipCols and j == 2):
                    res += ", "

                j += 1

            res += "]" + ("\n" if i + 1 < self.matrix.rows else "")
            i += 1

        return res + "]"

    def __repr__(self):
        return str(self)

    def __reduce__(self):
        return (
            Matrix,
            (self.matrix.rows, self.matrix.cols, self.toList(), self.dtype, self.threads)
        )

    def copy(self):
        res = Matrix(internal_new=True)
        res.matrix = self.matrix.copy()
        res.dtype = self.dtype
        res._threads = self._threads
        res._fastTranspose = self._fastTranspose

        return res
