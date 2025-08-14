import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def helix(t, a=1, b=0.5):
    """Generate a helix curve"""
    x = a * np.cos(t)
    y = a * np.sin(t)
    z = b * t
    return x, y, z

def velocity_acceleration(r_func, t, dt=1e-5):
    """Calculate velocity and acceleration vectors numerically"""
    r = r_func(t)
    r_plus = r_func(t + dt)
    r_minus = r_func(t - dt)
    
    # Velocity (first derivative)
    v = [(r_plus[i] - r_minus[i]) / (2 * dt) for i in range(3)]
    
    # Acceleration (second derivative)
    a = [(r_plus[i] - 2*r[i] + r_minus[i]) / (dt**2) for i in range(3)]
    
    return np.array(v), np.array(a)

def arc_length(r_func, t_start, t_end, num_points=1000):
    """Calculate arc length of a parametric curve"""
    t = np.linspace(t_start, t_end, num_points)
    dt = t[1] - t[0]
    
    length = 0
    for i in range(len(t) - 1):
        r1 = np.array(r_func(t[i]))
        r2 = np.array(r_func(t[i+1]))
        length += np.linalg.norm(r2 - r1)
    
    return length

# Example usage
if __name__ == "__main__":
    # Generate helix
    t = np.linspace(0, 4*np.pi, 1000)
    x, y, z = helix(t)
    
    # Calculate properties at a specific point
    t_point = np.pi
    r_point = helix(t_point)
    v, a = velocity_acceleration(helix, t_point)
    arc_len = arc_length(helix, 0, t_point)
    
    print(f"At t = π:")
    print(f"Position: ({r_point[0]:.3f}, {r_point[1]:.3f}, {r_point[2]:.3f})")
    print(f"Velocity: ({v[0]:.3f}, {v[1]:.3f}, {v[2]:.3f})")
    print(f"Speed: {np.linalg.norm(v):.3f}")
    print(f"Acceleration: ({a[0]:.3f}, {a[1]:.3f}, {a[2]:.3f})")
    print(f"Arc length from 0 to π: {arc_len:.3f}")
    
    # Visualization
    fig = plt.figure(figsize=(12, 5))
    
    # 3D curve
    ax1 = fig.add_subplot(121, projection='3d')
    ax1.plot(x, y, z, 'b-', linewidth=2)
    ax1.scatter(*r_point, color='r', s=100, label=f't = π')
    ax1.quiver(*r_point, *v, color='g', arrow_length_ratio=0.2, label='Velocity')
    ax1.quiver(*r_point, *a, color='orange', arrow_length_ratio=0.2, label='Acceleration')
    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_zlabel('Z')
    ax1.set_title('Helix with Velocity and Acceleration')
    ax1.legend()
    
    # 2D projections
    ax2 = fig.add_subplot(122)
    ax2.plot(t, x, 'r-', label='x(t)')
    ax2.plot(t, y, 'g-', label='y(t)')
    ax2.plot(t, z, 'b-', label='z(t)')
    ax2.axvline(x=t_point, color='k', linestyle='--', alpha=0.5)
    ax2.set_xlabel('t')
    ax2.set_ylabel('Component value')
    ax2.set_title('Parametric Components')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.show()