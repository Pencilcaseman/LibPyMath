[![Build Status](https://travis-ci.com/Pencilcaseman/LibPyMath.svg?branch=master)](https://travis-ci.com/Pencilcaseman/LibPyMath)
[![Documentation Status](https://readthedocs.org/projects/libpymath/badge/?version=latest)](https://libpymath.readthedocs.io/en/latest/?badge=latest)
[![PyPI version fury.io](https://badge.fury.io/py/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)
[![PyPI license](https://img.shields.io/pypi/l/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)
[![PyPI pyversions](https://img.shields.io/pypi/pyversions/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)
[![Downloads per month](https://img.shields.io/pypi/dm/libpymath.svg)](https://pypi.python.org/pypi/libpymath/)

# LibPyMath
### A fast, general purpose math library for Python
---

## Install
Run ```pip install libpymath``` to dowload and install ```libpymath``` for your system. There are also wheels provided for many of the latest Python versions on Windows, Mac OS* and Linux thanks to the ```cibuildwheel``` project.

---

## Features and usage
### Matrix math
Easily create, manipulate and perform calculations with dense matrices. The matrices themselves are stored and manipulated with optimised C code, resulting in faster operations and more efficient calculations. To further increase the speed of the calculations, when libpymath imported into a project for the first time, it runs some tests on the CPU to find the optimal number of threads to use for the matrix calculations.

The matrix library contains many features, such as:
1. Elementwise addition, subtraction, multiplication and division with another matrix
2. Addition, subtraction, multiplication and division by a scalar
3. Matrix transpose,
4. Matrix product,
5. Filling a matrix with ascending values, descending values, random values or a single value
6. Getting or setting values
7. Creating a matrix from data
8. Mapping with sigmoid, tanh, ReLU and leaky ReLU, as well as their derivatives
9. Getting the matrix as a Python list
10. Supports pickling with the standard library ```pickle``` module
11. Formatting and printing a matrix

\* Due to Clang on Mac OS the wheels do not support OpenMP, meaning some matrix operations may be slower than on other operating systems.
