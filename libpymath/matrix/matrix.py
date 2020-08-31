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


class Matrix:
    def __init__(self, rows=0, cols=0, dtype="float64", data=None):
        """
        Create a new Matrix object

        rows -- Number of rows for the matrix
        self.cols -- Number of columns for the matrix
        dtype -- Data type for the matrix (float64, float32, int64, int32, int16)
        data -- Data to create matrix from (1d or 2d array)
        """

        self.matrix = None
        self.rows = None
        self.cols = None
        self.rowStride = None
        self.colStride = None
        self.dtype = None

        if dtype is not None:
            self.dtype = dtype
        else:
            raise Exception("Matrix datatype must not be None") from TypeError

        if data is not None:
            # Create matrix from array

            # First check for a 2d array
            if isinstance(data[0], list):
                # List is *probably* 2d so check remaining elements
                l = len(data[0])
                for val in data:
                    if isinstance(val, list):
                        if len(val) == l:
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
                self.rows = len(data)
                self.cols = l
                self.rowStride = self.cols
                self.colStride = 1

                # Matrix type is based on dtype
                if dtype == "float64":
                    self.matrix = _matrix.matrixFromData2D(data, self.rows, self.cols)
                else:
                    raise Exception("Invalid matrix type -- this type may not yet be supported") from TypeError

    def __str__(self):
        skipRows = self.rows >= 32
        skipCols = self.cols >= 32

        longestBeforeDecimal = [0 for _ in range(self.cols if not skipCols else 6)]
        longestAfterDecimal = [0 for _ in range(self.cols if not skipCols else 6)];

        i = 0
        while i < self.rows:
            if skipRows and i == 3:
                i = self.rows - 3

            j = 0
            while j < self.cols:
                if skipCols and j == 3:
                    j = self.cols - 3

                tmp = str(self.matrix.get(j + i * self.cols))

                if "." in tmp:
                    index = tmp.index(".")
                    jj = j if not skipCols or j < 3 else (j - self.cols + 6)

                    longestBeforeDecimal[jj] = max([longestBeforeDecimal[jj], index])
                    longestAfterDecimal[jj] = max([longestAfterDecimal[jj], len(tmp) - index])

                else:
                    jj = j if not skipCols or j < 3 else (j - self.cols + 6)
                    longestBeforeDecimal[jj] = max([longestBeforeDecimal[jj], len(tmp)])

                j += 1
            i += 1

        res = "["
        i = 0

        while i < self.rows:
            if skipRows and i == 3:
                i = self.rows - 3

                for x in range(6):
                    res += ("  " if x == 0 else "") + " " * (longestBeforeDecimal[x] - 1) + "***" + (" " * longestAfterDecimal[x])
                    if x == 2:
                        res += "     "

                res += "\n"

            res += "[" if i == 0 else " ["

            j = 0
            while j < self.cols:
                if skipCols and j == 3:
                    j = self.cols - 3
                    res += "  ***  "

                tmp = str(self.matrix.get(j + i * self.cols))
                jj = j if not skipCols or j < 3 else (j - self.cols + 6)

                length = tmp.index(".") if "." in tmp else len(tmp)
                for k in range(longestBeforeDecimal[jj] - length):
                    res += " "

                res += tmp

                length = (len(tmp) - tmp.index(".")) if "." in tmp else len(tmp)
                for k in range(longestAfterDecimal[jj] - length):
                    res += " "

                if j + 1 < self.cols and not (skipCols and j == 2):
                    res += ", "

                j += 1

            res += "]" + ("\n" if i + 1 < self.rows else "")
            i += 1

        return res + "]"

    def __repr__(self):
        return str(self)

    def copy(self):
        res = Matrix(data=[[0]])
        res.matrix = self.matrix.copy()
        res.rows = self.rows
        res.cols = self.cols
        res.rowStride = self.rowStride
        res.colStride = self.colStride
        res.dtype = self.dtype

        return res
