import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def vector_projection(a, b):
    """Project vector a onto vector b"""
    return (np.dot(a, b) / np.dot(b, b)) * b

def angle_between_vectors(a, b):
    """Calculate angle between two vectors in radians"""
    cos_angle = np.dot(a, b) / (np.linalg.norm(a) * np.linalg.norm(b))
    return np.arccos(np.clip(cos_angle, -1.0, 1.0))

def triple_scalar_product(a, b, c):
    """Calculate the scalar triple product a · (b × c)"""
    return np.dot(a, np.cross(b, c))

# Example usage
if __name__ == "__main__":
    # Define three vectors
    v1 = np.array([1, 2, 3])
    v2 = np.array([4, -1, 2])
    v3 = np.array([2, 3, -1])
    
    # Calculate various operations
    proj = vector_projection(v1, v2)
    angle = angle_between_vectors(v1, v2)
    triple = triple_scalar_product(v1, v2, v3)
    
    print(f"Vector v1: {v1}")
    print(f"Vector v2: {v2}")
    print(f"Projection of v1 onto v2: {proj}")
    print(f"Angle between v1 and v2: {np.degrees(angle):.2f} degrees")
    print(f"Triple scalar product: {triple}")
    print(f"Volume of parallelepiped: {abs(triple)}")
    
    # Visualize vectors
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    
    # Plot vectors from origin
    ax.quiver(0, 0, 0, v1[0], v1[1], v1[2], color='r', arrow_length_ratio=0.1, label='v1')
    ax.quiver(0, 0, 0, v2[0], v2[1], v2[2], color='b', arrow_length_ratio=0.1, label='v2')
    ax.quiver(0, 0, 0, proj[0], proj[1], proj[2], color='g', arrow_length_ratio=0.1, label='proj(v1 onto v2)')
    
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.legend()
    ax.set_title('Vector Operations in 3D')
    plt.show()