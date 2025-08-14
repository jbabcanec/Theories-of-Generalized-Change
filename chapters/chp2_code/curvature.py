import numpy as np
import matplotlib.pyplot as plt

def plot_function_and_curvature(func, x_range, scaling_factor=1):
    # Generate x values
    x = np.linspace(x_range[0], x_range[1], 1000)
    dx = x[1] - x[0]  # Assuming uniform spacing

    # Compute y = f(x)
    y = func(x)

    # First derivative f'(x) using numerical differentiation
    f_prime = np.gradient(y, dx)

    # Second derivative f''(x)
    f_double_prime = np.gradient(f_prime, dx)

    # Compute curvature k = |f''(x)| / (1 + [f'(x)]^2)^(3/2)
    curvature = np.abs(f_double_prime) / (1 + f_prime**2)**1.5
    curvature *= scaling_factor  # Apply scaling if needed

    # Plotting
    fig, ax1 = plt.subplots(figsize=(10, 6))

    # Plot the function on the left y-axis
    color1 = 'tab:blue'
    ax1.set_xlabel('x')
    ax1.set_ylabel('Function', color=color1)
    ax1.plot(x, y, label='Function', color=color1, linewidth=2)
    ax1.tick_params(axis='y', labelcolor=color1)
    ax1.grid(True)

    # Create a second y-axis for the curvature
    ax2 = ax1.twinx()

    color2 = 'tab:red'
    ax2.set_ylabel('Curvature', color=color2)
    ax2.plot(x, curvature, label='Curvature', color=color2, linestyle='--', linewidth=2)
    ax2.tick_params(axis='y', labelcolor=color2)

    # Combine legends
    lines_1, labels_1 = ax1.get_legend_handles_labels()
    lines_2, labels_2 = ax2.get_legend_handles_labels()
    ax1.legend(lines_1 + lines_2, labels_1 + labels_2, loc='upper right')

    plt.title('Function and Curvature')
    plt.show()

# Example usage with a polynomial function
def polynomial_function(x):
    return x**3 - 3*x**2 + x - 2

plot_function_and_curvature(polynomial_function, x_range=(-1, 4))