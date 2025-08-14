import numpy as np

# Feature vectors for two products: [popularity, weight (kg), price (dollars)]
p = np.array([8, 2, 15])
q = np.array([5, 3, 12])

# Calculate the dot product
dot_product = np.dot(p, q)
print(f"Dot Product (Similarity Score): {dot_product}")

# Calculate the cross product
cross_product = np.cross(p, q)
print(f"Cross Product (Directional Difference): {cross_product}")

# Use dot product as a basic similarity measure for recommendation
similarity_threshold = 200  # Set an arbitrary threshold
if dot_product > similarity_threshold:
    print("Products are similar enough to recommend together.")
else:
    print("Products are distinct enough to consider separately.")

# Analyze uniqueness with cross product
if np.linalg.norm(cross_product) > 0:
    print("Products have unique directional attributes across feature space.")
else:
    print("Products are very similar across all feature dimensions.")