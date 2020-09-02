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

from libpymath.core import matrix as _matrix


def _internalGet(i, j, r, c):
    return j * c + i * r


class Matrix:
    def __init__(self, rows=None, cols=None, data=None, dtype="float64"):
        self.matrix = None
        self._rows = None
        self._cols = None
        self._rowStride = None
        self._colStride = None
        self.dtype = None

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
                        self._rows = len(data)
                        self._cols = dLen
                        self._rowStride = self._cols
                        self._colStride = 1

                        # Matrix type is based on dtype
                        if dtype == "float64":
                            self.matrix = _matrix.matrixFromData2D(data, self._rows, self._cols)
                        else:
                            raise Exception("Invalid matrix type -- this type may not yet be supported") from TypeError
                    elif isinstance(data[0], (float, int)):
                        # List is *probably* a 1d array to check remaining elements
                        for val in data:
                            if not isinstance(val, (int, float)):
                                raise Exception("Invalid type for matrix initialization") from TypeError

                        if rows is None:
                            # Matrix is 1 x ? or 1 x cols
                            if cols is None:
                                self._rows = 1
                                self._cols = len(data)
                                self._rowStride = self._cols
                                self._colStride = 1
                            else:
                                if cols != len(data):
                                    raise Exception("Invalid number of columns for matrix") from TypeError

                                self._rows = 1
                                self._cols = cols
                                self._rowStride = cols
                                self._colStride = 1
                        elif rows is not None and cols is None:
                            # Matrix is rows x 1
                            if rows != len(data):
                                raise Exception("Invalid number of rows for matrix") from TypeError

                            self._rows = rows
                            self._cols = 1
                            self._rowStride = 1
                            self._colStride = 1
                        else:
                            # Matrix is rows x cols
                            if rows * cols != len(data):
                                raise Exception("Invalid size for matrix") from TypeError

                            self._rows = rows
                            self._cols = cols
                            self._rowStride = cols
                            self._colStride = 1

                        # Matrix type is based on dtype
                        if dtype == "float64":
                            self.matrix = _matrix.matrixFromData1D(data, self._rows, self._cols)
                        else:
                            raise Exception("Invalid matrix type -- this type may not yet be supported") from TypeError
                else:
                    raise Exception("Matrix initialization data cannot have length of zero") from TypeError
            else:
                raise Exception("Creating a matrix filled with a value is not yet a feature") from NotImplementedError
        else:
            # Data was not given so create a matrix from rows and columns alone
            self._rows = rows if rows is not None else 1
            self._cols = cols if cols is not None else 1
            self._rowStride = self._cols
            self._colStride = 1

            self.matrix = _matrix.Matrix(self._rows, self._cols)

    @property
    def rows(self):
        return self._rows

    @property
    def cols(self):
        return self._cols

    @property
    def rowStride(self):
        return self._rowStride

    @property
    def colStride(self):
        return self._colStride

    def transpose(self) -> None:
        # Swap row and column stride
        self._rowStride, self._colStride = self._colStride, self._rowStride
        # Swap rows and columns
        self._rows, self._cols = self._cols, self._rows
        self.matrix.transposeMagic()

    @property
    def T(self):
        res = self.copy()
        res.transpose()
        return res

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

    def __str__(self):
        skipRows = self._rows >= 32
        skipCols = self._cols >= 32

        longestBeforeDecimal = [0 for _ in range(self._cols if not skipCols else 6)]
        longestAfterDecimal = [0 for _ in range(self._cols if not skipCols else 6)]

        i = 0
        while i < self._rows:
            if skipRows and i == 3:
                i = self._rows - 3

            j = 0
            while j < self._cols:
                if skipCols and j == 3:
                    j = self._cols - 3

                tmp = str(self.matrix.get(i, j))

                if "." in tmp:
                    index = tmp.index(".")
                    jj = j if not skipCols or j < 3 else (j - self._cols + 6)

                    longestBeforeDecimal[jj] = max([longestBeforeDecimal[jj], index])
                    longestAfterDecimal[jj] = max([longestAfterDecimal[jj], len(tmp) - index])

                else:
                    jj = j if not skipCols or j < 3 else (j - self._cols + 6)
                    longestBeforeDecimal[jj] = max([longestBeforeDecimal[jj], len(tmp)])

                j += 1
            i += 1

        res = "["
        i = 0

        while i < self._rows:
            if skipRows and i == 3:
                i = self._rows - 3

                for x in range(6):
                    res += ("  " if x == 0 else "") + " " * (longestBeforeDecimal[x] - 1) + "***" + (" " * longestAfterDecimal[x])
                    if x == 2:
                        res += "     "

                res += "\n"

            res += "[" if i == 0 else " ["

            j = 0
            while j < self._cols:
                if skipCols and j == 3:
                    j = self._cols - 3
                    res += "  ***  "

                tmp = str(self.matrix.get(i, j))
                jj = j if not skipCols or j < 3 else (j - self._cols + 6)

                length = tmp.index(".") if "." in tmp else len(tmp)
                for k in range(longestBeforeDecimal[jj] - length):
                    res += " "

                res += tmp

                length = (len(tmp) - tmp.index(".")) if "." in tmp else len(tmp)
                for k in range(longestAfterDecimal[jj] - length):
                    res += " "

                if j + 1 < self._cols and not (skipCols and j == 2):
                    res += ", "

                j += 1

            res += "]" + ("\n" if i + 1 < self._rows else "")
            i += 1

        return res + "]"

    def __repr__(self):
        return str(self)

    def copy(self):
        res = Matrix(rows=1, cols=1)
        res.matrix = self.matrix.copy()
        res._rows = self._rows
        res._cols = self._cols
        res._rowStride = self._rowStride
        res._colStride = self._colStride
        res.dtype = self.dtype

        return res
