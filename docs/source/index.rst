Libpymath Documentation
***********************
Libpymath - a fast, general purpose Python math library

The Aim
=======
The aim of libpymath is to provide a simple and intuitive inerface to complex mathematical routines, while maintaining a fast and efficient operation. The module will allow you to create powerful matrices, progress bars, neural networks and more with only a few lines of code.

Licencing
---------
Libpymath is covered under an MIT license, so you can use all of the code as is for private, open source or commercial projects, as long as the copyright notice is retained at the top of any files used.

Installing the Package
======================
To install the package, simply open your CLI of choice and run ``pip install libpymath`` to install the package. Hopefully there will be wheels provided for your operating system and Python version, however none exist the code must be built from source. This only takes a few seconds, however you will need a C or C++ compiler installed on your system.

If you would like to upgrade to the lastest version of libpymath, you can run ``pip install libpymath --upgrade`` to upgrade an existing version installed on your system.

Another option for more experienced users is to clone the repository using the ``git clone https://github.com/Pencilcaseman/LibPyMath.git`` command in a directory of your choice. This will install the source code for you, allowing you to edit the code, add features, fix potential bugs (hopefully you won't find any) or simply see how it works.

Using the Package
=================
With libpymath installed, you can open a Python environment, such as IDLE, and start to write your first piece of code.

To import the module, add this to the top of your file::

        import libpymath

Libpymath is commonly imported as ``lpm``, which is shorter and easier to type. This change can be made easily::

        import libpymath as lpm

The libpymath import allows you to access all of the features of libpymath, such as the matrix library or the progress bar library. For information on how to use these features, see below.


Contents
========

.. toctree::
	:maxdepth: 2
	:glob:

	modules
	libpymath.matrix
	libpymath.progress


Indices and tables
******************

* :ref:`search`
