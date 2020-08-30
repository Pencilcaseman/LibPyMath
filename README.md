[![Build Status](https://travis-ci.com/Pencilcaseman/LibPyMath.svg?branch=master)](https://travis-ci.com/Pencilcaseman/LibPyMath)
[![Documentation Status](https://readthedocs.org/projects/libpymath/badge/?version=latest)](https://libpymath.readthedocs.io/en/latest/?badge=latest)
[![PyPI version fury.io](https://badge.fury.io/py/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)
[![PyPI license](https://img.shields.io/pypi/l/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)
[![PyPI pyversions](https://img.shields.io/pypi/pyversions/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)
[![Downloads per month](https://img.shields.io/pypi/dm/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)

# LibPyMath
### A fast, general purpose Python library
---

## Install
Run ```pip install libpymath``` to dowload and install ```libpymath``` for your operating system and python version. There are also wheels provided for many of the latest Python versions on Windows, Mac OS and Linux thanks to the ```cibuildwheel``` project.

---

## Features and usage
### Matrix math
Easily create, manipulate and perform calculations with dense matrices. The matrices themselves are stored and manipulated with optimised C code, resulting in faster operations and more efficient calculations.

The matrix library is currently very primitive and supports very few operations, though is constantly being expanded on and improved. Many ease of use features also exist, such as the ability to format and print a matrix, alligning the decimal points (if present) and providing brackets in the relevant places.

For example, the following creates a new matrix from a 2d list of data
```python
from libpymath.matrix import Matrix

matrixData = [[1, 2, 3],
              [4, 5, 6],
              [7, 8, 9]]

mat = Matrix(data=matrixData)

print(mat)

>>> [[1, 2, 3]
     [4, 5, 6]
     [7, 8, 9]]
```
