#!/usr/bin/env ruby

class ComplexFunction
  def self.domain_coloring_data(function, x_range, y_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,real_output,imag_output,magnitude,argument,hue,brightness"
      
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
            hue = (argument + Math::PI) / (2 * Math::PI)
            brightness = Math.log(1 + magnitude)
            
            file.puts "#{x},#{y},#{w.real},#{w.imag},#{magnitude},#{argument},#{hue},#{brightness}"
          rescue
            # Skip points where function is undefined
          end
        end
      end
    end
    
    puts "Domain coloring data saved to #{filename}"
  end
end

puts "Domain Coloring Analysis (Ruby version)"
puts "======================================="

# Generate data for f(z) = z^2
square_func = ->(z) { z**2 }
ComplexFunction.domain_coloring_data(square_func, [-2, 2], [-2, 2], 200, "z_squared_ruby.csv")

# Generate data for f(z) = 1/z
inverse_func = ->(z) { 1.0 / z }
ComplexFunction.domain_coloring_data(inverse_func, [-2, 2], [-2, 2], 200, "inverse_z_ruby.csv")