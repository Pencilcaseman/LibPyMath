import libpymath as lpm
import random


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


class Network:
    def __init__(self, **kwargs):
        if "layers" in kwargs:
            if isinstance(kwargs["layers"], (list, tuple)):
                self._nodeCounts = kwargs["layers"]

                # Check everything is an integer
                for n in self._nodeCounts:
                    if not isinstance(n, int):
                        raise TypeError("\"layers\" must be a list or tuple of integers. Found {}", type(n))
            else:
                raise TypeError("\"layers\" must be defined as a list or tuple of integers")
        else:
            raise TypeError("Missing required argument \"layers\"")

        self._activations = kwargs["activations"] if "activations" in kwargs else [lpm.matrix.RELU for _ in range(len(self._nodeCounts) - 1)]

        # Check activations
        for a in self._activations:
            if a not in [SIGMOID, TANH, RELU, LEAKY_RELU, lpm.matrix.SIGMOID, lpm.matrix.TANH, lpm.matrix.RELU, lpm.matrix.LEAKY_RELU]:
                raise NotImplementedError("The activation function {} is not implemented. If you would like this to be added please raise an issue on GitHub: https://github.com/Pencilcaseman/LibPyMath/issues".format(a))

        self._layers = []
        self._biases = []

        if "lr" in kwargs:
            if isinstance(kwargs["lr"], (int, float)):
                self._learningRate = float(kwargs["lr"])
            else:
                raise TypeError("\"lr\" must be an int or a float")
        else:
            self._learningRate = 0.025

        for i in range(len(self._nodeCounts) - 1):
            self._layers.append(lpm.matrix.Matrix(self._nodeCounts[i + 1], self._nodeCounts[i]))
            self._biases.append(lpm.matrix.Matrix(self._nodeCounts[i + 1]))

            self._layers[-1].fillRandom()
            self._biases[-1].fillRandom()

        self._metrics = {
            "loss": [[], False]
        }

        self._metricMod = {
            "loss": 1
        }

        self.backpropagateIndex = 0

    @property
    def layers(self):
        return self._layers.copy()

    @property
    def biases(self):
        return self._biases.copy()

    @property
    def learningRate(self):
        return self._learningRate

    @property
    def shape(self):
        return self._nodeCounts.copy()

    def parseData(self, dataIn, dataOut=None):
        res = []

        if isinstance(dataIn[0], (list, tuple)):
            # Check for a single list containing input + output
            if dataOut is None:
                for element in dataIn:
                    if len(element) == self._nodeCounts[0] + self._nodeCounts[-1]:
                        # Contains input and output data in a single list
                        res.append((self.__parseData(element[:self._nodeCounts[0]]), self.__parseData(element[self._nodeCounts[0]:], -1)))
                    else:
                        raise ValueError("Invalid input data. Expected input data of length {} but got length {}".format(self._nodeCounts[0] + self._nodeCounts[-1], len(element)))
            else:
                if len(dataIn) != len(dataOut):
                    raise ValueError("Input and output data lengths must be equal")

                for a, b in zip(dataIn, dataOut):
                    res.append((self.__parseData(a), self.__parseData(b, -1)))
        else:
            raise NotImplementedError("Cannot yet parse 1 dimensional training data")

        return res

    def __parseData(self, data, layer=0):
        # Check that data is valid and return a valid matrix if possible, else raise an error
        if isinstance(data, (int, float)):
            if self._nodeCounts[0] == 1:
                # Single bit of data so create [[x]]
                return lpm.matrix.Matrix(data=[[data]]).T
            else:
                raise ValueError("Only one value was passed, though input requires {}".format(self._nodeCounts[0]))
        elif isinstance(data, (lpm.matrix.Matrix, list, tuple)):
            # Check if data will fit currently
            if isinstance(data, lpm.matrix.Matrix):
                tmp = data
            else:
                tmp = lpm.matrix.Matrix(data=data)

            if tmp.rows == self._nodeCounts[layer] and tmp.cols == 1:
                return tmp
            elif tmp.cols == self._nodeCounts[layer] and tmp.rows == 1:
                return tmp.T
            else:
                if tmp.rows == 1:
                    val = tmp.cols
                elif tmp.cols == 1:
                    val = tmp.rows
                else:
                    val = None

                raise ValueError("Inputted data is invalid for network of this size. Input requires {} values, received {}".format(self._nodeCounts[0], val if val is not None else "{}x{}".format(tmp.rows, tmp.cols)))
        else:
            raise TypeError("Invalid type for neural network. Requires list, tuple{}".format(", int or float" if self._nodeCounts[0] == 1 else ""))

    def feedForward(self, data, **kwargs):
        # For improved speed when one is sure that the data is correct
        if "noCheck" in kwargs:
            current = data.copy()
        else:
            current = self.__parseData(data)

        for i in range(len(self._nodeCounts) - 1):
            current = self._layers[i] @ current
            current += self._biases[i]
            current.map(self._activations[i])

        return current

    def backpropagate(self, inputData, targetData, **kwargs):
        # For improved speed when one is sure that the data is correct
        if "noCheck" in kwargs:
            inputs = inputData.copy()
            targets = targetData.copy()
        else:
            inputs = self.__parseData(inputData)
            targets = self.__parseData(targetData, layer=-1)

        layerData = []
        errors = [None for _ in range(len(self._nodeCounts) - 1)]

        current = inputs.copy()
        for i in range(len(self._nodeCounts) - 1):
            current = self._layers[i] @ current
            current += self._biases[i]
            current.map(self._activations[i])

            layerData.append(current.copy())

        errors[-1] = targets - layerData[-1]

        if self._metrics["loss"][1] and self.backpropagateIndex % self._metricMod["loss"] == 0:
            self._metrics["loss"][0].append(errors[-1].mean() ** 2)

        self.backpropagateIndex += 1

        for i in range(len(self._nodeCounts) - 2, -1, -1):
            gradient = layerData[i].mapped(self._activations[i] << 5)
            gradient *= errors[i]
            gradient *= self._learningRate

            if i > 0:
                transposed = layerData[i - 1].T
            else:
                transposed = inputs.T

            weight_deltas = gradient @ transposed

            self._layers[i] += weight_deltas
            self._biases[i] += gradient

            if i > 0:
                layerT = self._layers[i].T
                errors[i - 1] = layerT @ errors[i]

    def log(self, metric, mod=1):
        if metric == "loss" and self._metrics["loss"] != [None]:
            self._metrics["loss"][1] = True
            self._metricMod["loss"] = mod

    def metrics(self, metric):
        if metric not in self._metrics:
            raise KeyError("Metric {} does not exist".format(metric))
        if not self._metrics[metric][1]:
            raise KeyError("Metric {} is not being logged".format(metric))

        return self._metrics[metric][0]

    def plotMetric(self, metric):
        if metric not in self._metrics:
            raise KeyError("Metric {} does not exist".format(metric))
        if not self._metrics[metric][1]:
            raise KeyError("Metric {} is not being logged".format(metric))

        x = [i * self._metricMod[metric] for i in range(len(self.metrics(metric)))]
        y = self._metrics[metric][0]

        try:
            import matplotlib.pyplot as plt

            fig, axs = plt.figure(), plt.axes()
            fig.canvas.set_window_title("Libpymath Network {}".format(metric.title()))

            axs.plot(x, y)
            axs.set_title("{} vs Epoch".format(metric.title()))
            axs.set_xlabel("Epoch")
            axs.set_ylabel("{}".format(metric.title()))

            plt.show()
        except ImportError:
            raise ModuleNotFoundError("The module matplotlib.pyplot was not found, please install this via pip")

    def metricData(self, metric):
        if metric not in self._metrics:
            raise KeyError("Metric {} does not exist".format(metric))
        if not self._metrics[metric][1]:
            raise KeyError("Metric {} is not being logged".format(metric))

        return [i * self._metricMod[metric] for i in range(len(self.metrics(metric)))], self._metrics[metric][0]

    def fit(self, inputData, targetData=None, epochs=500, **kwargs):
        data = self.parseData(inputData, targetData)
        samples = len(data)

        if "progress" in kwargs and kwargs["progress"] not in [0, False]:
            iterator = lpm.progress.Progress(range(epochs))
        else:
            iterator = range(epochs)

        for _ in iterator:
            pos = random.randint(0, samples - 1)
            self.backpropagate(data[pos][0], data[pos][1], noCheck=True)
