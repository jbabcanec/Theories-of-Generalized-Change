import numpy as np
import matplotlib.pyplot as plt

def gradient_descent(func, grad_func, initial_x, learning_rate, num_iterations):
    """
    Performs gradient descent optimization.

    Parameters:
    - func: The function to minimize.
    - grad_func: The derivative (gradient) of the function.
    - initial_x: The starting point for optimization.
    - learning_rate: The step size multiplier.
    - num_iterations: The number of iterations to perform.

    Returns:
    - x_history: A list of x values representing the path of optimization.
    """
    x = initial_x
    x_history = [x]

    for i in range(num_iterations):
        grad = grad_func(x)        # Compute the gradient at current x
        x = x - learning_rate * grad  # Update x in the opposite direction of the gradient
        x_history.append(x)        # Record the new x value

        # Optional: Print progress every 10 iterations
        if (i+1) % 10 == 0:
            print(f"Iteration {i+1}: x = {x:.4f}, f(x) = {func(x):.4f}")

    return x_history

# Example usage with the function f(x) = x^2
def f(x):
    return x**2

def grad_f(x):
    return 2*x

# Parameters for gradient descent
initial_x = 10        # Starting point
learning_rate = 0.1   # Step size
num_iterations = 50   # Number of iterations

# Perform gradient descent
x_history = gradient_descent(f, grad_f, initial_x, learning_rate, num_iterations)

# Plotting the optimization path
iterations = range(len(x_history))
function_values = [f(x) for x in x_history]

plt.figure(figsize=(10, 6))
plt.plot(iterations, function_values, marker='o', linestyle='-', color='b')
plt.title('Gradient Descent Optimization Path')
plt.xlabel('Iteration')
plt.ylabel('f(x) = x²')
plt.grid(True)
plt.show()
