# Import required modules
try:
    from . import _threadSetup, _threadInfo
except ImportError:
    class _threadInfo:
        @property
        def LPM_OPTIMAL_MATRIX_THREADS(self):
            return 4

import libpymath.core.matrix as _matrix

__all__ = ["Matrix", "SCALAR", "ASCENDING", "DESCENDING", "RANDOM", "SIGMOID", "TANH", "RELU", "LEAKY_RELU",
           "D_SIGMOID", "D_TANH", "D_RELU", "D_LEAKY_RELU"]

# Matrix fill options
SCALAR = 1
ASCENDING = 2
DESCENDING = 3
RANDOM = 4

# Matrix map options (shift 5 left for corresponding derivative)
SIGMOID = 1 << 5
TANH = 1 << 6
RELU = 1 << 7
LEAKY_RELU = 1 << 8

# Matrix map derivative options (shift 5 right for corresponding activation)
D_SIGMOID = 1 << 10
D_TANH = 1 << 11
D_RELU = 1 << 12
D_LEAKY_RELU = 1 << 13


# The Matrix class
class Matrix:
    def __init__(self, *args, **kwargs):
        """
        Create a new instance of the libpymath Matrix type.

        The libpymath matrix type allows for fast, intuitive matrix calculations to be run
        with only a single line of code. The matrix type allows you to create large, dense matrices
        that are stored with homogenous types in C, improving the speed and reducing memory usage.

        The matrix type also makes use of multiple cores and threads, greatly improving performance
        for larger matrices, while smaller matrices have custom routines that make them even faster
        as well.

        :param rows: The rows of the matrix
        :param cols: The columns of the matrix
        :param data: The data to fill the matrix with
        :param threads: The number of threads to use for matrix calculations
        :param dtype: The datatype of the matrix. Defaults to "float64"
        """

        self.__safe_for_unpickling__ = True

        # Override kwargs if args are specified. This makes it easier
        # to use and increases functionality (e.g. pickling)
        if len(args) == 1:
            kwargs["rows"] = args[0]
        elif len(args) == 2:
            kwargs["rows"] = args[0]
            kwargs["cols"] = args[1]
        elif len(args) == 3:
            kwargs["rows"] = args[0]
            kwargs["cols"] = args[1]
            kwargs["data"] = args[2]
        elif len(args) == 4:
            kwargs["rows"] = args[0]
            kwargs["cols"] = args[1]
            kwargs["data"] = args[2]
            kwargs["dtype"] = args[3]
        elif len(args) == 5:
            kwargs["rows"] = args[0]
            kwargs["cols"] = args[1]
            kwargs["data"] = args[2]
            kwargs["dtype"] = args[3]
            kwargs["threads"] = args[4]

        # Create a matrix internally
        if "internal_new" in kwargs:
            self.matrix = None
            self._dtype = None
            self._threads = _threadInfo.LPM_OPTIMAL_MATRIX_THREADS
        else:
            # These should all be set by the end
            rows = None
            cols = None
            data = None
            dataDims = []
            threads = None
            dtype = None

            # Sanity check
            if len(kwargs) == 0:
                raise ValueError("Matrix cannot be created with 0 arguments")

            # Start with the easy arguments

            # Matrix threads
            if "threads" in kwargs:
                if isinstance(kwargs["threads"], int):
                    threads = kwargs["threads"]

                    if threads <= 0:
                        raise ValueError("Threads must be positive")
                else:
                    raise TypeError("Threads must be an integer, not {}".format(type(kwargs["threads"])))
            else:
                threads = _threadInfo.LPM_OPTIMAL_MATRIX_THREADS

            # Matrix datatype
            if "dtype" in kwargs:
                if isinstance(kwargs["dtype"], str):
                    dtype = kwargs["dtype"]

                    # TODO: Add different datatypes
                    if dtype != "float64":
                        raise NotImplementedError("Matrix currently only supports \"float64\" as datatype. {} is invalid".format(dtype))
                else:
                    raise TypeError("Datatype must be a string, not {}".format(type(kwargs["dtype"])))

            # First check for rows x columns
            if "N" in kwargs:
                if isinstance(kwargs["N"], int):
                    rows = kwargs["N"]
                    cols = kwargs["N"]

                    if rows <= 0 or cols <= 0:
                        raise ValueError("Matrix must have positive dimensions")
                else:
                    raise TypeError("Matrix columns must be an integer")
            else:
                if "rows" in kwargs or "cols" in kwargs:
                    # Rows or columns are specified

                    if "rows" in kwargs:
                        # Check rows is an integer
                        if isinstance(kwargs["rows"], int):
                            rows = kwargs["rows"]
                        else:
                            raise TypeError("Matrix rows must be an integer")

                    if "cols" in kwargs:
                        # Check rows is an integer
                        if isinstance(kwargs["cols"], int):
                            cols = kwargs["cols"]
                        else:
                            raise TypeError("Matrix columns must be an integer")

                    # Sanity check
                    if (rows is not None and rows <= 0) or (cols is not None and cols <= 0):
                        raise ValueError("Matrix must have positive dimensions")

            # Check for data
            if "data" in kwargs:
                # Create local variable for increased speed
                tmp = kwargs["data"]

                # Check for 1d list
                if isinstance(tmp[0], (int, float)):
                    # List *should* be a 1d list at this point
                    data = tmp
                    dataDims = [len(tmp)]
                elif isinstance(tmp[0], (list, tuple)):
                    # List *should* be a 2d list here
                    data = tmp
                    dataDims = [len(tmp), len(tmp[0])]
                else:
                    # An invalid value will be found by the C code. No point checking here
                    pass

                # Map data to rows and columns
                if len(dataDims) == 1:
                    # Check against 1d list
                    if rows is not None and cols is not None and rows * cols != len(tmp):
                        raise ValueError("Dimensions provided do not fit with the data provided")
                    else:
                        if rows is None and cols is not None:
                            rows = len(tmp) // cols
                            if rows * cols != len(tmp):
                                raise ValueError("Impossible to make dimensions fit the supplied data. Closest fit is {}x{}".format(rows, cols))
                        elif cols is None and rows is not None:
                            cols = len(tmp) // rows
                            if rows * cols != len(tmp):
                                raise ValueError("Impossible to make dimensions fit the supplied data. Closest fit is {}x{}".format(rows, cols))
                        else:
                            # No rows or columns specified
                            rows = 1
                            cols = len(data)
                else:
                    # Check against a 2d list
                    if rows is not None and cols is not None:
                        # Rows and cols are valid
                        if rows * cols != dataDims[0] * dataDims[1]:
                            raise ValueError("Data supplied does not fit the provided dimensions")
                        else:
                            # Reshaping is possible
                            pass
                    else:
                        if rows is None and cols is not None:
                            # Find rows
                            rows = (dataDims[0] * dataDims[1]) // cols

                            # Check the dims fit
                            if rows * cols != dataDims[0] * dataDims[1]:
                                raise ValueError("Impossible to make dimensions fit the supplied data. Closest fit is {]x{}".format(rows, cols))
                        else:
                            # Find cols
                            cols = (dataDims[0] * dataDims[1]) // rows

                            # Check the dims fit
                            if rows * cols != dataDims[0] * dataDims[1]:
                                raise ValueError("Impossible to make dimensions fit the supplied data. Closest fit is {]x{}".format(rows, cols))
            else:
                if rows is None:
                    rows = 1
                if cols is None:
                    cols = 1

            # Construct the matrix
            if data is not None:
                if len(dataDims) == 1:
                    self.matrix = _matrix.matrixFromData1D(data, rows, cols)
                else:
                    self.matrix = _matrix.matrixFromData2D(data, dataDims[0], dataDims[1])

                    # reshape
                    if rows != dataDims[0] or cols != dataDims[1]:
                        self.matrix.matrixReshape(rows, cols)
            else:
                self.matrix = _matrix.Matrix(rows, cols)
            self._threads = threads
            self._dtype = dtype

    @staticmethod
    def _internal_new(matrix, dtype="float64", threads=_threadInfo.LPM_OPTIMAL_MATRIX_THREADS):
        """
        FOR INTERNAL USE ONLY

        Create a new matrix for internal use. This is faster than going through the entire __init__
        function, as no data needs to be processed.

        :param matrix: Matrix core object to use as the result's matrix
        :param dtype: Datatype of the matrix object
        :param threads: Number of threads to use
        :return: Matrix object from supplied information
        """

        # Create and initialise values in res
        res = Matrix(internal_new=True)
        res.matrix = matrix
        res._dtype = dtype
        res._threads = threads

        return res

    @property
    def rows(self):
        """
        :return: The number of rows of the matrix
        """
        return self.matrix.rows

    @property
    def cols(self):
        """
        :return: The number of columns of the matrix
        """
        return self.matrix.cols

    @property
    def dtype(self):
        """
        :return: The datatype of the matrix
        """
        return self._dtype

    @property
    def shape(self):
        """
        :return: The shape of the matrix in the form (rows, columns)
        """
        return self.rows, self.cols

    @property
    def threads(self):
        """
        :return: The number of threads the matrix is using
        """
        return self._threads

    def transpose(self):
        """
        Transpose a matrix inplace.

        :return: None
        """
        self.matrix = self.matrix.transpose()

    def transposed(self):
        """
        See Matrix.transpose()

        :return: Return the transpose of a matrix
        """

        # Create result
        res = Matrix(self.rows, self.cols, internal_new=True)
        res.matrix = self.matrix.transpose()
        return res

    @property
    def T(self):
        """
        See Matrix.transposed()

        :return: Return the transpose of a matrix
        """

        # Create result
        res = Matrix(self.rows, self.cols, internal_new=True)
        res.matrix = self.matrix.transpose()
        return res

    def dot(self, other):
        """
        Compute the matrix-matrix product with another matrix

        :param other: Matrix to compute matrix product with
        :return: Result of matrix product calculation
        """

        if isinstance(other, Matrix) and self.matrix.cols == other.matrix.rows:
            return Matrix._internal_new(self.matrix.matrixProduct(other.matrix, self.threads), self._dtype, self.threads)
        else:
            raise TypeError("Invalid matrix size for matrix product")

    def __matmul__(self, other):
        """
        See Matrix.dot()

        :param other: Matrix to compute matrix product with
        :return: Result of matrix product calculation
        """

        return self.dot(other)

    def sum(self):
        return self.matrix.matrixSum()

    def mean(self):
        return self.matrix.matrixMean()

    def __add__(self, other):
        """
        Add a matrix to another matrix elementwise, or add a scalar to every value

        :param other: Matrix or scalar
        :return: Result of addition
        """

        if isinstance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixAddMatrixReturn(other.matrix, self.threads), self._dtype, self.threads)
        elif isinstance(other, (int, float)):
            return Matrix._internal_new(self.matrix.matrixAddScalarReturn(other, self.threads), self._dtype, self.threads)
        else:
            raise TypeError("Invalid matrix size for matrix addition")

    def __sub__(self, other):
        """
        Subtract a matrix from another matrix elementwise, or subtract a scalar from every value

        :param other: Matrix or scalar
        :return: Result of subtraction
        """

        if isinstance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixSubMatrixReturn(other.matrix, self.threads), self._dtype, self.threads)
        elif isinstance(other, (int, float)):
            return Matrix._internal_new(self.matrix.matrixSubScalarReturn(other, self.threads), self._dtype, self.threads)
        else:
            raise TypeError("Invalid matrix size for matrix subtraction")

    def __mul__(self, other):
        """
        Multiply a matrix by another matrix elementwise, or multiply every value by a scalar

        :param other: Matrix or scalar
        :return: Result of multiplication
        """

        if isinstance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixMulMatrixReturn(other.matrix, self.threads), self._dtype, self.threads)
        elif isinstance(other, (int, float)):
            return Matrix._internal_new(self.matrix.matrixMulScalarReturn(other, self.threads), self._dtype, self.threads)
        else:
            raise TypeError("Invalid matrix size for matrix multiplication")

    def __truediv__(self, other):
        """
        Divide a matrix by another matrix elementwise, or divide every value by a scalar

        :param other: Matrix or scalar
        :return: Result of division
        """

        if isinstance(other, Matrix) and self.matrix.rows == other.matrix.rows and self.matrix.cols == other.matrix.cols:
            return Matrix._internal_new(self.matrix.matrixDivMatrixReturn(other.matrix, self.threads), self._dtype, self.threads)
        elif isinstance(other, (int, float)):
            return Matrix._internal_new(self.matrix.matrixDivScalarReturn(other, self.threads), self._dtype, self.threads)
        else:
            raise TypeError("Invalid matrix size for matrix division")

    def __iadd__(self, other):
        """
        See Matrix.__add__()

        :param other: Value to add
        :return: Result of addition
        """

        return self + other

    def __isub__(self, other):
        """
        See Matrix.__sub__()

        :param other: Value to subtract
        :return: Result of subtraction
        """

        return self - other

    def __imul__(self, other):
        """
        See Matrix.__mul__()

        :param other: Value to multiply by
        :return: Result of multiplication
        """

        return self * other

    def __itruediv__(self, other):
        """
        See Matrix.__div__()

        :param other: Value to divide by
        :return: Result of division
        """

        return self / other

    # TODO: Make this adjust rows/cols when only one value is specified
    def reshape(self, nr, nc):
        """
        Reshape a matrix by adjusting the rows and columns.

        :param nr: New rows
        :param nc: New columns
        :return: None
        """

        self.matrix.matrixReshape(nr, nc)

    # TODO: Make this adjust rows/cols when only one value is specified
    def reshaped(self, nr, nc):
        """
        Reshape a matrix by adjusting the rows and columns and return the result.

        :param nr: New rows
        :param nc: New columns
        :return: Reshaped matrix
        """

        res = self.copy()
        res.reshape(nr, nc)
        return res

    def fill(self, fillType, *args):
        """
        Fill a matrix. Valid fills are:

        SCALAR      -> Fill with a single scalar value
        ASCENDING   -> Fill a matrix with ascending values, starting with 0
        DESCENDING  -> Fill a matrix with descending values, starting from (rows * cols) - 1
        RANDOM      -> Fill a matrix with random values between a given range (defaults to [-1, 1|)

        :param fillType: Method to use when filling the matrix
        :param args: Some fill methods accept parameters
        :return: None
        """

        if fillType == SCALAR:
            self.matrix.matrixFillScalar(args[0] if len(args) > 0 else 0)
        elif fillType == ASCENDING:
            self.matrix.matrixFillAscending()
        elif fillType == DESCENDING:
            self.matrix.matrixFillDescending()
        elif fillType == RANDOM:
            if len(args) == 1:
                self.matrix.matrixFillRandom(0, args[0])
            elif len(args) == 2:
                self.matrix.matrixFillRandom(args[0], args[1])
            else:
                self.matrix.matrixFillRandom(-1, 1)
        else:
            raise TypeError("Invalid fill type")

    def map(self, mapType):
        """
        Apply a function to every element of the matrix.

        Valid functions are:
        SIGMOID
        TANH
        RELU
        LEAKY_RELU

        D_SIGMOID
        D_TANH
        D_RELU
        D_LEAKY_RELU

        :param mapType: Function to map with
        :return: None
        """

        if mapType == SIGMOID:
            self.matrix.matrixMapSigmoid()
        elif mapType == TANH:
            self.matrix.matrixMapTanh()
        elif mapType == RELU:
            self.matrix.matrixMapRELU()
        elif mapType == LEAKY_RELU:
            self.matrix.matrixMapLeakyRELU()
        elif mapType == D_SIGMOID:
            self.matrix.matrixMapSigmoidDerivative()
        elif mapType == D_TANH:
            self.matrix.matrixMapTanhDerivative()
        elif mapType == D_RELU:
            self.matrix.matrixMapRELUDerivative()
        elif mapType == D_LEAKY_RELU:
            self.matrix.matrixMapLeakyRELUDerivative()
        else:
            raise TypeError("Invalid mapping type")

    def mapped(self, mapType):
        """
        See Matrix.map()

        :param mapType: Function to map with
        :return: Mapped matrix
        """

        res = self.copy()
        res.map(mapType)
        return res

    def fillScalar(self, x):
        """
        See Matrix.map(SCALAR)

        :param x: Scalar value to fill with
        :return: None
        """

        self.matrix.matrixFillScalar(x)

    def fillAscending(self):
        """
        See Matrix.map(ASCENDING)

        :return: None
        """

        self.matrix.matrixFillAscending()

    def fillDescending(self):
        """
        See Matrix.map(DESCENDING)

        :return: None
        """

        self.matrix.matrixFillDescending()

    def fillRandom(self, _min=None, _max=None):
        """
        See Matrix.map(RANDOM)

        :return: None
        """

        if _min is not None and _max is None:
            if _min < 0:
                self.matrix.matrixFillRandom(_min, 0)
            else:
                self.matrix.matrixFillRandom(0, _min)
        elif _min is None and _max is not None:
            if _max < 0:
                self.matrix.matrixFillRandom(_max, 0)
            else:
                self.matrix.matrixFillRandom(0, _max)
        elif _min is not None and _max is not None:
            self.matrix.matrixFillRandom(_min, _max)
        else:
            self.matrix.matrixFillRandom(-1, 1)

    # TODO: Make this return a vector of the relevant row -- i.e. matrix[0] -> [1, 2, 3]
    def __getitem__(self, pos):
        """
        Return the value at a given index.

        Index must be given in the form:
        [row, column]

        :param pos: Index to get
        :return: Float value at that point in the matrix
        """

        if isinstance(pos, tuple):
            i, j = pos
            return self.matrix.get(i, j)
        else:
            raise NotImplementedError("Indexing must take two values in this version of libpymath")

    def __setitem__(self, pos, val):
        """
        Set the value at a given index.

        Index must be given in the form:
        [row, column]

        :param pos: Index to set
        :param val: Value to set to
        :return: None
        """

        if isinstance(pos, tuple):
            i, j = pos
            self.matrix.set(i, j, val)
        else:
            raise NotImplementedError("Indexing must take two values in this version of libpymath")

    def toList(self):
        """
        Convert a Matrix into a 2d Python list

        :return: 2d Python list
        """

        return self.matrix.matrixToList()

    def __str__(self):
        """
        Convert the matrix to a string. This is mainly used for printing.

        This function aligns all of the columns, decimal points, brackets
        and commas, improving readability of the matrix

        :return: String representation of the matrix
        """

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
        """
        See Matrix.__str__()

        :return: String representation of matrix
        """

        return str(self)

    def __reduce__(self):
        """
        For the pickle module

        :return: Pickle-able object
        """

        return (
            Matrix,
            (self.matrix.rows, self.matrix.cols, self.toList(), self._dtype, self.threads)
        )

    def copy(self):
        """
        Return an exact copy of a matrix

        :return: Copied matrix
        """

        res = Matrix(internal_new=True)
        res.matrix = self.matrix.copy()
        res._dtype = self._dtype
        res._threads = self._threads

        return res
