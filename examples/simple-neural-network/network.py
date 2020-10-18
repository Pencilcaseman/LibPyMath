import libpymath as lpm


class Network:
    def __init__(self, **kwargs):
        self.nodeCounts = kwargs["layers"]
        self.activations = kwargs["activations"] if "activations" in kwargs else [lpm.matrix.RELU for _ in range(len(self.nodeCounts) - 1)]

        self.layers = []
        self.biases = []

        self.learningRate = kwargs["lr"] if "lr" in kwargs else 0.01

        for i in range(len(self.nodeCounts) - 1):
            self.layers.append(lpm.matrix.Matrix(self.nodeCounts[i + 1], self.nodeCounts[i]))
            self.biases.append(lpm.matrix.Matrix(self.nodeCounts[i + 1]))

            self.layers[-1].fillRandom()
            self.biases[-1].fillRandom()

    def forwardPass(self, data):
        current = lpm.matrix.Matrix(data=data).T

        for i in range(len(self.nodeCounts) - 1):
            current = self.layers[i] @ current
            current += self.biases[i]
            current.map(self.activations[i])

        return current

    def backpropagate(self, i, o):
        inputs = lpm.matrix.Matrix(data=i).T
        targets = lpm.matrix.Matrix(data=o).T

        layerData = []
        errors = [None for _ in range(len(self.nodeCounts) - 1)]

        current = inputs.copy()
        for i in range(len(self.nodeCounts) - 1):
            current = self.layers[i] @ current
            current += self.biases[i]
            current.map(self.activations[i])

            layerData.append(current.copy())

        errors[len(self.nodeCounts) - 2] = targets - layerData[-1]

        for i in range(len(self.nodeCounts) - 2, -1, -1):
            gradient = layerData[i].mapped(self.activations[i] << 5)
            gradient *= errors[i]
            gradient *= self.learningRate

            if i > 0:
                transposed = layerData[i - 1].T
            else:
                transposed = inputs.T

            weight_deltas = gradient @ transposed

            self.layers[i] += weight_deltas
            self.biases[i] += gradient

            if i > 0:
                layerT = self.layers[i].T
                errors[i - 1] = layerT @ errors[i]
