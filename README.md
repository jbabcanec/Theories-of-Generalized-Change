# Theories of Generalized Change: A Calculus III Textbook

## Overview

This repository contains a comprehensive Calculus III textbook exploring the theories of generalized change, complete with mathematical concepts, proofs, and extensive programming examples across multiple languages.

## Contents

### Textbook Structure

The textbook is organized into seven chapters, each building upon fundamental concepts of multivariable calculus and vector analysis:

1. **Chapter 1: Vectors and 3D Geometry**
   - Vector operations (dot and cross products)
   - 3D coordinate systems
   - Quadric surfaces and their visualizations

2. **Chapter 2: Vector Functions and Parametric Curves**
   - Parametric representations of curves
   - Velocity, acceleration, and curvature
   - Arc length and curve analysis

3. **Chapter 3: Partial Derivatives**
   - Functions of multiple variables
   - Gradient vectors and directional derivatives
   - Optimization and critical point analysis

4. **Chapter 4: Multiple Integration**
   - Double and triple integrals
   - Change of variables and coordinate systems
   - Applications to area, volume, and center of mass

5. **Chapter 5: Vector Calculus**
   - Vector fields and line integrals
   - Green's theorem and circulation
   - Divergence and curl operations

6. **Chapter 6: Differential Equations**
   - Ordinary differential equations
   - Numerical solution methods
   - Partial differential equations and heat transfer

7. **Chapter 7: Complex Analysis**
   - Complex functions and mappings
   - Domain coloring visualization
   - Riemann surfaces and branch cuts

### Programming Examples

Each chapter includes comprehensive code examples in **four programming languages**:

- **Python** - High-level mathematical computing with NumPy and visualization
- **C++** - Object-oriented implementations with performance optimization
- **C** - Low-level procedural programming for computational efficiency
- **Ruby** - Elegant scripting for mathematical exploration

#### Code Organization

```
chapters/
├── chp1_code/          # Vectors and 3D Geometry
├── chp2_code/          # Vector Functions
├── chp3_code/          # Partial Derivatives  
├── chp4_code/          # Multiple Integration
├── chp5_code/          # Vector Calculus
├── chp6_code/          # Differential Equations
└── chp7_code/          # Complex Analysis
```

Each code directory contains equivalent implementations across all four languages, allowing students and instructors to:
- Compare algorithmic approaches across programming paradigms
- Learn mathematical concepts through multiple computational perspectives
- Develop programming skills while studying advanced mathematics

### Key Features

#### Mathematical Rigor
- Complete mathematical proofs and derivations
- Rigorous treatment of limits, continuity, and differentiability
- Advanced topics in vector calculus and complex analysis

#### Computational Focus
- Numerical methods for solving differential equations
- Visualization techniques for complex mathematical objects
- Performance considerations across different programming languages

#### Educational Design
- Progressive difficulty building from basic concepts
- Extensive worked examples and applications
- Programming exercises that reinforce mathematical understanding

## File Types

- **`.tex`** - LaTeX source files for the main textbook
- **`.py`** - Python implementations with scientific computing libraries
- **`.cpp`** - C++ object-oriented mathematical programming
- **`.c`** - C procedural implementations for numerical computing
- **`.rb`** - Ruby scripts for mathematical exploration
- **`.csv`** - Data output files for visualization and analysis

## Educational Applications

This textbook and code collection is designed for:

### Students
- Advanced undergraduate calculus courses
- Graduate-level mathematical analysis
- Computational mathematics and scientific computing
- Cross-disciplinary studies combining mathematics and programming

### Instructors
- Course material for Calculus III and Vector Calculus
- Programming assignments across multiple languages
- Visualization examples for complex mathematical concepts
- Research applications in mathematical modeling

### Researchers
- Reference implementations of numerical algorithms
- Benchmarking across programming languages
- Foundation for advanced mathematical computing projects

## Technical Requirements

### For LaTeX Compilation
- LaTeX distribution (TeX Live, MiKTeX, or MacTeX)
- Standard mathematical packages (amsmath, amsfonts, etc.)

### For Code Execution

**Python:**
- Python 3.7+
- NumPy, Matplotlib, SciPy
- Jupyter notebooks (optional)

**C++:**
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2019+)
- Standard library with complex number support

**C:**
- C99 compatible compiler
- Math library support for complex numbers

**Ruby:**
- Ruby 2.7+
- Built-in complex number support

## Getting Started

1. **Clone the repository:**
   ```bash
   git clone https://github.com/jbabcanec/Theories-of-Generalized-Change.git
   cd Theories-of-Generalized-Change
   ```

2. **Compile the textbook:**
   ```bash
   # Navigate to appropriate chapter directory
   pdflatex your_chapter.tex
   ```

3. **Run code examples:**
   ```bash
   # Python
   python3 chapters/chp1_code/vector_operations.py
   
   # C++
   g++ -std=c++17 chapters/chp1_code/vector_operations.cpp -o vector_ops
   ./vector_ops
   
   # C
   gcc chapters/chp1_code/vector_operations.c -lm -o vector_ops_c
   ./vector_ops_c
   
   # Ruby
   ruby chapters/chp1_code/vector_operations.rb
   ```

## Contributing

This educational resource welcomes contributions from students, educators, and researchers. Please see the LICENSE file for usage restrictions and requirements.

## Mathematical Notation and Conventions

- Vectors are denoted in boldface: **v**, **F**
- Complex numbers use standard notation: z = x + iy
- Partial derivatives: ∂f/∂x, ∇f
- Vector operations: **a** · **b** (dot product), **a** × **b** (cross product)
- Integrals maintain standard mathematical notation

## Visualization and Graphics

The code examples generate data suitable for:
- 3D surface plotting and visualization
- Vector field representations
- Complex function domain coloring
- Parametric curve animations
- Heat equation simulations

Output files in CSV format can be imported into visualization tools like:
- Python: Matplotlib, Plotly, Mayavi
- R: ggplot2, plotly
- MATLAB: Built-in plotting functions
- Mathematica: Graphics3D, ComplexPlot

## Author

**Joseph Babcanec**

## Academic Integrity

This textbook and accompanying code are provided for educational purposes. Students should:
- Use these materials to enhance understanding of mathematical concepts
- Cite this work appropriately in academic submissions
- Follow their institution's academic integrity policies
- Develop independent solutions while using these examples as learning aids

---

*This work represents a comprehensive approach to teaching advanced calculus through the integration of mathematical theory and computational practice across multiple programming paradigms.*