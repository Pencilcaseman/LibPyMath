from libpymath.network import Network, TANH, SIGMOID, RELU, LEAKY_RELU

# Training data
data = [
    [0, 0, 0],
    [0, 1, 1],
    [1, 0, 1],
    [1, 1, 0]
]

# Number of training iterations to complete
epochs = int(input("Number of training iterations >> "))

# Create the network with two inputs, one hidden layer of three
# neurons and one output, a learning rate of 0.2 and the sigmoid
# activation for all layers
x = Network(layers=(2, 3, 1), lr=0.2, activations=[SIGMOID] * 3)

# Record the loss of the network every 10 training iterations
x.log("loss", 10)

# Fit the network to the data and show a progress bar
x.fit(data, epochs=epochs, progress=1)

# Print the network's output for all of the input data
print("Result of network after training:")
for val in data:
    print(bool(val[0]), "^", bool(val[1]), "=", x.feedForward(val[:2])[0, 0] > 0.5)

# Plot a graph of the network's loss
x.plotMetric("loss")
