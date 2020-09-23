
import sys
import os
from libpymath.core import matrix as _matrix
from time import time
import math
import shutil

LPM_CORES = os.cpu_count()

# Find the optimal number of threads to use
def _lpmFindOptimalMatrixThreads(matSize=1000, n=1000, verbose=False):
    termWidth = shutil.get_terminal_size(fallback=(120, 50)).columns

    fastTime = 99999999999999
    fastThreads = 0

    for i in range(1, LPM_CORES + 1):
        # Create a matrix
        mat = _matrix.Matrix(matSize, matSize)

        dt = 0
        maxTime = 0
        minTime = 99999999999999
        for j in range(n):
            start = time()
            res = mat.matrixAddMatrixReturn(mat, i)
            end = time()
            t = end - start

            if t < minTime:
                minTime = t

            if t > maxTime:
                maxTime = t

            dt += t

            progLen = termWidth - 31
            inc = math.ceil(n / progLen) # n // progLen

            if verbose and j % 10 == 0:
                print("Testing {} thread(s)   [{}{}]\r".format(str(i).rjust(5), "#" * math.ceil(j / inc), " " * (math.ceil(n / inc) - math.ceil(j / inc))), end="")
                sys.stdout.flush()

        if dt < fastTime:
            fastTime = dt
            fastThreads = i

        if not verbose:
            print("Tested {} cores".format(i))

    if verbose:
        print("", end=" " * (termWidth - 5) + "\r")

    return fastThreads

write = False
try:
    with open("{}/_threadInfo.py".format(os.path.dirname(os.path.realpath(__file__))), "r") as f:
        if f.read() == "UNINITIALIZED":
            write = True
except FileNotFoundError:
    write = True

# If running in idle don't use verbose output. It takes *FOREVER* due to the printing speed
verbose = "idlelib.run" not in sys.modules

if write:
    LPM_OPTIMAL_MATRIX_THREADS = _lpmFindOptimalMatrixThreads(matSize=1000, n=250, verbose=verbose)

    with open("{}/_threadInfo.py".format(os.path.dirname(os.path.realpath(__file__))), "w") as f:
        f.write("LPM_CORES = {}\n".format(LPM_CORES))
        f.write("LPM_OPTIMAL_MATRIX_THREADS = {}\n".format(LPM_OPTIMAL_MATRIX_THREADS))
