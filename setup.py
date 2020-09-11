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

from pathlib import Path
import platform
import os
import sys

try:
    from setuptools import setup, Extension, find_packages
except ImportError:
    from distutils.core import setup, Extension

    def find_packages(where='.'):
        return [folder.replace("/", ".").lstrip(".")
                for (folder, _, fils) in os.walk(where)
                if "__init__.py" in fils]


def compilerName():
  import re
  import distutils.ccompiler
  comp = distutils.ccompiler.get_default_compiler()
  getnext = False

  for a in sys.argv[2:]:
    if getnext:
      comp = a
      getnext = False
      continue
    #separated by space
    if a == '--compiler'  or  re.search('^-[a-z]*c$', a):
      getnext = True
      continue
    #without space
    m = re.search('^--compiler=(.+)', a)
    if m == None:
      m = re.search('^-[a-z]*c(.+)', a)
    if m:
      comp = m.group(1)

  return comp

this_directory = Path(__file__).parent
long_description = (this_directory / 'README.md').read_text(encoding='utf-8')

def stdCompile():
	c = compilerName()
	if c == "msvc":
		return ""
	elif c in ("gcc", "g++"):
		return "-std=c99"
	elif c == "clang":
		return "-std=c99"
	elif c == "unix":
		return "-std=c99"
	return ""

def openmpCompile():
	c = compilerName()
	if c == "msvc":
		return "/openmp"
	elif c in ("gcc", "g++"):
		return "-fopenmp"
	elif c == "clang":
		return "-fopenmp"
	elif c == "unix":
		return "-fopenmp"
	return ""

def fpicCompile():
	c = compilerName()
	if c == "msvc":
		return ""
	elif c in ("gcc", "g++"):
		return "-fpic"
	elif c == "clang":
		return "-fpic"
	elif c == "unix":
		return "-fpic"
	return ""

def o3Compile():
	c = compilerName()
	if c == "msvc":
		return "/O3"
	elif c in ("gcc", "g++"):
		return "-O3"
	elif c == "clang":
		return "-O3"
	elif c == "unix":
		return "-O3"
	return ""

def mavxCompile():
	c = compilerName()
	if c == "msvc":
		return ""
	elif c in ("gcc", "g++"):
		return "-mavx"
	elif c == "clang":
		return "-mavx"
	elif c == "unix":
		return "-mavx"
	return ""

def m64Compile():
	if platform.system() != "Linux":
		c = compilerName()
		if c == "msvc":
			return ""
		elif c in ("gcc", "g++"):
			return "-m64"
		elif c == "clang":
			return "-m64"
	return ""

def lgompLink():
	c = compilerName()
	if c == "mvcc":
		return "-lgomp"
	elif c in ("gcc", "g++"):
		return "-lgomp"
	elif c == "clang":
		return ""
	return ""


compiler_flags = [stdCompile(), openmpCompile(), fpicCompile(), o3Compile(), mavxCompile()]
link_args = [lgompLink]
macros = []

ext_modules = [
    Extension(
        "libpymath.core.testModule",
        ["libpymath/LibPyMathModules/testModule.c"],
        extra_compile_args=compiler_flags,
        extra_link_args=link_args
    ),
    Extension(
        "libpymath.core.matrix",
        ["libpymath/LibPyMathModules/matrix/matrixModule.c"],
        extra_compile_args=compiler_flags,
        extra_link_args=link_args,
        define_macros=macros,
        include_dirs=["./"]
    )
]

setup(
    name="libpymath",
    version="0.0.63",
    description="A general purpose Python math module",
    long_description=long_description,
    long_description_content_type='text/markdown',
    author="Toby Davis",
    author_email="pencilcaseman@gmail.com",
    url="https://www.github.com/pencilcaseman/gpc",
    ext_modules=ext_modules,
    packages=["libpymath"] + ["libpymath." + mod for mod in find_packages("libpymath")],
    license="MIT Licences",
    keywords=["math", "matrix", "vector", "network", "neural network",
    		  "libpymath", "pymath", "libmath", "progress bar", "progress"],
    install_requires=[],
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
    ],
)
