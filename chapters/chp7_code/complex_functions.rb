#!/usr/bin/env ruby

class Complex
  # Extend Ruby's built-in Complex class with additional methods for visualization
  
  def self.from_polar(r, theta)
    new(r * Math.cos(theta), r * Math.sin(theta))
  end
  
  def to_polar
    [abs, arg]
  end
  
  def pretty_print
    if imag >= 0
      "#{real.round(3)} + #{imag.round(3)}i"
    else
      "#{real.round(3)} - #{(-imag).round(3)}i"
    end
  end
end

class ComplexFunction
  def initialize(&block)
    @function = block
  end
  
  def call(z)
    @function.call(z)
  end
  
  def evaluate_grid(x_range, y_range, resolution = 50)
    results = []
    x_step = (x_range[1] - x_range[0]) / (resolution - 1).to_f
    y_step = (y_range[1] - y_range[0]) / (resolution - 1).to_f
    
    resolution.times do |i|
      row = []
      resolution.times do |j|
        x = x_range[0] + i * x_step
        y = y_range[0] + j * y_step
        z = Complex(x, y)
        
        begin
          w = call(z)
          row << { input: z, output: w, magnitude: w.abs, argument: w.arg }
        rescue => e
          row << { input: z, output: nil, magnitude: Float::INFINITY, argument: 0 }
        end
      end
      results << row
    end
    
    results
  end
  
  def find_zeros(x_range, y_range, resolution = 20, tolerance = 0.01)
    zeros = []
    grid = evaluate_grid(x_range, y_range, resolution)
    
    grid.each do |row|
      row.each do |point|
        if point[:output] && point[:magnitude] < tolerance
          zeros << point[:input]
        end
      end
    end
    
    zeros
  end
  
  def find_poles(x_range, y_range, resolution = 20, threshold = 100)
    poles = []
    grid = evaluate_grid(x_range, y_range, resolution)
    
    grid.each do |row|
      row.each do |point|
        if point[:magnitude] > threshold
          poles << point[:input]
        end
      end
    end
    
    poles
  end
  
  def save_domain_coloring_data(x_range, y_range, filename, resolution = 100)
    File.open(filename, 'w') do |file|
      file.puts "x,y,real_output,imag_output,magnitude,argument,hue,brightness"
      
      grid = evaluate_grid(x_range, y_range, resolution)
      grid.each do |row|
        row.each do |point|
          if point[:output]
            x = point[:input].real
            y = point[:input].imag
            w = point[:output]
            
            # Domain coloring values
            hue = (point[:argument] + Math::PI) / (2 * Math::PI)
            brightness = Math.log(1 + point[:magnitude])
            
            file.puts "#{x},#{y},#{w.real},#{w.imag},#{point[:magnitude]},#{point[:argument]},#{hue},#{brightness}"
          end
        end
      end
    end
    
    puts "Domain coloring data saved to #{filename}"
  end
end

# Define some interesting complex functions
puts "Complex Function Analysis (Ruby version)"
puts "========================================"
puts

# Function 1: z^2
square_function = ComplexFunction.new { |z| z**2 }
puts "Function 1: f(z) = z²"

test_points = [Complex(1, 0), Complex(0, 1), Complex(1, 1), Complex(-1, 1)]
puts "Sample evaluations:"
test_points.each do |z|
  w = square_function.call(z)
  puts "f(#{z.pretty_print}) = #{w.pretty_print}"
end

zeros = square_function.find_zeros([-2, 2], [-2, 2])
puts "Zeros found: #{zeros.map(&:pretty_print)}"
puts

# Function 2: 1/(z^2 + 1)
rational_function = ComplexFunction.new { |z| 1.0 / (z**2 + 1) }
puts "Function 2: f(z) = 1/(z² + 1)"

puts "Sample evaluations:"
test_points.each do |z|
  begin
    w = rational_function.call(z)
    puts "f(#{z.pretty_print}) = #{w.pretty_print}"
  rescue => e
    puts "f(#{z.pretty_print}) = undefined (#{e.message})"
  end
end

poles = rational_function.find_poles([-3, 3], [-3, 3])
puts "Poles found: #{poles.map(&:pretty_print)}"
puts

# Function 3: exp(z)
exp_function = ComplexFunction.new { |z| 
  Math.exp(z.real) * Complex(Math.cos(z.imag), Math.sin(z.imag))
}
puts "Function 3: f(z) = exp(z)"

puts "Sample evaluations:"
test_points.each do |z|
  w = exp_function.call(z)
  puts "f(#{z.pretty_print}) = #{w.pretty_print}"
end
puts

# Function 4: sin(z)
sin_function = ComplexFunction.new { |z|
  exp_iz = exp_function.call(Complex(0, 1) * z)
  exp_neg_iz = exp_function.call(Complex(0, -1) * z)
  (exp_iz - exp_neg_iz) / Complex(0, 2)
}
puts "Function 4: f(z) = sin(z)"

puts "Sample evaluations:"
test_points.each do |z|
  w = sin_function.call(z)
  puts "f(#{z.pretty_print}) = #{w.pretty_print}"
end
puts

# Möbius transformation
mobius_function = ComplexFunction.new { |z| (z - 1) / (z + 1) }
puts "Function 5: Möbius transformation f(z) = (z-1)/(z+1)"

puts "Sample evaluations:"
test_points.each do |z|
  begin
    w = mobius_function.call(z)
    puts "f(#{z.pretty_print}) = #{w.pretty_print}"
  rescue => e
    puts "f(#{z.pretty_print}) = undefined"
  end
end
puts

# Analysis of periodicity for exp(z)
puts "Periodicity Analysis for exp(z):"
z_base = Complex(1, 0)
puts "f(#{z_base.pretty_print}) = #{exp_function.call(z_base).pretty_print}"

# exp(z + 2πi) = exp(z)
z_plus_period = z_base + Complex(0, 2 * Math::PI)
puts "f(#{z_plus_period.pretty_print}) = #{exp_function.call(z_plus_period).pretty_print}"
puts "Difference: #{(exp_function.call(z_plus_period) - exp_function.call(z_base)).abs.round(10)}"
puts

# Generate domain coloring data
puts "Generating domain coloring data files..."
square_function.save_domain_coloring_data([-2, 2], [-2, 2], "z_squared_ruby.csv")
rational_function.save_domain_coloring_data([-3, 3], [-3, 3], "rational_function_ruby.csv")
exp_function.save_domain_coloring_data([-2, 2], [-6, 6], "exp_function_ruby.csv")

# Special values and properties
puts "\nSpecial Values and Properties:"

# Roots of unity
puts "Cube roots of unity (solutions to z³ = 1):"
3.times do |k|
  angle = 2 * Math::PI * k / 3
  root = Complex.from_polar(1, angle)
  cube = root**3
  puts "ω#{k} = #{root.pretty_print}, (ω#{k})³ = #{cube.pretty_print}"
end

# Euler's formula verification
puts "\nEuler's formula verification:"
theta = Math::PI / 4
euler_left = exp_function.call(Complex(0, theta))
euler_right = Complex(Math.cos(theta), Math.sin(theta))
puts "e^(iπ/4) = #{euler_left.pretty_print}"
puts "cos(π/4) + i*sin(π/4) = #{euler_right.pretty_print}"
puts "Difference: #{(euler_left - euler_right).abs.round(10)}"

puts "\nNote: CSV files contain domain coloring data that can be visualized"
puts "using plotting software to create colorful representations of complex functions."