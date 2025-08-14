import numpy as np
import matplotlib.pyplot as plt

# Define the functions P(x, y) and Q(x, y)
def P(x, y):
    return -y  # Example: -y

def Q(x, y):
    return x  # Example: x

# Create the grid
x, y = np.meshgrid(np.linspace(-10, 10, 25), np.linspace(-10, 10, 25))

# Compute the vector components
u = P(x, y)
v = Q(x, y)

# Normalize vectors to minimize arrow crossings
magnitude = np.sqrt(u**2 + v**2)
u = u / magnitude
v = v / magnitude

# Plot the vector field
plt.quiver(x, y, u, v, angles='xy', scale_units='xy', scale=1, color='blue', width=0.0025)
plt.xlim(-10, 10)
plt.ylim(-10, 10)
plt.xlabel('x')
plt.ylabel('y')
plt.title('2D Vector Field (Minimized Arrow Crossing)')
plt.grid()
plt.show()
