import libpymath as lpm
from network import Network
from random import randint
import matplotlib.pyplot as plt

# Training data
train = [
    [0, 0, 0],
    [0, 1, 1],
    [1, 0, 1],
    [1, 1, 0]
]

x = Network(layers=(2, 4, 1), lr=0.1, activations=[lpm.matrix.LEAKY_RELU] * 3)

epochs = int(input("Number of training iterations >> "))

print("Before training")
for t in train:
    print(bool(t[0]), "^", bool(t[1]), "=", x.forwardPass(t[:-1])[0, 0] > 0)

loss = []
acc = []

# Training loop
for i in lpm.progress.Progress(range(epochs)):
    pos = randint(0, 3)
    x.backpropagate(train[pos][:-1], [train[pos][-1]])

    l = 0
    a = 0
    for t in train:
        l += (x.forwardPass(t[:-1])[0, 0] - t[-1])
        a += (abs(int(x.forwardPass(t[:-1])[0, 0] > 0.25) - t[-1]))
    loss.append((l / 4) ** 2)
    acc.append((1 - a / 4) * 100)

print("After training")
for t in train:
    print(bool(t[0]), "^", bool(t[1]), "=", x.forwardPass(t[:-1])[0, 0] > 0)

# Plot the loss and the accuracy
fig, axs = plt.subplots(2, 1, constrained_layout=True)
axs[0].plot([i for i in range(epochs)], loss)
axs[0].set_title("Loss vs Epoch")
axs[0].set_xlabel("Epoch")
axs[0].set_ylabel("Mean squared error")

axs[1].plot([i for i in range(epochs)], acc, color="green")
axs[1].set_title("Accuracy vs Epoch")
axs[1].set_xlabel("Epoch")
axs[1].set_ylabel("Accuracy")

plt.show()
