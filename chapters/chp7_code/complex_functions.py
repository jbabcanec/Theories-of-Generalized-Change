#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import hsv_to_rgb
import warnings
warnings.filterwarnings('ignore')

class ComplexFunction:
    def __init__(self, function):
        self.function = function
    
    def evaluate_grid(self, x_range, y_range, resolution=200):
        x = np.linspace(*x_range, resolution)
        y = np.linspace(*y_range, resolution)
        X, Y = np.meshgrid(x, y)
        Z = X + 1j * Y
        
        try:
            W = self.function(Z)
            return X, Y, W
        except:
            return X, Y, np.full_like(Z, np.nan)
    
    def find_zeros_and_poles(self, x_range, y_range, resolution=50):
        X, Y, W = self.evaluate_grid(x_range, y_range, resolution)
        
        magnitude = np.abs(W)
        zeros = []
        poles = []
        
        for i in range(resolution):
            for j in range(resolution):
                if magnitude[i, j] < 1e-6:
                    zeros.append(complex(X[i, j], Y[i, j]))
                elif magnitude[i, j] > 1e6:
                    poles.append(complex(X[i, j], Y[i, j]))
        
        return zeros, poles
    
    def save_analysis_data(self, x_range, y_range, filename, resolution=200):
        X, Y, W = self.evaluate_grid(x_range, y_range, resolution)
        
        with open(filename, 'w') as f:
            f.write("x,y,real_output,imag_output,magnitude,argument,hue,brightness\n")
            
            for i in range(resolution):
                for j in range(resolution):
                    x, y = X[i, j], Y[i, j]
                    w = W[i, j]
                    
                    if np.isfinite(w):
                        magnitude = abs(w)
                        argument = np.angle(w)
                        hue = (argument + np.pi) / (2 * np.pi)
                        brightness = np.log(1 + magnitude)
                        
                        f.write(f"{x},{y},{w.real},{w.imag},{magnitude},{argument},{hue},{brightness}\n")
        
        print(f"Complex function analysis saved to {filename}")

# Define various complex functions
def polynomial_functions():
    return {
        'z': lambda z: z,
        'z²': lambda z: z**2,
        'z³': lambda z: z**3,
        'z⁴': lambda z: z**4,
        'z² + 1': lambda z: z**2 + 1,
        'z² - 1': lambda z: z**2 - 1,
        'z³ - z': lambda z: z**3 - z
    }

def rational_functions():
    return {
        '1/z': lambda z: np.where(z != 0, 1/z, np.inf),
        '1/(z²+1)': lambda z: 1/(z**2 + 1),
        '(z-1)/(z+1)': lambda z: (z-1)/(z+1),
        'z/(z²+1)': lambda z: z/(z**2 + 1),
        '(z²-1)/(z²+1)': lambda z: (z**2-1)/(z**2+1)
    }

def transcendental_functions():
    return {
        'e^z': lambda z: np.exp(z),
        'sin(z)': lambda z: np.sin(z),
        'cos(z)': lambda z: np.cos(z),
        'log(z)': lambda z: np.log(z + 1e-10),
        'sinh(z)': lambda z: np.sinh(z),
        'cosh(z)': lambda z: np.cosh(z),
        'tan(z)': lambda z: np.tan(z)
    }

def special_functions():
    return {
        'z^(1/2)': lambda z: np.sqrt(z + 1e-10),
        'z^(1/3)': lambda z: np.power(z + 1e-10, 1/3),
        'e^(1/z)': lambda z: np.exp(np.where(z != 0, 1/z, np.inf)),
        'sin(1/z)': lambda z: np.sin(np.where(z != 0, 1/z, np.inf)),
        'z*e^z': lambda z: z * np.exp(z),
        'z*sin(z)': lambda z: z * np.sin(z)
    }

if __name__ == "__main__":
    print("Complex Functions Analysis (Python version)")
    print("===========================================")
    
    # Analyze different categories of functions
    all_functions = {
        **polynomial_functions(),
        **rational_functions(), 
        **transcendental_functions(),
        **special_functions()
    }
    
    # Generate analysis for selected functions
    selected_functions = ['z²', '1/z', 'e^z', 'sin(z)', '(z-1)/(z+1)', 'e^(1/z)']
    
    for func_name in selected_functions:
        if func_name in all_functions:
            print(f"Analyzing {func_name}...")
            cf = ComplexFunction(all_functions[func_name])
            
            # Choose appropriate domain for each function
            if func_name in ['e^z']:
                x_range, y_range = (-2, 2), (-6, 6)
            elif func_name in ['sin(z)', 'cos(z)', 'tan(z)']:
                x_range, y_range = (-4, 4), (-4, 4)
            elif func_name in ['e^(1/z)', 'sin(1/z)']:
                x_range, y_range = (-1, 1), (-1, 1)
            else:
                x_range, y_range = (-3, 3), (-3, 3)
            
            # Save analysis
            safe_name = func_name.replace('/', '_div_').replace('^', '_pow_').replace('(', '').replace(')', '')
            filename = f"complex_function_{safe_name}_python.csv"
            cf.save_analysis_data(x_range, y_range, filename, resolution=150)
            
            # Find critical points
            zeros, poles = cf.find_zeros_and_poles(x_range, y_range, resolution=30)
            
            if zeros:
                print(f"  Zeros found: {len(zeros)}")
                for i, zero in enumerate(zeros[:5]):  # Show first 5
                    print(f"    {zero:.3f}")
            
            if poles:
                print(f"  Poles found: {len(poles)}")
                for i, pole in enumerate(poles[:5]):  # Show first 5
                    print(f"    {pole:.3f}")
    
    print("\nFunction properties summary:")
    print("• Polynomial functions: Entire (analytic everywhere)")
    print("• Rational functions: Meromorphic (poles at zeros of denominator)")
    print("• Exponential functions: Entire, periodic in imaginary direction")
    print("• Trigonometric functions: Meromorphic, periodic")
    print("• Logarithmic functions: Multi-valued, branch cuts")
    print("• Power functions: Multi-valued for non-integer powers")
    
    print(f"\nGenerated analysis files for {len(selected_functions)} complex functions.")
    print("Files can be visualized using domain coloring techniques.")