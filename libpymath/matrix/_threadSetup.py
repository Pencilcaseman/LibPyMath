
import sys
import os
from libpymath.core import matrix as _matrix
from time import time
import math

# Min 4 cores for thread test purposes -- speed may increase with more cores
LPM_CORES = min(os.cpu_count(), 4)
LPM_THREADS = 4
try:
    if sys.platform == 'win32':
        LPM_THREADS = (int)(os.environ['NUMBER_OF_PROCESSORS'])
    else:
        LPM_THREADS = (int)(os.popen('grep -c cores /proc/cpuinfo').read())
except:
    print("Number of threads available unknown. Assuming 4")

LPM_IS_HYPERTHREADING = LPM_CORES != LPM_THREADS

# Find the optimal number of threads to use
def _lpmFindOptimalMatrixThreads(n = 1000, verbose=False):
    termWidth = os.get_terminal_size().columns

    fastTime = 99999999999999
    fastThreads = 0

    for i in range(1, LPM_THREADS + 1):
        # Create a matrix
        mat = _matrix.Matrix(1000, 1000)

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

            if verbose:
                print("Testing thread {}   [{}{}]\r".format(str(i).rjust(5), "#" * math.ceil(j / inc), " " * (math.ceil(n / inc) - math.ceil(j / inc))), end="")
                sys.stdout.flush()

        if dt < fastTime:
            fastTime = dt
            fastThreads = i

    if verbose:
        print(" ".ljust(termWidth - 5), end="")

    return fastThreads

LPM_OPTIMAL_MATRIX_THREADS = _lpmFindOptimalMatrixThreads(n=500, verbose=True)

with open("_threadInfo.py", "w") as out:
    out.write("LPM_CORES = {}\n".format(LPM_CORES))
    out.write("LPM_THREADS = {}\n".format(LPM_THREADS))
    out.write("LPM_IS_HYPERTHREADING = {}\n".format(LPM_IS_HYPERTHREADING))
    out.write("LPM_OPTIMAL_MATRIX_THREADS = {}\n".format(LPM_OPTIMAL_MATRIX_THREADS))
