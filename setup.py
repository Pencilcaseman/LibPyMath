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

try:
    from setuptools import setup, Extension, find_packages
except ImportError:
    from distutils.core import setup, Extension

    def find_packages(where='.'):
        return [folder.replace("/", ".").lstrip(".")
                for (folder, _, fils) in os.walk(where)
                if "__init__.py" in fils]

this_directory = Path(__file__).parent
long_description = (this_directory / 'README.md').read_text(encoding='utf-8')
compiler_flags = ["-std=c99"]

ext_modules = [
    Extension(
        "libpymath.core.testModule",
        ["libpymath/LibPyMathModules/testModule.c"],
        extra_compile_args=compiler_flags
    ),
    Extension(
        "libpymath.core.matrix",
        ["libpymath/LibPyMathModules/matrixModule.c"],
        extra_compile_args=compiler_flags
    )
]

setup(
    name="libpymath",
    version="0.0.13",
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
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        'Operating System :: MacOS',
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: Microsoft',
        'Operating System :: Microsoft :: MS-DOS',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX :: BSD',
        'Operating System :: POSIX :: BSD :: FreeBSD',
        'Operating System :: POSIX :: Linux',
        'Operating System :: Unix',
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
    ],
)
