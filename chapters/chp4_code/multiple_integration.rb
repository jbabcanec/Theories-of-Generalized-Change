#!/usr/bin/env ruby

class MultipleIntegration
  def self.monte_carlo_2d(function, x_range, y_range, n_samples = 10000)
    x_min, x_max = x_range
    y_min, y_max = y_range
    
    sum = 0
    n_samples.times do
      x = x_min + rand * (x_max - x_min)
      y = y_min + rand * (y_max - y_min)
      sum += function.call(x, y)
    end
    
    area = (x_max - x_min) * (y_max - y_min)
    area * sum / n_samples
  end
  
  def self.trapezoidal_2d(function, x_range, y_range, nx = 100, ny = 100)
    x_min, x_max = x_range
    y_min, y_max = y_range
    
    dx = (x_max - x_min) / nx.to_f
    dy = (y_max - y_min) / ny.to_f
    
    integral = 0
    
    (0..nx).each do |i|
      (0..ny).each do |j|
        x = x_min + i * dx
        y = y_min + j * dy
        
        weight = 1.0
        weight *= 0.5 if i == 0 || i == nx
        weight *= 0.5 if j == 0 || j == ny
        
        integral += weight * function.call(x, y)
      end
    end
    
    integral * dx * dy
  end
  
  def self.center_of_mass_2d(density_func, x_range, y_range, resolution = 50)
    x_min, x_max = x_range
    y_min, y_max = y_range
    
    # Total mass
    mass = trapezoidal_2d(density_func, x_range, y_range, resolution, resolution)
    
    # First moments
    mx_func = ->(x, y) { x * density_func.call(x, y) }
    my_func = ->(x, y) { y * density_func.call(x, y) }
    
    mx = trapezoidal_2d(mx_func, x_range, y_range, resolution, resolution)
    my = trapezoidal_2d(my_func, x_range, y_range, resolution, resolution)
    
    [mx / mass, my / mass]
  end
  
  def self.surface_area(function, x_range, y_range, resolution = 100)
    x_min, x_max = x_range
    y_min, y_max = y_range
    
    dx = (x_max - x_min) / resolution.to_f
    dy = (y_max - y_min) / resolution.to_f
    h = 1e-5
    
    area = 0
    
    resolution.times do |i|
      resolution.times do |j|
        x = x_min + (i + 0.5) * dx
        y = y_min + (j + 0.5) * dy
        
        # Partial derivatives using numerical differentiation
        fx = (function.call(x + h, y) - function.call(x - h, y)) / (2 * h)
        fy = (function.call(x, y + h) - function.call(x, y - h)) / (2 * h)
        
        # Surface area element
        ds = Math.sqrt(1 + fx**2 + fy**2)
        area += ds * dx * dy
      end
    end
    
    area
  end
  
  def self.polar_integration(f_polar, r_range, theta_range, nr = 50, ntheta = 50)
    r_min, r_max = r_range
    theta_min, theta_max = theta_range
    
    dr = (r_max - r_min) / nr.to_f
    dtheta = (theta_max - theta_min) / ntheta.to_f
    
    integral = 0
    
    nr.times do |i|
      ntheta.times do |j|
        r = r_min + (i + 0.5) * dr
        theta = theta_min + (j + 0.5) * dtheta
        
        # Include Jacobian factor r for polar coordinates
        integral += f_polar.call(r, theta) * r * dr * dtheta
      end
    end
    
    integral
  end
  
  def self.save_surface_data(function, x_range, y_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,z"
      
      x_min, x_max = x_range
      y_min, y_max = y_range
      
      dx = (x_max - x_min) / resolution.to_f
      dy = (y_max - y_min) / resolution.to_f
      
      (0..resolution).each do |i|
        (0..resolution).each do |j|
          x = x_min + i * dx
          y = y_min + j * dy
          z = function.call(x, y)
          file.puts "#{x},#{y},#{z}"
        end
      end
    end
    
    puts "Surface data written to #{filename}"
  end
  
  def self.method_comparison(function, x_range, y_range, exact_value = nil)
    puts "Method Comparison for given function:"
    puts "=" * 40
    
    # Monte Carlo with different sample sizes
    [1000, 10000, 100000].each do |n|
      mc_result = monte_carlo_2d(function, x_range, y_range, n)
      error = exact_value ? (mc_result - exact_value).abs : "unknown"
      puts "Monte Carlo (n=#{n}): #{mc_result.round(6)} (error: #{error})"
    end
    
    # Trapezoidal with different resolutions
    [20, 50, 100].each do |n|
      trap_result = trapezoidal_2d(function, x_range, y_range, n, n)
      error = exact_value ? (trap_result - exact_value).abs : "unknown"
      puts "Trapezoidal (#{n}×#{n}): #{trap_result.round(6)} (error: #{error})"
    end
    
    puts "Exact value: #{exact_value}" if exact_value
    puts
  end
end

# Define example functions
gaussian_2d = ->(x, y) { Math.exp(-(x**2 + y**2)) }
paraboloid = ->(x, y) { x**2 + y**2 }
saddle = ->(x, y) { x**2 - y**2 }
simple_product = ->(x, y) { x * y }
sine_product = ->(x, y) { Math.sin(x) * Math.cos(y) }

puts "Multiple Integration Examples (Ruby version)"
puts "============================================"
puts

# Example 1: Gaussian integral
puts "Example 1: Double integral of exp(-(x²+y²)) over [-2,2]×[-2,2]"
puts "=" * 60

mc_result = MultipleIntegration.monte_carlo_2d(gaussian_2d, [-2, 2], [-2, 2], 50000)
trap_result = MultipleIntegration.trapezoidal_2d(gaussian_2d, [-2, 2], [-2, 2], 100, 100)

puts "Monte Carlo result: #{mc_result.round(6)}"
puts "Trapezoidal result: #{trap_result.round(6)}"
puts "Theoretical (π): #{Math::PI.round(6)}"
puts "MC Error: #{(mc_result - Math::PI).abs.round(6)}"
puts "Trap Error: #{(trap_result - Math::PI).abs.round(6)}"

MultipleIntegration.save_surface_data(gaussian_2d, [-3, 3], [-3, 3], 100, "gaussian_2d_ruby.csv")

# Example 2: Volume calculation
puts "\n" + "=" * 70
puts "Example 2: Volume under z = x² + y² over [0,2]×[0,2]"
puts "=" * 50

volume = MultipleIntegration.trapezoidal_2d(paraboloid, [0, 2], [0, 2], 100, 100)
theoretical_volume = 32.0 / 3.0

puts "Calculated volume: #{volume.round(6)}"
puts "Theoretical volume: #{theoretical_volume.round(6)}"
puts "Error: #{(volume - theoretical_volume).abs.round(6)}"

MultipleIntegration.save_surface_data(paraboloid, [0, 2], [0, 2], 100, "paraboloid_ruby.csv")

# Example 3: Center of mass
puts "\n" + "=" * 70
puts "Example 3: Center of mass calculation"
puts "=" * 50

# Triangular region with density ρ(x,y) = 2 - x - y
density_func = lambda do |x, y|
  if x >= 0 && y >= 0 && x + y <= 1
    2 - x - y
  else
    0
  end
end

cm = MultipleIntegration.center_of_mass_2d(density_func, [0, 1], [0, 1], 100)
puts "Center of mass: (#{cm[0].round(6)}, #{cm[1].round(6)})"

# Example 4: Surface area
puts "\n" + "=" * 70
puts "Example 4: Surface area calculation"
puts "=" * 50

surf_area = MultipleIntegration.surface_area(paraboloid, [0, 1], [0, 1], 50)
puts "Surface area of z = x² + y² over [0,1]×[0,1]: #{surf_area.round(6)}"

# Example 5: Polar integration
puts "\n" + "=" * 70
puts "Example 5: Polar coordinate integration"
puts "=" * 50

polar_func = ->(r, theta) { r * Math.exp(-r**2) }
polar_result = MultipleIntegration.polar_integration(polar_func, [0, 2], [0, 2*Math::PI])
polar_theoretical = Math::PI * (1 - Math.exp(-4))

puts "Polar integration of r*exp(-r²) over disk:"
puts "Calculated: #{polar_result.round(6)}"
puts "Theoretical: #{polar_theoretical.round(6)}"
puts "Error: #{(polar_result - polar_theoretical).abs.round(6)}"

# Example 6: Method comparisons
puts "\n" + "=" * 70
puts "Example 6: Numerical method comparisons"
puts "=" * 50

puts "\nFor f(x,y) = xy over [0,1]×[0,1] (exact = 0.25):"
MultipleIntegration.method_comparison(simple_product, [0, 1], [0, 1], 0.25)

puts "For f(x,y) = sin(x)cos(y) over [0,π]×[0,π] (exact = 4):"
MultipleIntegration.method_comparison(sine_product, [0, Math::PI], [0, Math::PI], 4.0)

# Advanced applications
puts "\n" + "=" * 70
puts "Advanced Applications"
puts "=" * 50

# Moment of inertia
puts "\n1. Moment of Inertia:"
moment_func = ->(x, y) { (x**2 + y**2) * density_func.call(x, y) }
moment = MultipleIntegration.trapezoidal_2d(moment_func, [0, 1], [0, 1], 100, 100)
puts "   I = ∫∫ r² ρ(x,y) dA = #{moment.round(6)}"

# Average value
puts "\n2. Average Value:"
avg_func = ->(x, y) { Math.sin(Math::PI * x) * Math.sin(Math::PI * y) }
area = 1.0  # Area of [0,1]×[0,1]
avg_integral = MultipleIntegration.trapezoidal_2d(avg_func, [0, 1], [0, 1], 100, 100)
average_value = avg_integral / area
puts "   Average of sin(πx)sin(πy) over [0,1]×[0,1] = #{average_value.round(6)}"

# Probability applications
puts "\n3. Probability Applications:"
# Joint probability density (simplified)
pdf = ->(x, y) { x >= 0 && y >= 0 && x + y <= 1 ? 2 : 0 }
total_prob = MultipleIntegration.trapezoidal_2d(pdf, [0, 1], [0, 1], 100, 100)
puts "   Total probability: #{total_prob.round(6)} (should be 1)"

# Expected values
ex_func = ->(x, y) { x * pdf.call(x, y) }
ey_func = ->(x, y) { y * pdf.call(x, y) }
ex = MultipleIntegration.trapezoidal_2d(ex_func, [0, 1], [0, 1], 100, 100)
ey = MultipleIntegration.trapezoidal_2d(ey_func, [0, 1], [0, 1], 100, 100)
puts "   E[X] = #{ex.round(6)}, E[Y] = #{ey.round(6)}"

puts "\n" + "=" * 70
puts "Integration Techniques Summary"
puts "=" * 50

techniques = {
  "Monte Carlo" => [
    "Random sampling approach",
    "Error ∝ 1/√n",
    "Good for high dimensions",
    "Simple for complex regions"
  ],
  "Trapezoidal Rule" => [
    "Grid-based approach", 
    "Error ∝ 1/n²",
    "Better for smooth functions",
    "Requires rectangular grids"
  ],
  "Polar Coordinates" => [
    "Transform: x = r cos θ, y = r sin θ",
    "Jacobian: r",
    "Good for circular symmetry",
    "∫∫ f(r,θ) r dr dθ"
  ]
}

techniques.each do |method, properties|
  puts "\n#{method}:"
  properties.each { |prop| puts "  • #{prop}" }
end

puts "\nNote: All CSV files generated for 3D visualization in plotting software."