import numpy as np
import matplotlib.pyplot as plt

# Define the De Jong attractor equations
def de_jong_attractor(x, y, a, b, c, d):
    x_new = np.sin(a * y) - np.cos(b * x)
    y_new = np.sin(c * x) - np.cos(d * y)
    return x_new, y_new

# Parameters for the attractor (you can tweak these for different patterns)
a, b, c, d = -2.0, -2.0, 1.5, -2.5

# Generate points
num_points = 1000000  # Dense set of points for smooth patterns
x, y = 0.1, 0.1  # Initial condition
trajectory = np.zeros((num_points, 2))

for i in range(num_points):
    x, y = de_jong_attractor(x, y, a, b, c, d)
    trajectory[i] = [x, y]

# Extract x and y coordinates
x_vals, y_vals = trajectory[:, 0], trajectory[:, 1]

# Plot the attractor
plt.figure(figsize=(12, 12), dpi=300)
plt.scatter(x_vals, y_vals, s=0.1, color='black', alpha=0.1)  # Adjust alpha for smooth transitions
plt.axis("off")  # Remove axes for a minimalist aesthetic
plt.gca().set_aspect("equal", adjustable="box")

# Save as a high-quality PNG
plt.savefig("dejong_attractor.png", bbox_inches="tight", pad_inches=0, transparent=False)
plt.close()

print("The De Jong attractor art has been saved as 'dejong_attractor.png'")
