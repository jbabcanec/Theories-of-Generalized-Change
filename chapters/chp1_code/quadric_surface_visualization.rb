#!/usr/bin/env ruby

class QuadricSurface
  attr_reader :A, :B, :C, :D, :E, :F, :G, :H, :I, :J
  
  def initialize(a = 1, b = 1, c = 0, d = 0, e = 0, f = -1, g = 0, h = 0, i = 0, j = 0)
    @A, @B, @C, @D, @E, @F, @G, @H, @I, @J = a, b, c, d, e, f, g, h, i, j
  end
  
  def evaluate(x, y)
    @A * x**2 + @B * y**2 + @C * x * y + @D * x + @E * y + @F
  end
  
  def surface_type
    discriminant = 4 * @A * @B - @C**2
    
    if discriminant > 0
      if @A > 0 && @B > 0
        "Elliptic Paraboloid"
      elsif @A < 0 && @B < 0
        "Elliptic Paraboloid (inverted)"
      else
        "Elliptic surface"
      end
    elsif discriminant < 0
      "Hyperbolic Paraboloid (Saddle)"
    else
      "Parabolic Cylinder or Degenerate"
    end
  end
  
  def equation_string
    terms = []
    terms << "#{@A}x²" if @A != 0
    terms << "#{@B}y²" if @B != 0
    terms << "#{@C}xy" if @C != 0
    terms << "#{@D}x" if @D != 0
    terms << "#{@E}y" if @E != 0
    terms << @F.to_s if @F != 0
    
    equation = terms.empty? ? "0" : terms.join(" + ").gsub("+ -", "- ")
    "z = #{equation}"
  end
  
  def generate_mesh(x_min, x_max, y_min, y_max, num_points, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,z"
      
      dx = (x_max - x_min) / (num_points - 1).to_f
      dy = (y_max - y_min) / (num_points - 1).to_f
      
      num_points.times do |i|
        num_points.times do |j|
          x = x_min + i * dx
          y = y_min + j * dy
          z = evaluate(x, y)
          file.puts "#{x},#{y},#{z}"
        end
      end
    end
    
    puts "Surface data written to #{filename}"
  end
  
  def analyze_critical_points(x_range, y_range)
    # For quadric surfaces, critical points occur where partial derivatives are zero
    # ∂z/∂x = 2Ax + Cy + D = 0
    # ∂z/∂y = 2By + Cx + E = 0
    
    det = 4 * @A * @B - @C**2
    
    if det != 0
      # Unique critical point
      x_crit = (@C * @E - 2 * @B * @D) / det
      y_crit = (@C * @D - 2 * @A * @E) / det
      
      if x_crit >= x_range[0] && x_crit <= x_range[1] && 
         y_crit >= y_range[0] && y_crit <= y_range[1]
        z_crit = evaluate(x_crit, y_crit)
        puts "Critical point found: (#{x_crit.round(3)}, #{y_crit.round(3)}, #{z_crit.round(3)})"
        
        # Classify critical point using second derivative test
        fxx = 2 * @A
        fyy = 2 * @B
        fxy = @C
        
        discriminant = fxx * fyy - fxy**2
        
        if discriminant > 0
          if fxx > 0
            puts "Type: Local minimum"
          else
            puts "Type: Local maximum"
          end
        elsif discriminant < 0
          puts "Type: Saddle point"
        else
          puts "Type: Inconclusive (second derivative test fails)"
        end
      else
        puts "Critical point exists but outside specified range"
      end
    else
      puts "No unique critical point (degenerate case)"
    end
  end
  
  def print_info
    puts "Quadric Surface Information:"
    puts "Equation: #{equation_string}"
    puts "Surface type: #{surface_type}"
    puts "Coefficients: A=#{@A}, B=#{@B}, C=#{@C}, D=#{@D}, E=#{@E}, F=#{@F}"
  end
end

puts "Quadric Surface Analysis (Ruby version)"
puts "======================================="
puts

# Example 1: Elliptic Paraboloid
puts "Example 1: Elliptic Paraboloid"
surface1 = QuadricSurface.new(1, 1, 0, 0, 0, -1)
surface1.print_info
surface1.analyze_critical_points([-3, 3], [-3, 3])
surface1.generate_mesh(-2, 2, -2, 2, 50, "elliptic_paraboloid_ruby.csv")

puts "\nSample points:"
(-1..1).step(0.5) do |x|
  (-1..1).step(0.5) do |y|
    puts "f(#{x}, #{y}) = #{surface1.evaluate(x, y)}"
  end
end

# Example 2: Hyperbolic Paraboloid (Saddle)
puts "\n" + "="*50
puts "Example 2: Hyperbolic Paraboloid (Saddle)"
surface2 = QuadricSurface.new(1, -1, 0, 0, 0, 0)
surface2.print_info
surface2.analyze_critical_points([-3, 3], [-3, 3])
surface2.generate_mesh(-2, 2, -2, 2, 50, "hyperbolic_paraboloid_ruby.csv")

# Example 3: More complex surface
puts "\n" + "="*50
puts "Example 3: Complex Surface with Cross Term"
surface3 = QuadricSurface.new(2, 3, 1, -1, 2, -5)
surface3.print_info
surface3.analyze_critical_points([-3, 3], [-3, 3])
surface3.generate_mesh(-3, 3, -3, 3, 75, "complex_surface_ruby.csv")

# Level curves analysis
puts "\n" + "="*50
puts "Level Curves Analysis for Surface 1:"
levels = [-2, -1, 0, 1, 2]
levels.each do |level|
  puts "\nLevel curve z = #{level}:"
  puts "Equation: #{surface1.A}x² + #{surface1.B}y² = #{level - surface1.F}"
  
  if level - surface1.F > 0
    a = Math.sqrt((level - surface1.F) / surface1.A)
    b = Math.sqrt((level - surface1.F) / surface1.B)
    puts "Ellipse with semi-axes a=#{a.round(3)}, b=#{b.round(3)}"
  elsif level - surface1.F == 0
    puts "Point (origin)"
  else
    puts "No real solutions (empty set)"
  end
end

puts "\nNote: Generated CSV files can be visualized using:"
puts "- Python with matplotlib"
puts "- R with plot3d"
puts "- Excel or Google Sheets with 3D plotting"
puts "- Any scientific plotting software"