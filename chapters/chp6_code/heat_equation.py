import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D

def solve_heat_equation_1d(initial_condition, L, T, alpha=1.0, nx=100, nt=1000):
    """Solve 1D heat equation using finite differences"""
    dx = L / (nx - 1)
    dt = T / nt
    
    # Stability condition
    r = alpha * dt / dx**2
    if r > 0.5:
        print(f"Warning: r = {r:.3f} > 0.5, solution may be unstable")
    
    # Initialize grid
    x = np.linspace(0, L, nx)
    u = np.zeros((nt, nx))
    
    # Set initial condition
    u[0, :] = initial_condition(x)
    
    # Boundary conditions (fixed at zero)
    u[:, 0] = 0
    u[:, -1] = 0
    
    # Time stepping
    for n in range(0, nt - 1):
        for i in range(1, nx - 1):
            u[n + 1, i] = u[n, i] + r * (u[n, i + 1] - 2 * u[n, i] + u[n, i - 1])
    
    return x, u

def solve_wave_equation_1d(initial_pos, initial_vel, L, T, c=1.0, nx=100, nt=1000):
    """Solve 1D wave equation using finite differences"""
    dx = L / (nx - 1)
    dt = T / nt
    
    # CFL condition
    CFL = c * dt / dx
    if CFL > 1:
        print(f"Warning: CFL = {CFL:.3f} > 1, solution may be unstable")
    
    # Initialize grid
    x = np.linspace(0, L, nx)
    u = np.zeros((nt, nx))
    
    # Set initial conditions
    u[0, :] = initial_pos(x)
    u[1, :] = u[0, :] + dt * initial_vel(x)
    
    # Time stepping
    r2 = (c * dt / dx) ** 2
    for n in range(1, nt - 1):
        for i in range(1, nx - 1):
            u[n + 1, i] = 2 * u[n, i] - u[n - 1, i] + \
                          r2 * (u[n, i + 1] - 2 * u[n, i] + u[n, i - 1])
        
        # Boundary conditions (fixed ends)
        u[n + 1, 0] = 0
        u[n + 1, -1] = 0
    
    return x, u

def solve_laplace_equation_2d(boundary_conditions, nx=50, ny=50, max_iter=10000, tol=1e-5):
    """Solve 2D Laplace equation using iterative method"""
    # Initialize grid
    u = np.zeros((ny, nx))
    
    # Apply boundary conditions
    u[0, :] = boundary_conditions['bottom']
    u[-1, :] = boundary_conditions['top']
    u[:, 0] = boundary_conditions['left']
    u[:, -1] = boundary_conditions['right']
    
    # Iterative solver (Gauss-Seidel)
    for iteration in range(max_iter):
        u_old = u.copy()
        
        for i in range(1, ny - 1):
            for j in range(1, nx - 1):
                u[i, j] = 0.25 * (u[i+1, j] + u[i-1, j] + u[i, j+1] + u[i, j-1])
        
        # Check convergence
        if np.max(np.abs(u - u_old)) < tol:
            print(f"Converged after {iteration} iterations")
            break
    
    return u

# Example usage
if __name__ == "__main__":
    # 1D Heat equation
    L = 10.0  # Length of rod
    T = 2.0   # Total time
    
    # Initial temperature distribution
    initial_temp = lambda x: np.sin(np.pi * x / L)
    
    x, u_heat = solve_heat_equation_1d(initial_temp, L, T, alpha=0.5)
    
    # 1D Wave equation
    initial_position = lambda x: np.exp(-((x - L/2)**2))
    initial_velocity = lambda x: np.zeros_like(x)
    
    x_wave, u_wave = solve_wave_equation_1d(initial_position, initial_velocity, L, T, c=2.0)
    
    # 2D Laplace equation
    boundary = {
        'top': 100,
        'bottom': 0,
        'left': 0,
        'right': 0
    }
    u_laplace = solve_laplace_equation_2d(boundary)
    
    # Visualization
    fig = plt.figure(figsize=(15, 10))
    
    # Heat equation solution
    ax1 = fig.add_subplot(231)
    time_steps = [0, len(u_heat)//4, len(u_heat)//2, 3*len(u_heat)//4, -1]
    for t in time_steps:
        ax1.plot(x, u_heat[t, :], label=f't = {t*T/len(u_heat):.2f}')
    ax1.set_xlabel('Position')
    ax1.set_ylabel('Temperature')
    ax1.set_title('1D Heat Equation Evolution')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Heat equation spacetime
    ax2 = fig.add_subplot(232)
    T_grid, X_grid = np.meshgrid(np.linspace(0, T, len(u_heat)), x)
    im1 = ax2.contourf(X_grid, T_grid, u_heat.T, levels=20, cmap='hot')
    plt.colorbar(im1, ax=ax2, label='Temperature')
    ax2.set_xlabel('Position')
    ax2.set_ylabel('Time')
    ax2.set_title('Heat Equation Spacetime')
    
    # Wave equation solution
    ax3 = fig.add_subplot(233)
    for t in time_steps:
        ax3.plot(x_wave, u_wave[t, :], label=f't = {t*T/len(u_wave):.2f}')
    ax3.set_xlabel('Position')
    ax3.set_ylabel('Amplitude')
    ax3.set_title('1D Wave Equation Evolution')
    ax3.legend()
    ax3.grid(True, alpha=0.3)
    
    # Wave equation spacetime
    ax4 = fig.add_subplot(234)
    T_grid_wave, X_grid_wave = np.meshgrid(np.linspace(0, T, len(u_wave)), x_wave)
    im2 = ax4.contourf(X_grid_wave, T_grid_wave, u_wave.T, levels=20, cmap='seismic')
    plt.colorbar(im2, ax=ax4, label='Amplitude')
    ax4.set_xlabel('Position')
    ax4.set_ylabel('Time')
    ax4.set_title('Wave Equation Spacetime')
    
    # Laplace equation solution
    ax5 = fig.add_subplot(235)
    im3 = ax5.imshow(u_laplace, cmap='coolwarm', origin='lower')
    plt.colorbar(im3, ax=ax5, label='Potential')
    ax5.set_xlabel('X')
    ax5.set_ylabel('Y')
    ax5.set_title('2D Laplace Equation Solution')
    
    # 3D surface of Laplace solution
    ax6 = fig.add_subplot(236, projection='3d')
    X_lap, Y_lap = np.meshgrid(range(u_laplace.shape[1]), range(u_laplace.shape[0]))
    ax6.plot_surface(X_lap, Y_lap, u_laplace, cmap='coolwarm')
    ax6.set_xlabel('X')
    ax6.set_ylabel('Y')
    ax6.set_zlabel('Potential')
    ax6.set_title('Laplace Solution (3D)')
    
    plt.tight_layout()
    plt.show()