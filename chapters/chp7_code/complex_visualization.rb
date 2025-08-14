#!/usr/bin/env ruby

class ComplexVisualization
  def self.domain_coloring_comprehensive(function, x_range, y_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,real_output,imag_output,magnitude,argument,hue,brightness,is_pole,is_zero"
      
      x_min, x_max = x_range
      y_min, y_max = y_range
      
      dx = (x_max - x_min) / resolution.to_f
      dy = (y_max - y_min) / resolution.to_f
      
      (0..resolution).each do |i|
        (0..resolution).each do |j|
          x = x_min + i * dx
          y = y_min + j * dy
          
          z = Complex(x, y)
          
          begin
            w = function.call(z)
            magnitude = w.abs
            argument = w.arg
            
            is_pole = magnitude > 1e6 ? 1 : 0
            is_zero = magnitude < 1e-6 ? 1 : 0
            
            hue = (argument + Math::PI) / (2 * Math::PI)
            brightness = Math.log(1 + magnitude)
            
            file.puts "#{x},#{y},#{w.real},#{w.imag},#{magnitude},#{argument},#{hue},#{brightness},#{is_pole},#{is_zero}"
          rescue
            file.puts "#{x},#{y},NaN,NaN,NaN,NaN,NaN,NaN,1,0"
          end
        end
      end
    end
    
    puts "Complex visualization data saved to #{filename}"
  end
  
  def self.grid_mapping(function, x_range, y_range, grid_lines, filename)
    File.open(filename, 'w') do |file|
      file.puts "type,x,y,mapped_x,mapped_y"
      
      x_min, x_max = x_range
      y_min, y_max = y_range
      
      # Vertical lines
      (0..grid_lines).each do |i|
        x = x_min + i * (x_max - x_min) / grid_lines.to_f
        (0..100).each do |j|
          y = y_min + j * (y_max - y_min) / 100.0
          z = Complex(x, y)
          
          begin
            w = function.call(z)
            file.puts "vertical,#{x},#{y},#{w.real},#{w.imag}"
          rescue
            file.puts "vertical,#{x},#{y},NaN,NaN"
          end
        end
      end
      
      # Horizontal lines
      (0..grid_lines).each do |j|
        y = y_min + j * (y_max - y_min) / grid_lines.to_f
        (0..100).each do |i|
          x = x_min + i * (x_max - x_min) / 100.0
          z = Complex(x, y)
          
          begin
            w = function.call(z)
            file.puts "horizontal,#{x},#{y},#{w.real},#{w.imag}"
          rescue
            file.puts "horizontal,#{x},#{y},NaN,NaN"
          end
        end
      end
    end
    
    puts "Grid mapping data saved to #{filename}"
  end
end

puts "Complex Function Visualization (Ruby version)"
puts "============================================="

# Define complex functions
f_square = ->(z) { z**2 }
f_exp = ->(z) { Complex.exp(z) }
f_sin = ->(z) { Complex.sin(z) }
f_mobius = ->(z) { (z - 1) / (z + 1) }

ComplexVisualization.domain_coloring_comprehensive(f_square, [-2, 2], [-2, 2], 200, "z_squared_visualization_ruby.csv")
ComplexVisualization.domain_coloring_comprehensive(f_exp, [-2, 2], [-6, 6], 200, "exp_z_visualization_ruby.csv")
ComplexVisualization.domain_coloring_comprehensive(f_mobius, [-3, 3], [-3, 3], 200, "mobius_visualization_ruby.csv")

ComplexVisualization.grid_mapping(f_square, [-2, 2], [-2, 2], 10, "grid_mapping_z2_ruby.csv")