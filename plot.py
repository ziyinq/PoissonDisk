import numpy as np

import matplotlib.pyplot as plt

with open("points.txt") as f:
    lines = f.readlines()
    x = [line.split()[0] for line in lines]
    y = [line.split()[1] for line in lines]
plt.scatter(x,y)
ax = plt.gca()
ax.set_xticks(np.arange(0, 11, 1))
ax.set_yticks(np.arange(0, 11, 1))
plt.grid()
plt.show()

