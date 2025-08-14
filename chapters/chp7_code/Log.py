import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def plot_log_riemann_surface():
    # Parameter ranges: theta covers many windings (branches); r > 0 avoids the singularity at 0.
    theta = np.linspace(-8 * np.pi, 8 * np.pi, 400)  # many branches
    r = np.linspace(0.1, 2, 200)
    Theta, R = np.meshgrid(theta, r)
    
    # Parametrization of the Riemann surface of log(z) (a helicoid):
    # Here, z = r exp(i theta) and log(z) = ln r + i theta.
    # We use (R*cos(theta), R*sin(theta)) to represent the z-plane projection,
    # and theta as the vertical coordinate to represent the branch structure.
    X = R * np.cos(Theta)
    Y = R * np.sin(Theta)
    Z = Theta

    # Create the 3D plot
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(X, Y, Z, cmap='viridis', edgecolor='none', alpha=0.9)
    
    ax.set_xlabel(r'$\Re(z)$')
    ax.set_ylabel(r'$\Im(z)$')
    ax.set_zlabel('Argument (Branch)')
    ax.set_title('Riemann Surface of $\\log(z)$')
    fig.colorbar(surf, shrink=0.5, aspect=10, label=r'$\theta$')
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    plot_log_riemann_surface()
