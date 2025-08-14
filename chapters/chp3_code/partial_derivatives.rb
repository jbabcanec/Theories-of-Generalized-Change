#!/usr/bin/env ruby

class PartialDerivatives
  def initialize(function_proc, h = 1e-5)
    @func = function_proc
    @h = h
  end
  
  def partial_x(x, y)
    (@func.call(x + @h, y) - @func.call(x - @h, y)) / (2 * @h)
  end
  
  def partial_y(x, y)
    (@func.call(x, y + @h) - @func.call(x, y - @h)) / (2 * @h)
  end
  
  def gradient(x, y)
    [partial_x(x, y), partial_y(x, y)]
  end
  
  def directional_derivative(x, y, dir_x, dir_y)
    grad = gradient(x, y)
    magnitude = Math.sqrt(dir_x**2 + dir_y**2)
    return 0 if magnitude == 0
    
    unit_x = dir_x / magnitude
    unit_y = dir_y / magnitude
    grad[0] * unit_x + grad[1] * unit_y
  end
  
  def second_partial_xx(x, y)
    (@func.call(x + @h, y) - 2 * @func.call(x, y) + @func.call(x - @h, y)) / (@h**2)
  end
  
  def second_partial_yy(x, y)
    (@func.call(x, y + @h) - 2 * @func.call(x, y) + @func.call(x, y - @h)) / (@h**2)
  end
  
  def second_partial_xy(x, y)
    (@func.call(x + @h, y + @h) - @func.call(x + @h, y - @h) -
     @func.call(x - @h, y + @h) + @func.call(x - @h, y - @h)) / (4 * @h**2)
  end
  
  def hessian_matrix(x, y)
    fxx = second_partial_xx(x, y)
    fyy = second_partial_yy(x, y)
    fxy = second_partial_xy(x, y)
    [[fxx, fxy], [fxy, fyy]]
  end
  
  def classify_critical_point(x, y)
    fxx = second_partial_xx(x, y)
    fyy = second_partial_yy(x, y)
    fxy = second_partial_xy(x, y)
    
    discriminant = fxx * fyy - fxy**2
    
    puts "At point (#{x.round(3)}, #{y.round(3)}):"
    puts "f_xx = #{fxx.round(6)}, f_yy = #{fyy.round(6)}, f_xy = #{fxy.round(6)}"
    puts "Discriminant D = #{discriminant.round(6)}"
    
    if discriminant > 0
      if fxx > 0
        puts "Classification: Local minimum"
      else
        puts "Classification: Local maximum"
      end
    elsif discriminant < 0
      puts "Classification: Saddle point"
    else
      puts "Classification: Test inconclusive"
    end
    puts
  end
  
  def find_critical_points(x_range, y_range, resolution = 50, tolerance = 1e-4)
    critical_points = []
    dx = (x_range[1] - x_range[0]) / resolution.to_f
    dy = (y_range[1] - y_range[0]) / resolution.to_f
    
    (0..resolution).each do |i|
      (0..resolution).each do |j|
        x = x_range[0] + i * dx
        y = y_range[0] + j * dy
        grad = gradient(x, y)
        
        if grad[0].abs < tolerance && grad[1].abs < tolerance
          critical_points << [x, y]
        end
      end
    end
    
    critical_points
  end
  
  def tangent_plane_equation(x0, y0)
    f0 = @func.call(x0, y0)
    grad = gradient(x0, y0)
    
    puts "Tangent plane at (#{x0}, #{y0}):"
    puts "L(x,y) = #{f0.round(6)} + #{grad[0].round(6)}(x - #{x0}) + #{grad[1].round(6)}(y - #{y0})"
    
    # Return lambda for evaluation
    ->(x, y) { f0 + grad[0] * (x - x0) + grad[1] * (y - y0) }
  end
  
  def save_gradient_field(x_range, y_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,fx,fy,magnitude,angle"
      
      dx = (x_range[1] - x_range[0]) / resolution.to_f
      dy = (y_range[1] - y_range[0]) / resolution.to_f
      
      (0..resolution).each do |i|
        (0..resolution).each do |j|
          x = x_range[0] + i * dx
          y = y_range[0] + j * dy
          grad = gradient(x, y)
          magnitude = Math.sqrt(grad[0]**2 + grad[1]**2)
          angle = Math.atan2(grad[1], grad[0])
          
          file.puts "#{x},#{y},#{grad[0]},#{grad[1]},#{magnitude},#{angle}"
        end
      end
    end
    
    puts "Gradient field data written to #{filename}"
  end
  
  def save_contour_data(x_range, y_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,z"
      
      dx = (x_range[1] - x_range[0]) / resolution.to_f
      dy = (y_range[1] - y_range[0]) / resolution.to_f
      
      (0..resolution).each do |i|
        (0..resolution).each do |j|
          x = x_range[0] + i * dx
          y = y_range[0] + j * dy
          z = @func.call(x, y)
          
          file.puts "#{x},#{y},#{z}"
        end
      end
    end
    
    puts "Contour data written to #{filename}"
  end
  
  def analyze_function(x_range, y_range, test_points)
    puts "Function Analysis Summary"
    puts "=" * 50
    
    puts "\nCritical Points:"
    critical_points = find_critical_points(x_range, y_range, 30, 1e-3)
    if critical_points.empty?
      puts "No critical points found in the specified range."
    else
      critical_points.each { |point| classify_critical_point(point[0], point[1]) }
    end
    
    puts "\nGradient Analysis at Test Points:"
    puts "Point\t\tf(x,y)\t\t∇f\t\t\t|∇f|"
    puts "-" * 70
    
    test_points.each do |point|
      x, y = point
      fx = @func.call(x, y)
      grad = gradient(x, y)
      grad_mag = Math.sqrt(grad[0]**2 + grad[1]**2)
      
      puts "(#{x},#{y})\t\t#{fx.round(4)}\t\t[#{grad[0].round(4)},#{grad[1].round(4)}]\t#{grad_mag.round(4)}"
    end
  end
end

# Define example functions
example_1 = ->(x, y) { Math.sin(x) * Math.cos(y) + 0.5 * x }
saddle = ->(x, y) { x**2 - y**2 + 2*x*y }
gaussian = ->(x, y) { Math.exp(-(x**2 + y**2)) * Math.cos(x) * Math.sin(y) }
rosenbrock = ->(x, y) { (1 - x)**2 + 100 * (y - x**2)**2 }
paraboloid = ->(x, y) { x**2 + y**2 }

puts "Partial Derivatives Analysis (Ruby version)"
puts "==========================================="
puts

# Example 1: Mixed trigonometric function
puts "Example 1: f(x,y) = sin(x)cos(y) + 0.5x"
puts "=" * 50
pd1 = PartialDerivatives.new(example_1)

x0, y0 = Math::PI/4, Math::PI/3
grad1 = pd1.gradient(x0, y0)
dir_deriv = pd1.directional_derivative(x0, y0, 1, 1)

puts "At point (#{x0.round(6)}, #{y0.round(6)}):"
puts "Function value: #{example_1.call(x0, y0).round(6)}"
puts "Gradient: [#{grad1[0].round(6)}, #{grad1[1].round(6)}]"
puts "Gradient magnitude: #{Math.sqrt(grad1[0]**2 + grad1[1]**2).round(6)}"
puts "Directional derivative in direction [1,1]: #{dir_deriv.round(6)}"

tangent_plane = pd1.tangent_plane_equation(x0, y0)

# Test tangent plane approximation
test_x, test_y = x0 + 0.1, y0 + 0.1
actual = example_1.call(test_x, test_y)
approx = tangent_plane.call(test_x, test_y)
error = (actual - approx).abs

puts "\nTangent plane approximation test:"
puts "At nearby point (#{test_x.round(3)}, #{test_y.round(3)}):"
puts "Actual value: #{actual.round(6)}"
puts "Linear approximation: #{approx.round(6)}"
puts "Error: #{error.round(6)}"

pd1.save_gradient_field([-Math::PI, Math::PI], [-Math::PI, Math::PI], 20, "gradient_field_ruby_1.csv")
pd1.save_contour_data([-Math::PI, Math::PI], [-Math::PI, Math::PI], 100, "contour_data_ruby_1.csv")

# Example 2: Saddle function
puts "\n" + "=" * 70
puts "Example 2: f(x,y) = x² - y² + 2xy (Saddle function)"
puts "=" * 50
pd2 = PartialDerivatives.new(saddle)

test_points = [[0, 0], [1, 1], [-1, 1], [2, 0], [0, 2]]
pd2.analyze_function([-3, 3], [-3, 3], test_points)

pd2.save_gradient_field([-3, 3], [-3, 3], 20, "gradient_field_ruby_2.csv")

# Example 3: Gaussian function
puts "\n" + "=" * 70
puts "Example 3: f(x,y) = exp(-(x²+y²))cos(x)sin(y)"
puts "=" * 50
pd3 = PartialDerivatives.new(gaussian)

pd3.analyze_function([-3, 3], [-3, 3], test_points)
pd3.save_gradient_field([-3, 3], [-3, 3], 25, "gradient_field_ruby_3.csv")

# Example 4: Rosenbrock function
puts "\n" + "=" * 70
puts "Example 4: Rosenbrock function (optimization classic)"
puts "=" * 50
pd4 = PartialDerivatives.new(rosenbrock)

# Focus on area around the minimum
rosenbrock_points = [[1, 1], [0, 0], [1.5, 2], [0.5, 0.5], [2, 4]]
pd4.analyze_function([-2, 2], [-1, 3], rosenbrock_points)

pd4.save_gradient_field([-2, 2], [-1, 3], 30, "gradient_field_ruby_rosenbrock.csv")

# Chain rule demonstration
puts "\n" + "=" * 70
puts "Chain Rule Application"
puts "=" * 50
puts "For f(x,y) = sin(x)cos(y) + 0.5x along curve x = t, y = t²"

puts "\nt\tdf/dt (chain rule)"
puts "-" * 25

(0..2).step(0.5) do |t|
  x = t
  y = t**2
  grad = pd1.gradient(x, y)
  dx_dt = 1
  dy_dt = 2 * t
  df_dt = grad[0] * dx_dt + grad[1] * dy_dt
  
  puts "#{t}\t#{df_dt.round(6)}"
end

# Level curves analysis
puts "\n" + "=" * 70
puts "Level Curves Analysis for Paraboloid f(x,y) = x² + y²"
puts "=" * 50

pd5 = PartialDerivatives.new(paraboloid)
levels = [1, 4, 9, 16, 25]

levels.each do |level|
  puts "\nLevel curve f(x,y) = #{level}:"
  puts "This is a circle with radius r = #{Math.sqrt(level).round(3)}"
  
  # Sample points on the level curve
  angles = [0, Math::PI/4, Math::PI/2, 3*Math::PI/4, Math::PI]
  angles.each do |theta|
    r = Math.sqrt(level)
    x = r * Math.cos(theta)
    y = r * Math.sin(theta)
    grad = pd5.gradient(x, y)
    
    puts "  At (#{x.round(3)}, #{y.round(3)}): ∇f = [#{grad[0].round(3)}, #{grad[1].round(3)}]"
  end
end

puts "\n" + "=" * 70
puts "Mathematical Properties Summary"
puts "=" * 50

puts "\n1. Gradient Properties:"
puts "   - Points in direction of steepest increase"
puts "   - Perpendicular to level curves"
puts "   - Zero at critical points"

puts "\n2. Second Derivative Test:"
puts "   - D > 0, f_xx > 0: Local minimum"
puts "   - D > 0, f_xx < 0: Local maximum"
puts "   - D < 0: Saddle point"
puts "   - D = 0: Inconclusive"

puts "\n3. Chain Rule:"
puts "   - For f(x(t), y(t)): df/dt = (∂f/∂x)(dx/dt) + (∂f/∂y)(dy/dt)"

puts "\n4. Directional Derivative:"
puts "   - D_u f = ∇f · u (where u is unit vector)"
puts "   - Maximum in direction of gradient"

puts "\nAll CSV files generated for visualization in plotting software."