import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Define the vector field components
def F_x(x, y, z):
    return y**2

def F_y(x, y, z):
    return 2 * x * y

def F_z(x, y, z):
    return x + z

# Create the grid
# Adjust the range and density of the grid as needed
x, y, z = np.meshgrid(
    np.linspace(-2, 2, 5),  # x-axis
    np.linspace(-2, 2, 5),  # y-axis
    np.linspace(-2, 2, 5)   # z-axis
)

# Compute the vector components at each grid point
u = F_x(x, y, z)
v = F_y(x, y, z)
w = F_z(x, y, z)

# Compute the magnitude of each vector
magnitude = np.sqrt(u**2 + v**2 + w**2)

# Avoid division by zero for normalization
# Replace zeros with ones to keep the direction unchanged
magnitude[magnitude == 0] = 1

# Normalize the vectors to have unit length
u_normalized = u / magnitude
v_normalized = v / magnitude
w_normalized = w / magnitude

# Initialize the plot
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Plot the vector field using quiver
ax.quiver(
    x, y, z,           # Starting points of the vectors
    u_normalized,     # X-components of the vectors
    v_normalized,     # Y-components of the vectors
    w_normalized,     # Z-components of the vectors
    length=0.5,        # Length of the arrows
    normalize=False,   # Already normalized
    color='blue',
    linewidth=0.5
)

# Set the limits of the axes
ax.set_xlim([-2, 2])
ax.set_ylim([-2, 2])
ax.set_zlim([-2, 2])

# Label the axes
ax.set_xlabel('X', fontsize=20, labelpad=10)
ax.set_ylabel('Y', fontsize=20, labelpad=10)
ax.set_zlabel('Z', fontsize=20, labelpad=10)

# Show grid
ax.grid(True)

# Show the plot
plt.show()
