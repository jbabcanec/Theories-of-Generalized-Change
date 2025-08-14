#!/usr/bin/env ruby

class CurvatureCalculator
  def initialize(function_proc, dx = 0.001)
    @func = function_proc
    @dx = dx
  end
  
  def first_derivative(x)
    (@func.call(x + @dx) - @func.call(x - @dx)) / (2 * @dx)
  end
  
  def second_derivative(x)
    (@func.call(x + @dx) - 2 * @func.call(x) + @func.call(x - @dx)) / (@dx**2)
  end
  
  def curvature(x)
    f_prime = first_derivative(x)
    f_double_prime = second_derivative(x)
    denominator = (1 + f_prime**2)**1.5
    f_double_prime.abs / denominator
  end
  
  def analyze_curvature(x_min, x_max, num_samples = 10)
    step = (x_max - x_min) / (num_samples - 1).to_f
    max_curvature = 0
    max_curvature_x = x_min
    
    puts "Curvature Analysis:"
    puts "x\t\tf(x)\t\tκ(x)"
    
    (0...num_samples).each do |i|
      x = x_min + i * step
      fx = @func.call(x)
      k = curvature(x)
      
      puts "#{x.round(3)}\t\t#{fx.round(3)}\t\t#{k.round(6)}"
      
      if k > max_curvature
        max_curvature = k
        max_curvature_x = x
      end
    end
    
    puts "\nMaximum curvature: #{max_curvature.round(6)} at x = #{max_curvature_x.round(3)}"
  end
  
  def generate_data(x_min, x_max, num_points, filename, scaling_factor = 1.0)
    step = (x_max - x_min) / (num_points - 1).to_f
    
    File.open(filename, 'w') do |file|
      file.puts "x,y,curvature"
      
      (0...num_points).each do |i|
        x = x_min + i * step
        y = @func.call(x)
        k = curvature(x) * scaling_factor
        file.puts "#{x},#{y},#{k}"
      end
    end
    
    puts "Data written to #{filename}"
  end
end

# Example functions
polynomial = ->(x) { x**3 - 3*x**2 + x - 2 }
sine_wave = ->(x) { Math.sin(x) }
gaussian = ->(x) { Math.exp(-x**2 / 2) }
parabola = ->(x) { x**2 }

puts "Curvature Analysis Tool (Ruby version)"
puts "======================================"
puts

# Example 1: Polynomial function
puts "Example 1: Polynomial f(x) = x³ - 3x² + x - 2"
poly_calc = CurvatureCalculator.new(polynomial)
poly_calc.analyze_curvature(-1, 4, 11)
poly_calc.generate_data(-1, 4, 100, "polynomial_curvature_ruby.csv")

# Example 2: Sine wave
puts "\n\nExample 2: Sine wave f(x) = sin(x)"
sine_calc = CurvatureCalculator.new(sine_wave)
sine_calc.analyze_curvature(-Math::PI, Math::PI, 9)

# Example 3: Gaussian
puts "\n\nExample 3: Gaussian f(x) = exp(-x²/2)"
gauss_calc = CurvatureCalculator.new(gaussian)
gauss_calc.analyze_curvature(-3, 3, 7)

# Road design example
puts "\n\nRoad Design Example: y = x²"
road_calc = CurvatureCalculator.new(parabola)

puts "Curvature at different points along the road:"
(0..5).each do |t|
  k = road_calc.curvature(t)
  radius_of_curvature = k > 0 ? 1.0 / k : Float::INFINITY
  
  print "t = #{t}: κ = #{k.round(6)}, Radius = #{radius_of_curvature.round(2)}"
  
  if radius_of_curvature < 50
    print " (Sharp curve - reduce speed!)"
  end
  puts
end

# Special case analysis
puts "\n\nSpecial Cases:"

# Circle of radius R has constant curvature 1/R
circle_radius = 5
circle = ->(x) { Math.sqrt(circle_radius**2 - x**2) }  # Upper semicircle
circle_calc = CurvatureCalculator.new(circle)

puts "Circle with radius #{circle_radius}:"
puts "Theoretical curvature: #{(1.0/circle_radius).round(6)}"
puts "Calculated curvature at x=0: #{circle_calc.curvature(0).round(6)}"

puts "\nNote: CSV files can be visualized using plotting software."