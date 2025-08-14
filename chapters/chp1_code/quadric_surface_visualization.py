import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Define the function for the quadric surface
def quadric_surface(x, y, A, B, C, D, E, F, G, H, I, J):
    return A*x**2 + B*y**2 + C*(x*y) + D*x + E*y + F  # Replace or expand as needed

# Coefficients (students can modify these to observe changes)
A, B, C = 1, 1, 0
D, E, F = 0, 0, -1

# Create a meshgrid for the surface
x = np.linspace(-2, 2, 100)
y = np.linspace(-2, 2, 100)
x, y = np.meshgrid(x, y)

# Compute z-values for the surface
z = quadric_surface(x, y, A, B, C, D, E, F, 0, 0, 0, 0)

# Plot the surface
fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x, y, z, cmap="viridis", alpha=0.8)

# Labels and title
ax.set_xlabel('X-axis')
ax.set_ylabel('Y-axis')
ax.set_zlabel('Z-axis')
ax.set_title('Quadric Surface Visualization')

plt.show()